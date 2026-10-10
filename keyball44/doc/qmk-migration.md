# Keyball44 QMK移行記録

## Step 1：バックアップと復旧準備

2026-10-10開始。Step 4で左右の元Flash・EEPROM・VIA配列を取得し、Step 1のバックアップ準備を完了した。
Step 2以降の実装や実機への書き込みは、各ステップの確認後に進める。

### 確認済み

- 利用者申告：右ボール、Pro Micro / ATmega32U4。
- LinuxはUS配列、MacもUS配列。今後Windows JISを追加する。
- 現行配列の参考資料はこの文書と同じディレクトリ内の
  `recovery/keymap_cheatsheet_keyball44.pdf`と
  `recovery/keymap_cheatsheet_keyball44_mac.pdf`。
  PDFは表示上の記録であり、復元用の完全なバックアップではない。
- 2026-10-10に保存先を確認。両PDFのSHA256は移動前と一致し、内容に変更はない。
- 両PDFに4レイヤーがある。Layer 2は現在使っていない。
- `Kb 0`は`KBC_RST`（Keyball設定リセット）、`Kb 1`は`KBC_SAVE`、
  `Kb 7`は`SCRL_MO`（押している間スクロール）。
  `KBC_RST`はブートローダーに入るキーではない。

### 復旧候補のファームウェア

- 公式カタログ：<https://remap-keys.app/catalog/tAJ9Htme4oNabUkx4832/firmware>
- 表示名：`Keyball44_via_v1.3.2`
- カタログ表示日：2024-03-26
- ローカル保存先：`recovery/keyball44-via-v1.3.2.hex`（この文書と同じディレクトリ内）
- SHA256：`864da42597166d9085da08081c13e6fdae1e9f1765b35e7b20dda7280dd09187`
- ダウンロードしたファイルのSHA256がカタログ表示と一致することを確認済み。
- これは公式の復旧候補。現在実機に入っているファームウェアと同一かは未確認。
  現行ファームウェアの読み出しや復旧の実機テストは行っていない。

### 実機で取得・確認する項目

| 項目 | 状態 |
|---|---|
| 現行ファームウェアの入手元・版 | 移行前の左右両側のアプリ領域が公式`Keyball44_via_v1.3.2`と完全一致。「Remap for QMK 0.18」の表示は実機の版を示す根拠にはしない |
| Linux用の復元可能な配列データ | PDFと右側のVIA読み取り結果`recovery/right-linux-via-readback.json`を保存済み。JSONはRemapのインポート形式ではなく、復元用の生キーコード・マクロデータ |
| Mac用の復元可能な配列データ | PDFと左側のVIA読み取り結果`recovery/left-mac-via-readback.json`を保存済み。JSONはRemapのインポート形式ではなく、復元用の生キーコード・マクロデータ |
| PDFと実機配列の一致、Mod-Tap / Layer-Tap / TTのキーコード | 全44物理キー×4レイヤー×2プロファイルを照合済み。Mac Layer 3のF11だけPDFと実機に差があり、利用者はPDFのCtrl/F11を選択 |
| CPI・スクロール除数・スナップ方向 | 移行前のEEPROMをAVRのビット配置と照合。左右ともCPIは既定500、スクロールは既定D4（1/8）、垂直スナップ |
| 自動マウスレイヤーの有効状態・タイムアウト | 移行前のEEPROMは左右とも無効、650ms |
| OLEDの表示・LEDの有効状態、効果、明るさ | 移行前のEEPROMではLEDは両側とも消灯、色相0・彩度255・明るさ150。OLEDは元ファームウェアの標準表示。移行後のOLEDとポインタ表示は実機確認済み |
| 現行のUSB接続側と配列 | 左側＝Mac用、右側＝Linux用。現在はLinux使用中なので右側へ接続（2026-10-10、利用者確認済み） |
| 左右の物理リセットボタンの位置 | 両側とも利用者確認済み。左側は物理リセット後のCaterina認識とバックアップ取得に成功 |

Codexの操作可能なブラウザでRemapを開いたが、`+ KEYBOARD`操作後も
実機の接続・配列表示には進まなかった。実機からの取得に成功した扱いにはしない。

Step 4で、LinuxのhidrawインターフェースからVIAの読み取りコマンドを使い、左右の
4レイヤー（各384 bytes）とマクロ領域（各599 bytes）を取得できた。
左右ともVIAプロトコル12、マクロ16枠に設定済み文字列はなかった。
VIAプロトコルの版だけからQMKやKeyballファームウェアの版は特定しない。

「通常USBを挿す側」はPCからのUSBケーブルを左半分・右半分のどちらに
接続しているかを指す。左右をつなぐTRS/TRRSケーブルとは別。
現行環境は左側にMac用配列、右側にLinux用配列を保存している。
復旧時はMac用PDFを左側、Linux用PDFを右側の配列復元に使う。
移行後は両側に同じ個人用hexを書き込み、USB接続側によらずキー操作でOS用配列を選択する。
個人用hexはUSB接続後にLinux用で起動するので、Mac使用時はMac用へ手動切替する。
「Remap for QMK 0.18」はカタログ上で旧QMK向けRemapへのリンクとして
表示されるため、それだけで実機のファームウェアがQMK 0.18とは判断しない。

普段Remapを使っているChrome / EdgeでKeyball44に接続し、変更前の配列を保存する。
復元可能なファイルのエクスポートが利用できればLinux用・Mac用を別名で保存する。
ファイル出力がない場合は、Remapの保存機能で両配列を保存し、再適用できることを確認する。
保存機能の実際の操作名・保存先・復元方法もこの記録へ追記する。
配列を切り替える前に、現在接続している側の配列を保存する。

設定値はOLEDやRemapの表示から記録する。表示されない値は推測で埋めず、未確認とする。
設定を調べる目的で`KBC_RST`やEEPROM初期化を実行しない。

### 復旧手順（書き込み前にバックアップを完成させる）

1. 別の入力手段（内蔵キーボードなど）と、保存済み配列・設定記録を用意する。
2. USBを外してから左右接続ケーブルを外す。
3. 片側をUSB接続し、物理リセットでCaterinaブートローダーに入る。
   必要に応じてリセットを素早く2回押す。ブートローダーの待機時間が短いため、
   書き込みツールと対象hexを先に準備する。
4. 確認した復旧用hexを書き込み、書き込みと検証が成功したことを確認する。
   同じhexをもう片側にも書き込む。書き込み中はUSBを抜かない。
5. USBを外した状態で左右接続ケーブルを戻し、通常使用する側をUSB接続する。
6. Remapで接続し、保存済み配列を復元する。USB接続側ごとに配列が保存されるため、
   両側をUSB接続して使う場合はそれぞれ復元・確認する。
7. 記録したCPI、スクロール、自動マウス、LED設定を戻し、必要な設定を保存する。
8. 44キー、レイヤー、ポインタ、クリック、スクロール、日本語入力を確認する。

参考：
[公式Keyball44組立ガイド](rev1/buildguide_jp.md#anchor9)、
[QMK Caterina書き込み手順](https://docs.qmk.fm/flashing#caterina)。

### Step 1の完了条件

復旧用hex、左右の元Flash・EEPROM・VIA配列、現行ファームウェア情報、USB接続側と
リセット位置を確認済み。保存済み設定と配列を元のアプリと一緒に戻せる状態になった。
EEPROMへ保存されていなかった一時設定については、移行前の値を確認できない。

## Step 2：Nix開発環境

利用者の了承により、Step 1の未確認事項を残してビルド環境の準備へ進む。
実機への書き込み前にバックアップ・設定・USB接続側・リセット位置を再確認する。

### 環境とコマンド

- Nixpkgsは`nixos-26.05`を基礎に`flake.lock`で固定する。
- 公式`qmk`パッケージ（AVRツールチェーン、Make、avrdude、Python依存を含む）と`git`を使用。
- QMK本体は0.29.0、コミット`75a037d2f0f9c54bad8e2f68ae82134559c92f22`に固定する。
- QMK本体はリポジトリ直下の`tmp/qmk-0.29.0/`へ取得する（Git管理対象外）。
  Keyball定義はこのforkへのシンボリックリンクなので、編集をそのままビルドに反映できる。
- 既存READMEの0.22.14とは異なり、既存`build-user.yml`と同じ0.29.0を使う。

リポジトリ直下で実行する：

```sh
# 既存defaultキーマップをビルド。初回はQMK本体とサブモジュールも取得する。
bash bin/nix-develop.sh --command bash bin/build-keyball44.sh default

# 個人用キーマップ追加後のビルド例
bash bin/nix-develop.sh --command bash bin/build-keyball44.sh kaorubb
```

`bin/nix-develop.sh`はflakeとlockだけを`tmp/nix-shell/`へコピーしてNixを起動する。
そのためflakeをGitに追加する前でも実行でき、巨大なQMK checkoutをNixストアにコピーしない。
対話的に開発シェルへ入る場合は`bash bin/nix-develop.sh`を実行する。
ビルド結果は`tmp/qmk-0.29.0/keyball_keyball44_default.hex`。
セットアップは既存のcheckoutを削除・リセットせず再利用し、期待したQMK版でない場合は停止する。
QMKのユーザー共通設定やシステム環境の変更、実機への書き込みは行わない。
公開ソースの取得時だけGitのグローバル設定を読み込まず、HTTPSをSSHへ変換する設定の
影響を避ける。通常のGit設定ファイルは変更しない。

### 検証結果

2026-10-10、x86_64 Linux上でStep 2完了。

- `flake.lock`生成成功。Nixpkgsコミットは`7c8764b7c7b09b34f632464276218ef9090eaa11`。
- QMK本体と全サブモジュールの取得成功。QMK本体は指定コミットと一致。
- QMK CLI 1.2.0、avr-gcc 15.3.0、avrdude 8.1を確認。
- 既存`default`のビルドと、`bin/nix-develop.sh`を使う再実行が成功。
- 生成hexのSHA256：`1ab249e660d083f7c9d9bbec6b652da62e7b5cb495a5b29b7e2b70c429996daf`。
- ファームウェア容量：27,752 / 28,672 bytes（96%、残り920 bytes）。
- 3本のスクリプトは`bash -n`で構文確認済み。
- ログはリポジトリ直下の`tmp/build_log/step2/`へ保存（Git管理対象外）。
- 実機への書き込みは行っていない。macOSやaarch64上での環境検証は未実施。

初回ビルドは旧LED数定義`RGBLED_NUM`のため停止した。
Keyball44の`config.h`を`RGBLIGHT_LED_COUNT`へ更新すると成功した。
LED数60は変更していない。Keyball39にも既に同じ更新が入っている。
この名称変更は[QMKの2024-02-25変更履歴](https://docs.qmk.fm/ChangeLog/20240225)に記載されている。

旧形式のキーボード定義（`keyboard.json`がない、レイアウトマクロがヘッダーにある）に対する
警告と容量警告は残っている。定義全体の移行は今回行わない。
個人用配列の追加時は容量を再確認し、不足する場合はLEDエフェクト削減案を提示する。

## Step 3：Linux・Mac配列のコード化

個人キーマップとPDFからの変更点は
[kaorubbキーマップのREADME](../../qmk_firmware/keyboards/keyball/keyball44/keymaps/kaorubb/README.md)を参照。

- Linuxは0〜3、Macは4〜7の4レイヤーずつ。Layer 2への入口は追加しない。
- VIA無効、レイヤー状態16bit、起動時Linux、OS選択は手動で保存しない。
- Layer 3の左Space位置＝Linux、左Backspace位置＝Mac。
  Windows選択キーはStep 6で配列と一緒に追加する。
- RemapのPDF生成コードを確認し、修飾付き入力とホールドを表示位置で区別した。
- 計画の訂正：QMK 0.29.0のMuteは`0xA8`で8bitに収まり、標準のLayer-Tapで扱える。
  特別なタップ処理を追加する必要はなかった。
- タップ／ホールドはQMK 0.29.0標準の200ms、TT固定切替は標準の5回タップ。
- 新しい専用マウスレイヤーとWindows配列はこのステップでは実装しない。

### 容量調整

初回は29,008 / 28,672 bytesで容量検査に失敗した（336 bytes超過）。
利用者の選択に従い、個人キーマップのSnake・Knight・Static Gradient・Alternatingを省いた。
既存defaultキーマップのLEDエフェクトは変更していない。

復旧用hexには配布元の改行コードも含めて保存するため、`.gitattributes`でバイナリ扱いを指定した。
復旧ファイルのSHA256は変更していない。

### 検証結果

2026-10-10、x86_64 Linux上でStep 3完了。

- `kaorubb`の最終ビルド成功。
- 容量：28,212 / 28,672 bytes（98%、残り460 bytes）。
- 生成hexのSHA256：`eba02b85d127010f75a598fd1cd1af30c7d67124b69e7ed26d105e33a73b2d79`。
- 生成HEX/ELFに対する配列表検証6件が成功。
  容量、16bitレイヤー状態、右ボールの配線配置、基本入力とMod-Tap、
  数字とマウスボタン、設定とOS選択、Layer 2の修飾付き入力を確認した。
- 最終ビルドログと検証ログは`tmp/build_log/step3/`へ保存（Git管理対象外）。
- 旧形式のキーボード定義に対する警告と、容量警告は残る。
- OS切替後の旧TTキー解放で別プロファイルのレイヤーが復活しないよう、
  レイヤー状態を選択中の4レイヤーに制限する。同じOSの再選択は状態を変更しない。
- 実機には書き込んでいない。タップ／ホールド、OS切替時の解放、自動マウス、
  OLEDとOS上の実入力はStep 4で検証する。

容量の余裕は小さいため、Windows配列と専用マウスレイヤーを追加するときは再測定する。

## Step 4：書き込み前の実機照合・バックアップ

実機のVIA読み取り結果を使い、Step 3のPDFだけでは分からなかったキーコードを補正した。

- Muteは`KC_KB_MUTE`（0x7F）、音量は`KC_KB_VOLUME_UP/DOWN`（0x80/0x81）。
  Step 3で使用したConsumerキー（0xA8〜0xAA）から実機と同じKeyboardキーに合わせた。
  不要になった`EXTRAKEY`も無効にした。
- Layer 2のJIS Hiraganaは`KC_LNG4`（0x93）。`KC_INT2`から補正した。
- MacのLayer 3のQ位置は実機では通常F11だったが、利用者の選択でPDFのCtrl/F11を採用。
- Layer 2の修飾キーにあった実機の0x10E3/0x10E5は、修飾ビットがなく、
  それぞれLeft GUI / Right Shiftと同じ入力になるため通常のキーコードに正規化した。
- 配列表検証を8件へ拡張し、左右のVIA読み取り結果の全物理キーと比較して成功。

ブートローダー認識、元Flash/EEPROMの読み取り、左右への新ファームウェア書き込みは完了。
Linuxでの基本入力・数字・クリック・スクロール・OS切替は実機確認済み。
Mac実機の入力・Command/Option・記号・ポインタ・クリック・スクロールも確認済み。
キー保持中の両方向のOS切替と同じOSの再選択も確認済み。2026-10-10、Step 4完了。

### Linuxでのバックアップ・書き込み操作

左側の物理リセット後、CaterinaをSparkFun Pro Micro 5V（1B4F:9205）として認識した。
シリアルポートへのユーザー権限がなく読み取りを開始できなかったため、
利用者のターミナルでsudo認証してからavrdudeを使う。
sudoパスワードはターミナルへ入力し、チャットには送らない。

現在USB接続している左側のバックアップ：

```sh
bash bin/nix-develop.sh --command python3 bin/keyball44-avr.py backup left --sudo
```

パスワード入力後、`Press the physical reset button twice now`と表示されたら、
USB接続している側の物理リセットボタンを素早く2回押す。
スクリプトはそのKeyball44と同じUSB経路のCaterinaポートを待ち受け、FlashとEEPROMを読み取る。
成功時に`Backup completed`と保存先を表示する。

左側を新しいファームウェアへ書き込むコマンド（バックアップ成功を確認してから実行）：

```sh
bash bin/nix-develop.sh --command python3 bin/keyball44-avr.py flash left --sudo
```

書き込み前に配列表テストを実行し、SHA256を表示する。再度リセットを待ち受け、
Flashだけを書き込んでavrdudeによる検証を行う。avrdudeからEEPROMやブートローダーへの書き込みは行わない。
新ファームウェアの初回起動では、QMKの設定バージョン差（旧0xFEE6、新0xFEE3）により
EEPROMの設定が再初期化される。動作確認時にCPI・スクロール・自動マウス・LEDを確認する。
旧設定は左右の`eeprom.bin`に保存してある。

左側が済んだらUSBを外して右側へ挿し直し、同じコマンドの`left`を`right`へ替えて
バックアップ、書き込みの順に実行する。左右接続ケーブルは抜き差しする必要はない。
そのケーブルを抜き差しする場合は、必ず先にUSBを外す。

バックアップは`recovery/<side>-avr-backup-<UTC日時>/`へ保存する。
`application.hex`は復旧用のアプリ領域だけを含む。`flash.bin`はブート領域も含む生読み取りなので、
復旧時の書き込みには使わない。`eeprom.bin`は元のファームウェアと組み合わせて復元する。

元のRemap環境へ戻す場合のコマンド（復旧が必要な場合だけ実行）：

```sh
# 左側を元のMac用配列へ戻す
bash bin/nix-develop.sh --command python3 bin/keyball44-avr.py restore left --sudo --backup keyball44/doc/recovery/left-avr-backup-20261010T062915Z
# 右側を元のLinux用配列へ戻す
bash bin/nix-develop.sh --command python3 bin/keyball44-avr.py restore right --sudo --backup keyball44/doc/recovery/right-avr-backup-20261010T064317Z
```

対象側のUSB接続と物理リセットは書き込み時と同じ手順。
元のアプリ領域と元のEEPROMをセットで復元し、Remapで作った配列と保存設定を戻す。
実行前に対象側とバックアップの対応、ファイルのSHA256を確認する。
この復旧コマンド自体は実機で実行していない。

### 書き込み前の検証結果

- 左右判定修正前のビルドは27,562 / 28,672 bytes（残り1,110 bytes）。
  初回hexのSHA256：`540ab19a5a23bb5d0530ece3c1c17048878f68adab3b0e686c85f9b764580f20`。
- 左右のVIA配列との比較を含む8件の配列表テストが成功。
- 復旧用HEXの生成はアプリ領域を往復比較し、ブートローダー領域を含まないことを検証した。
- 左側のFlash・EEPROMを`recovery/left-avr-backup-20261010T062915Z/`に保存。
  復旧用`application.hex`と各ファイルのサイズ・SHA256を記録した`manifest.json`も作成済み。
- 最初のFlash読み取りではavrdudeが末尾の0xFFを6 bytes省略し、32,762 bytesになった。
  アプリ領域28,672 bytesは完全であり、そのデータから復旧用HEXを作成した。
  今後の読み取りは`-A`で末尾の省略を無効にする。
- 右側のFlash・EEPROMを`recovery/right-avr-backup-20261010T064317Z/`に保存。
  各ファイルのサイズ・SHA256を確認済み。元のアプリ領域は左側と同じ公式VIA v1.3.2。
- 左側への新hexの書き込みと27,562 bytesの読み戻し検証が成功（利用者のターミナル出力で確認）。
  右側への書き込みと検証も成功（利用者確認済み）。
- 最初の実機入力では左右のキーとポインタ方向が反転した。
  QMK 0.29.0の左右判定の極性を合わせるため、個人用`config.h`に
  `SPLIT_HAND_MATRIX_GRID_LOW_IS_LEFT`を追加した。jiyanの39用と同じ修正が44にも必要だった。
  修正後の左右への再書き込みと読み戻し検証も成功（利用者確認済み）。
- 左右判定修正版の容量：27,566 / 28,672 bytes（残り1,106 bytes）。
  SHA256：`0434ddb5e0c2954a0c0456eff35339514c0d1d4f7ab233380de538164604b59c`。
- 修正版で左右の基本入力・ポインタ方向・OLED表示が正常になった。
  数字レイヤー、左右クリック、スクロール、Linux/Macモード切替も利用者が正常と確認。
- Mac実機でも英字・US記号・Command/Option・ポインタ・クリック・スクロールが正常（利用者確認済み）。
- TTとMuteを保持してLinuxからMacへ切り替え、離した後にMac L0の通常入力へ戻ることを確認。
- 逆方向のMacからLinuxへの切替と同じOSの再選択も、離した後にL0へ戻り通常入力が正常。
- 動作確認済みの修正版hexを`recovery/keyball44-kaorubb-step4.hex`に保管。
  Step 5以降のビルドで上書きされる`tmp/`の生成物とは別に、現在の状態を維持する。
- バックアップ・復旧ファイルの検証4件が成功。復旧コマンドの実機実行は行っていない。

### 次のステップ

利用者確認後、Step 5で数字レイヤーから独立した自動マウスレイヤーを追加する。
U＝左クリック、O＝右クリック、I保持＝スクロール。Windows配列はStep 6で追加する。
