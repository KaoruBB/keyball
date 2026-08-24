import ApplicationServices
import Darwin
import Foundation
import IOKit.hid

private enum BridgeProtocol {
    static let packetSize = 32
    static let magic: [UInt8] = [0x4B, 0x42, 0x53] // "KBS"
    static let version: UInt8 = 1

    static let hostHello: UInt8 = 0x01
    static let hostGoodbye: UInt8 = 0x02
    static let hostHeartbeat: UInt8 = 0x03
    static let deviceHello: UInt8 = 0x81
    static let deviceScroll: UInt8 = 0x82

    static let versionOffset = 3
    static let messageOffset = 4
    static let sequenceOffset = 5
    static let verticalOffset = 8
    static let horizontalOffset = 10
    static let resolutionOffset = 12

    static func packet(message: UInt8) -> [UInt8] {
        var packet = [UInt8](repeating: 0, count: packetSize)
        packet[0] = magic[0]
        packet[1] = magic[1]
        packet[2] = magic[2]
        packet[versionOffset] = version
        packet[messageOffset] = message
        return packet
    }

    static func isValid(_ packet: [UInt8]) -> Bool {
        packet.count >= packetSize
            && Array(packet[0...2]) == magic
            && packet[versionOffset] == version
    }

    static func int16(_ packet: [UInt8], at offset: Int) -> Int16 {
        Int16(bitPattern: uint16(packet, at: offset))
    }

    static func uint16(_ packet: [UInt8], at offset: Int) -> UInt16 {
        UInt16(packet[offset]) | (UInt16(packet[offset + 1]) << 8)
    }
}

private struct Options {
    var pixelsPerTick = 40.0
    var invertVertical = false
    var invertHorizontal = false
    var verbose = false

    static func parse() -> Options {
        var options = Options()
        let arguments = Array(CommandLine.arguments.dropFirst())
        var index = 0

        while index < arguments.count {
            switch arguments[index] {
                case "--pixels-per-tick":
                    index += 1
                    guard index < arguments.count,
                          let value = Double(arguments[index]),
                          value.isFinite, value > 0, value <= 10_000
                    else {
                        fail("--pixels-per-tick requires a number between 0 and 10000")
                    }
                    options.pixelsPerTick = value
                case "--invert-vertical":
                    options.invertVertical = true
                case "--invert-horizontal":
                    options.invertHorizontal = true
                case "--verbose":
                    options.verbose = true
                case "--help", "-h":
                    printUsage()
                    Darwin.exit(0)
                default:
                    fail("unknown option: \(arguments[index])")
            }
            index += 1
        }

        return options
    }

    private static func fail(_ message: String) -> Never {
        FileHandle.standardError.write("keyball-scroll: \(message)\n".data(using: .utf8)!)
        printUsage()
        Darwin.exit(2)
    }

    private static func printUsage() {
        print("""
        Usage: keyball-scroll [options]

          --pixels-per-tick N   Pixel distance for one conventional wheel tick (default: 40)
          --invert-vertical     Reverse vertical scrolling
          --invert-horizontal   Reverse horizontal scrolling
          --verbose             Print connection and packet diagnostics
          -h, --help            Show this help
        """)
    }
}

private enum SingleInstanceLock {
    final class Token {
        private let descriptor: Int32

        init(descriptor: Int32) {
            self.descriptor = descriptor
        }

        deinit {
            flock(descriptor, LOCK_UN)
            Darwin.close(descriptor)
        }
    }

    static func acquire() -> Token {
        let path = URL(fileURLWithPath: NSTemporaryDirectory())
            .appendingPathComponent("keyball-scroll-\(getuid()).lock")
            .path
        let descriptor = Darwin.open(path, O_CREAT | O_RDWR | O_CLOEXEC, mode_t(S_IRUSR | S_IWUSR))
        guard descriptor >= 0 else {
            fail("cannot open instance lock: \(String(cString: strerror(errno)))")
        }
        guard flock(descriptor, LOCK_EX | LOCK_NB) == 0 else {
            Darwin.close(descriptor)
            fail("another keyball-scroll instance is already running")
        }
        return Token(descriptor: descriptor)
    }

    private static func fail(_ message: String) -> Never {
        FileHandle.standardError.write("keyball-scroll: \(message)\n".data(using: .utf8)!)
        Darwin.exit(1)
    }
}

private final class KeyballScrollBridge {
    private static let vendorID = 0x5957
    private static let productID = 0x0200
    private static let usagePage = 0xFF60
    private static let usage = 0x61

    private let options: Options
    private let manager: IOHIDManager
    private let reportBuffer: UnsafeMutablePointer<UInt8>
    private var device: IOHIDDevice?
    private var heartbeat: Timer?
    private var bridgeReady = false
    private var lastAcknowledgement = Date.distantPast
    private var lastProbe = Date.distantPast
    private var resolution: UInt16 = 1
    private var verticalRemainder = 0.0
    private var horizontalRemainder = 0.0

    init(options: Options) {
        self.options = options
        manager = IOHIDManagerCreate(kCFAllocatorDefault, IOOptionBits(kIOHIDOptionsTypeNone))
        reportBuffer = .allocate(capacity: BridgeProtocol.packetSize)
        reportBuffer.initialize(repeating: 0, count: BridgeProtocol.packetSize)
    }

    deinit {
        send(message: BridgeProtocol.hostGoodbye)
        heartbeat?.invalidate()
        if let device {
            IOHIDDeviceRegisterInputReportCallback(device, reportBuffer, BridgeProtocol.packetSize, nil, nil)
            IOHIDDeviceClose(device, IOOptionBits(kIOHIDOptionsTypeNone))
        }
        IOHIDManagerUnscheduleFromRunLoop(manager, CFRunLoopGetCurrent(), CFRunLoopMode.defaultMode.rawValue)
        IOHIDManagerClose(manager, IOOptionBits(kIOHIDOptionsTypeNone))
        reportBuffer.deallocate()
    }

    func start() {
        let matching: [String: Any] = [
            kIOHIDVendorIDKey: Self.vendorID,
            kIOHIDProductIDKey: Self.productID,
            kIOHIDPrimaryUsagePageKey: Self.usagePage,
            kIOHIDPrimaryUsageKey: Self.usage,
        ]
        IOHIDManagerSetDeviceMatching(manager, matching as CFDictionary)

        let context = Unmanaged.passUnretained(self).toOpaque()
        IOHIDManagerRegisterDeviceMatchingCallback(manager, Self.deviceMatched, context)
        IOHIDManagerRegisterDeviceRemovalCallback(manager, Self.deviceRemoved, context)
        IOHIDManagerScheduleWithRunLoop(manager, CFRunLoopGetCurrent(), CFRunLoopMode.defaultMode.rawValue)

        let result = IOHIDManagerOpen(manager, IOOptionBits(kIOHIDOptionsTypeNone))
        guard result == kIOReturnSuccess else {
            fatal("unable to open IOHIDManager (0x\(String(result, radix: 16)))")
        }

        heartbeat = Timer.scheduledTimer(withTimeInterval: 0.5, repeats: true) { [weak self] _ in
            self?.heartbeatTick()
        }

        print("Waiting for Keyball39 Raw HID interface (5957:0200)...")
    }

    private static let deviceMatched: IOHIDDeviceCallback = { context, _, _, device in
        guard let context else { return }
        Unmanaged<KeyballScrollBridge>.fromOpaque(context).takeUnretainedValue().connect(device)
    }

    private static let deviceRemoved: IOHIDDeviceCallback = { context, _, _, device in
        guard let context else { return }
        Unmanaged<KeyballScrollBridge>.fromOpaque(context).takeUnretainedValue().disconnect(device)
    }

    private static let inputReport: IOHIDReportCallback = { context, result, _, _, _, report, length in
        guard result == kIOReturnSuccess, let context else { return }
        let bridge = Unmanaged<KeyballScrollBridge>.fromOpaque(context).takeUnretainedValue()
        bridge.receive(Array(UnsafeBufferPointer(start: report, count: length)))
    }

    private func connect(_ newDevice: IOHIDDevice) {
        guard device == nil else { return }

        let result = IOHIDDeviceOpen(newDevice, IOOptionBits(kIOHIDOptionsTypeNone))
        guard result == kIOReturnSuccess else {
            log("Could not open Keyball Raw HID interface: 0x\(String(result, radix: 16))")
            return
        }

        device = newDevice
        let context = Unmanaged.passUnretained(self).toOpaque()
        IOHIDDeviceRegisterInputReportCallback(
            newDevice,
            reportBuffer,
            BridgeProtocol.packetSize,
            Self.inputReport,
            context
        )
        print("Keyball39 connected; requesting raw scroll stream")
        sendProbe()
    }

    private func disconnect(_ removedDevice: IOHIDDevice) {
        guard let device, device === removedDevice else { return }
        IOHIDDeviceRegisterInputReportCallback(device, reportBuffer, BridgeProtocol.packetSize, nil, nil)
        IOHIDDeviceClose(device, IOOptionBits(kIOHIDOptionsTypeNone))
        self.device = nil
        bridgeReady = false
        lastProbe = .distantPast
        resolution = 1
        verticalRemainder = 0
        horizontalRemainder = 0
        print("Keyball39 disconnected")
    }

    private func send(message: UInt8) {
        guard let device else { return }
        let packet = BridgeProtocol.packet(message: message)
        let result = IOHIDDeviceSetReport(
            device,
            kIOHIDReportTypeOutput,
            0,
            packet,
            packet.count
        )
        if result != kIOReturnSuccess {
            log("Raw HID write failed: 0x\(String(result, radix: 16))")
        }
    }

    private func heartbeatTick() {
        guard AXIsProcessTrusted() else {
            if bridgeReady {
                send(message: BridgeProtocol.hostGoodbye)
                bridgeReady = false
                print("Accessibility permission was removed; ordinary wheel fallback will resume")
            }
            return
        }

        if !bridgeReady {
            if Date().timeIntervalSince(lastProbe) >= 2 {
                sendProbe()
            }
            return
        }

        if Date().timeIntervalSince(lastAcknowledgement) >= 1.5 {
            bridgeReady = false
            print("Keyball scroll bridge stopped acknowledging; ordinary wheel fallback will resume")
            return
        }
        send(message: BridgeProtocol.hostHeartbeat)
    }

    private func sendProbe() {
        lastProbe = Date()
        send(message: BridgeProtocol.hostHello)
    }

    private func receive(_ report: [UInt8]) {
        guard BridgeProtocol.isValid(report) else {
            log("Ignored invalid Raw HID report (\(report.count) bytes)")
            return
        }

        switch report[BridgeProtocol.messageOffset] {
            case BridgeProtocol.deviceHello:
                let advertised = BridgeProtocol.uint16(report, at: BridgeProtocol.resolutionOffset)
                resolution = max(advertised, 1)
                lastAcknowledgement = Date()
                if !bridgeReady {
                    print("Bridge ready at 1/\(resolution) wheel-tick resolution")
                    send(message: BridgeProtocol.hostHeartbeat)
                }
                bridgeReady = true
            case BridgeProtocol.deviceScroll:
                let packetResolution = BridgeProtocol.uint16(report, at: BridgeProtocol.resolutionOffset)
                if packetResolution > 0 {
                    resolution = packetResolution
                }
                postScroll(
                    vertical: BridgeProtocol.int16(report, at: BridgeProtocol.verticalOffset),
                    horizontal: BridgeProtocol.int16(report, at: BridgeProtocol.horizontalOffset)
                )
            default:
                log("Ignored message 0x\(String(report[BridgeProtocol.messageOffset], radix: 16))")
        }
    }

    private func postScroll(vertical: Int16, horizontal: Int16) {
        let verticalPixels = pixels(
            delta: vertical,
            inverted: options.invertVertical,
            remainder: &verticalRemainder
        )
        let horizontalPixels = pixels(
            delta: horizontal,
            inverted: options.invertHorizontal,
            remainder: &horizontalRemainder
        )
        guard verticalPixels != 0 || horizontalPixels != 0 else { return }

        guard let event = CGEvent(
            scrollWheelEvent2Source: nil,
            units: .pixel,
            wheelCount: 2,
            wheel1: verticalPixels,
            wheel2: horizontalPixels,
            wheel3: 0
        ) else {
            log("Could not create CGEvent")
            return
        }
        event.setIntegerValueField(.scrollWheelEventIsContinuous, value: 1)
        event.post(tap: .cghidEventTap)
    }

    private func pixels(delta: Int16, inverted: Bool, remainder: inout Double) -> Int32 {
        let direction = inverted ? -1.0 : 1.0
        let scaled = Double(delta) * options.pixelsPerTick * direction / Double(resolution) + remainder
        let whole = scaled.rounded(.towardZero)
        remainder = scaled - whole
        let clamped = min(max(whole, Double(Int32.min)), Double(Int32.max))
        return Int32(clamped)
    }

    private func log(_ message: String) {
        if options.verbose {
            print(message)
        }
    }

    private func fatal(_ message: String) -> Never {
        FileHandle.standardError.write("keyball-scroll: \(message)\n".data(using: .utf8)!)
        Darwin.exit(1)
    }
}

private func requireAccessibilityPermission() {
    let key = kAXTrustedCheckOptionPrompt.takeUnretainedValue() as String
    let options = [key: true] as CFDictionary
    if !AXIsProcessTrustedWithOptions(options) {
        FileHandle.standardError.write(
            "Grant Accessibility permission in System Settings, then restart keyball-scroll.\n"
                .data(using: .utf8)!
        )
        Darwin.exit(1)
    }
}

let options = Options.parse()
requireAccessibilityPermission()
let instanceLock = SingleInstanceLock.acquire()
let bridge = KeyballScrollBridge(options: options)
bridge.start()
withExtendedLifetime(instanceLock) {
    RunLoop.current.run()
}
