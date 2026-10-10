# kaorubb: Linux US / Mac US

Keyball44の右ボール用、VIA無効の個人キーマップ。
QMK 0.29.0の左右判定は`SPLIT_HAND_MATRIX_GRID_LOW_IS_LEFT`で基板の極性に合わせる。
配列の正本は`keymap.c`。Linuxの4レイヤーを0〜3、Macを4〜7へ配置する。
専用マウスレイヤーは12。Windows用（8〜11）はStep 6で追加する。

元の配列：
[Linux PDF](../../../../../../keyball44/doc/recovery/keymap_cheatsheet_keyball44.pdf)、
[Mac PDF](../../../../../../keyball44/doc/recovery/keymap_cheatsheet_keyball44_mac.pdf)。

## 操作

- 起動時はLinux US。OS選択はEEPROMに保存しない。
- 左親指のMuteをホールドすると、各プロファイルのLayer 3に入る。
  Muteはタップで送信する（`LT(..., KC_KB_MUTE)`）。
- Layer 3で左Space位置を押すとLinux、左Backspace位置を押すとMac。
  別OSへの切替時は一時レイヤーを解除する。同じOSを再選択した場合は状態を維持する。
  保持していたキーは離してから入力を再開する。
- 右Shift位置でのWindows選択は、Windows配列を追加するStep 6で実装する。
  現在はPDFと同じ透明キー。
- `TT(1)`はLinuxではLayer 1、Macでは内部Layer 5。
  QMK 0.29.0の標準設定で、ホールド中は有効、5回タップで固定オン・オフ。
- タップ／ホールド判定は標準の200ms。個別のタイミング調整はまだ行わない。
- Layer 2のショートカット配列はコードに保存するが、切替キーは追加しない。
- ボールを動かすと共通の自動マウスレイヤーへ入る。M＝左クリック、カンマ＝右クリック、
  ピリオドを押している間＝スクロール。それ以外のキーは下の配列を使う。
  OLEDに`Linux US Mouse`または`Mac US Mouse`を表示する。
  起動時・OS切替後に自動マウスを有効にし、タイムアウトは既存のKeyball設定に従う。
  数字・設定レイヤーの使用中は自動マウスを停止し、戻ると再開する。
  クリックやピリオドを保持している間は、ボールを止めてもマウスレイヤーを維持する。
- Layer 3を保持している間のスクロールと、Layer 1のZ位置の`SCRL_MO`を維持する。
  左Zと右ピリオドを同時に保持した場合は、最後のスクロールキーを離すまでスクロールを続ける。

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
| Muteと音量 | 実機のVIA読み取り結果に合わせてKeyboard Mute / Volumeを使用。Consumerキー用の`EXTRAKEY`は無効 |
| Layer 2のJIS Hiragana | 実機で`KC_LNG4`と確認。International 2ではなくLanguage 4を使用 |
| Mac Layer 3のQ位置 | 実機の通常F11から、利用者の選択でPDFのCtrl/F11へ変更 |
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
Step 4での実機確認済みHEXは`keyball44/doc/recovery/keyball44-kaorubb-step4.hex`。
Step 5では両側への書き込み後、M/カンマのクリック、ドラッグ中の停止、ピリオドの解放、
数字・設定レイヤーの優先、OS切替後のキー解放を実機で確認する。

## 別のKeyball44をQMKへ移行する（Linux + Nix）

この手順は、**右ボール・Pro Micro / ATmega32U4・Caterinaブートローダー**の
Keyball44へ、このリポジトリの`kaorubb`配列を導入するためのもの。
他の個体のRemap配列を自動的にソースへ変換する手順ではない。
左ボールや別のマイコンを使う個体は、配線・レイアウト・書き込み方法を別途確認する。
書き込み作業はLinuxで行い、NixとGitを用意しておく。

### 1. 今の配列・設定を保存する

Remapで左右それぞれの配列を確認し、PDFや取得できる配列データを保存する。
CPI、スクロール速度・方向、OLED・LED、自動マウスの設定も記録する。
左右で別の配列を使っている場合は、USBをそれぞれに接続して確認する。
PDFだけではタップ／ホールドの設定やEEPROMを完全には復元できないため、
下記の実機バックアップも必ず行う。

### 2. ソースを取得してビルドする

初めて取得する場合（既にこのブランチを取得済みなら、そのリポジトリで作業する）：

```sh
git clone --branch kaoru/qmk_migration https://github.com/KaoruBB/keyball.git
cd keyball
```

リポジトリ直下から実行する：

```sh
bash bin/nix-develop.sh --command bash bin/build-keyball44.sh kaorubb
bash bin/nix-develop.sh --command python3 qmk_firmware/keyboards/keyball/keyball44/keymaps/kaorubb/verify_firmware.py -v
```

初回はNixの依存とQMK本体・サブモジュールをダウンロードする。
`flake.lock`の環境とQMK **0.29.0**を使う（元のファームウェアREADMEの0.22.14とは異なる）。
Keyball定義のリンクもスクリプトが作成するので、`qmk setup`は不要。
ビルドと配列表テストが成功したことを確認する。生成物は
`tmp/qmk-0.29.0/keyball_keyball44_kaorubb.hex`。
容量検査が失敗したビルドのHEXは使用しない。

### 3. 移行対象の左右をバックアップする

作業対象のKeyball44だけをUSB接続する。スクリプトはUSB ID `5957:0400`を探す。
認識しない場合は、その個体のマイコンと現行ファームウェアを確認する。
左右をつなぐケーブルは接続したままでよい。抜き差しする場合は先にUSBを外す。

まずUSBを右側へ接続し、以下を実行する：

```sh
bash bin/nix-develop.sh --command python3 bin/keyball44-avr.py backup right --sudo
```

sudo認証後、`Press the physical reset button twice now`と表示されたら、
USB接続している側の物理リセットを素早く2回押す。
`Backup completed`と保存先が表示されるまで待つ。
次にUSBを外して左側へ挿し直し、同じ操作を行う：

```sh
bash bin/nix-develop.sh --command python3 bin/keyball44-avr.py backup left --sudo
```

バックアップは`keyball44/doc/recovery/<side>-avr-backup-<UTC日時>/`へ保存される。
`flash.bin`、`eeprom.bin`、`application.hex`、`manifest.json`が生成される。
**今回の個体名と左右それぞれの保存先をメモし、別の場所にもコピーする。**
リポジトリに同梱されたバックアップは最初の個体のものであり、別個体の復旧には使わない。
書き込みスクリプトの確認は「対象側のバックアップが存在すること」までで、
別個体のバックアップとの区別はできない。毎回、その個体の両側を新しく保存する。

### 4. 同じHEXを両側へ書き込む

両側のバックアップ成功後、現在USBを接続している左側から実行する：

```sh
bash bin/nix-develop.sh --command python3 bin/keyball44-avr.py flash left --sudo
```

表示されたSHA256を記録し、リセット待ちの表示後に左リセットを2回押す。
`left flash write and verification completed.`を確認する。
USBを外して右側へ挿し直し、右側も実行する：

```sh
bash bin/nix-develop.sh --command python3 bin/keyball44-avr.py flash right --sudo
```

同じSHA256であることを確認し、右リセットを2回押す。
`right flash write and verification completed.`を確認する。
途中でソースやHEXを変えた場合は、両側を同じビルドへ揃える。

### 5. 入力とマウスを確認する

- 起動時はLinux US。Muteを保持し、左Backspace位置でMac、左Space位置でLinuxを選ぶ。
  選択はEEPROMへ保存しない。USBを接続する側でOSを使い分ける必要はない。
- 左右の全キー、記号、修飾キーのタップ／ホールド、TTの数字レイヤー、OLED・LEDを確認する。
- ボール移動でマウスレイヤーへ入り、**M＝左クリック、カンマ＝右クリック、
  ピリオド保持＝スクロール**。Mを保持したドラッグ中に1秒以上止めても保持され、
  キーを離すとクリックやスクロールが解除されることを確認する。
- 数字・設定レイヤー中は自動マウスが優先されないこと、マウス操作後に
  M・カンマ・ピリオドが通常入力へ戻ること、OS切替後にキーが保持されないことを確認する。
- 接続先のLinux/Mac実機で確認する。Windows JISプロファイルはまだ未実装。

元のQMK版が異なる場合、初回起動でEEPROM設定が再初期化されることがある。
CPIやスクロール、LEDは保存した記録を基に調整する。
LEDのオン／オフはMute保持中のTab位置で切り替える。
設定・配列の詳細は[個人キーマップREADME](./qmk_firmware/keyboards/keyball/keyball44/keymaps/kaorubb/README.md)、
移行時の実測値と復旧情報は[移行記録](./keyball44/doc/qmk-migration.md)を参照する。

### 6. 復旧する場合と、今後の編集

元へ戻す場合は、手順3で**その個体から取得した**左右の保存先を指定する。
次の`<...>`を実際のパスへ置き換え、対象側へUSB接続して実行する：

```sh
bash bin/nix-develop.sh --command python3 bin/keyball44-avr.py restore left --sudo --backup <今回の左側バックアップディレクトリ>
bash bin/nix-develop.sh --command python3 bin/keyball44-avr.py restore right --sudo --backup <今回の右側バックアップディレクトリ>
```

リセット操作は書き込み時と同じ。`application.hex`と`eeprom.bin`をセットで復元する。
ブートローダーを含む`flash.bin`を直接書き込まない。
復旧ファイルの生成・検証はテスト済みだが、復旧コマンドの実機実行は未検証。

今後の配列変更は`qmk_firmware/keyboards/keyball/keyball44/keymaps/kaorubb/keymap.c`を編集し、
ビルド・配列表テスト・両側への書き込み・実機確認を繰り返す。VIAは無効のため、Remapでの編集はできない。
実機確認済みHEXと、そのソースのGitコミット、SHA256をセットで保管する。
今回のStep 5で確認済みのHEXは`keyball44/doc/recovery/keyball44-kaorubb-step5.hex`。
対応するコミットは`git log -1 -- keyball44/doc/recovery/keyball44-kaorubb-step5.hex`で確認できる。
