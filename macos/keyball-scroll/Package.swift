// swift-tools-version: 5.9

import PackageDescription

let package = Package(
    name: "KeyballScroll",
    platforms: [.macOS(.v13)],
    products: [
        .executable(name: "keyball-scroll", targets: ["KeyballScroll"]),
    ],
    targets: [
        .executableTarget(name: "KeyballScroll"),
    ]
)
