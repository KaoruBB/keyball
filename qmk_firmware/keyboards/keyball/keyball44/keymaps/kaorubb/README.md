# kaorubb: Linux US / Mac US

Keyball44の右ボール用、VIA無効の個人キーマップ。
配列の正本は`keymap.c`。Linuxの4レイヤーを0〜3、Macを4〜7へ配置する。
Windows用（8〜11）と専用マウスレイヤー（12）は後続ステップで追加する。

元の配列：
[Linux PDF](../../../../../../keyball44/doc/recovery/keymap_cheatsheet_keyball44.pdf)、
[Mac PDF](../../../../../../keyball44/doc/recovery/keymap_cheatsheet_keyball44_mac.pdf)。

## 操作

- 起動時はLinux US。OS選択はEEPROMに保存しない。
- 左親指のMuteをホールドすると、各プロファイルのLayer 3に入る。
  Muteはタップで送信する（`LT(..., KC_MUTE)`）。
- Layer 3で左Space位置を押すとLinux、左Backspace位置を押すとMac。
  別OSへの切替時は一時レイヤーを解除する。同じOSを再選択した場合は状態を維持する。
  保持していたキーは離してから入力を再開する。
- 右Shift位置でのWindows選択は、Windows配列を追加するStep 6で実装する。
  現在はPDFと同じ透明キー。
- `TT(1)`はLinuxではLayer 1、Macでは内部Layer 5。
  QMK 0.29.0の標準設定で、ホールド中は有効、5回タップで固定オン・オフ。
- タップ／ホールド判定は標準の200ms。個別のタイミング調整はまだ行わない。
- Layer 2のショートカット配列はコードに保存するが、切替キーは追加しない。
- 既存の自動マウス機能はLinuxではLayer 1、Macでは内部Layer 5を使う。
  有効状態やタイムアウトは既存のKeyball設定に従う。
- Layer 3を保持している間のスクロールと、Layer 1のZ位置の`SCRL_MO`を維持する。
  右手だけのマウス操作はStep 5で追加する。

## PDFとの対応・変更点

RemapのPDFでは、キー上部の修飾表示は同時入力、下部はホールド動作。
`*`の左右位置は左／右修飾キーを示す。
たとえばLayer 1のH位置はShift+左クリックという同時入力であり、Mod-Tapではない。
Layer 2のCtrl+数字も同時入力として保存する。

| 項目 | 実装 |
|---|---|
| LinuxのS/Lホールド | 左／右GUI（Super） |
| MacのS/Lホールド | 左／右Alt |
| LinuxのSpace | タップSpace、ホールド左Alt |
| MacのSpace | 通常のSpace |
| Macの左下のF3 | タップF3、ホールド左Ctrl |
| Mac Layer 1のD/F | タップ8/9、ホールド左Shift/GUI |
| Linux Layer 1のN/M/カンマ位置 | GUI+X / Page Down / Page Up |
| Mac Layer 1のN/M/カンマ位置 | N / M / カンマ |
| Layer 3のI位置 | LinuxはF12、MacはF8（PDFの違いをそのまま保存） |
| Linux Layer 0左下の透明キー | 下位レイヤーがないので`KC_NO`（入力なし） |
| Layer 3のSpace/Backspace位置 | PDFの透明キーからLinux/Mac選択キーへ変更 |
| Muteと音量 | `EXTRAKEY_ENABLE = yes`でメディアキーを有効化 |
| OLED | キー位置の表示行をOS名とプロファイル内のレイヤー番号へ変更。ボール・CPI・スクロール・自動マウス表示は維持 |
| LED | 利用者の選択によりSnake・Knight・Static Gradient・Alternatingを無効化 |

LEDのPlain・Breathe・Rainbow・Twinkle・RGB Test・Xmasを維持する。
既存のRainbow Swirlも残っており、エフェクト切替の循環に含まれる。
PDFのDebug Toggleキーは保存するが、Consoleは無効なのでデバッグ出力はない。

PDFからは現行ファームウェアのタップ判定時間、TTのタップ回数、CPI、スクロール設定、
自動マウス設定は分からない。上記の標準値と実機の現在の動作との違いは、書き込み前後に確認する。

## ビルドと検証

リポジトリ直下で実行する：

```sh
bash bin/nix-develop.sh --command bash bin/build-keyball44.sh kaorubb
bash bin/nix-develop.sh --command python3 qmk_firmware/keyboards/keyball/keyball44/keymaps/kaorubb/verify_firmware.py -v
```

検証スクリプトは生成されたHEXとELFを読み、容量、16bitレイヤー状態、
右ボールの配線に対応した配列表、主要なキーとLinux/Mac差分を確認する。
ホールドの判定、OS切替時のキー解放、自動マウス、OLED、実際の入力はStep 4の実機検証対象。

OS切替の実機確認では、通常の切替に加え、TTを保持したまま別OSへ切り替えてから
TTとMuteを離す操作、および同じOSを再選択する操作を試す。
旧プロファイルのレイヤーが復活しないこと、修飾キーやマウスボタンが保持されないことを確認する。

生成物：`tmp/qmk-0.29.0/keyball_keyball44_kaorubb.hex`。
サイズ検査に失敗したビルドでもhexが残るため、必ずビルドコマンドが成功した生成物を使う。
このStep 3では書き込みを行わない。
