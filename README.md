# Keyball Series

![Keyball61](./keyball61/doc/rev1/images/kb61_001.jpg)

Keyball series is keyboard family which have 100% track ball.

Keyboards in the family are:

* Available
    * Keyball39: split + 39 keys + a track ball
    * Keyball44: split + 44 keys + a track ball
    * Keyball61: split + 61 keys + a track ball
* Unavailable
    * Keyball46 (first one!)
    * One47

## Where to Buy

|Keyboard   |Shirogane Lab / 白銀ラボ                                   |Yushakobo / 遊舎工房                       |
|-----------|-------------------------------------------|-----------------------------------------------------------|
|Keyball39  |<https://shiroganelab.com/products/keyball39> |<https://shop.yushakobo.jp/products/5357>  |
|Keyball44  |<https://shiroganelab.com/products/keyball44> |<https://shop.yushakobo.jp/products/8337>  |
|Keyball61  |<https://shiroganelab.com/products/keyball61> |<https://shop.yushakobo.jp/products/5358>  |

## Build Guide

*   Keyball39:
    [English/英語](/keyball39/doc/rev1/buildguide_en.md),
    [日本語/Japanese](./keyball39/doc/rev1/buildguide_jp.md)
*   Keyball44:
    [English/英語](./keyball44/doc/rev1/buildguide_en.md),
    [日本語/Japanese](./keyball44/doc/rev1/buildguide_jp.md)
*   Keyball61:
    [English/英語](./keyball61/doc/rev1/buildguide_en.md),
    [日本語/Japanese](./keyball61/doc/rev1/buildguide_jp.md)

## Firmware

See [document for firmware source code](./qmk_firmware/keyboards/keyball/readme.md).

### Pre-compiled Firmwares

(TO BE DOCUMENTED)


## 別のKeyball44をQMKへ移行する（Linux + Nix）

対象は**右ボール・Pro Micro / ATmega32U4・Caterinaブートローダー**のKeyball44。
このリポジトリの`kaorubb`配列を導入する手順で、Linux上でNixとGitを使う。
左ボールや別のマイコンを使う個体は、配線・レイアウト・書き込み方法を別途確認する。
バックアップは任意。ここではビルドから直接書き込む。

### 1. ソースを取得してビルドする

初めて取得する場合（取得済みなら、そのリポジトリで作業する）：

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
Keyball定義のリンクもスクリプトが作成するため、`qmk setup`は不要。
ビルドと配列表テストが成功したことを確認する。生成物は
`tmp/qmk-0.29.0/keyball_keyball44_kaorubb.hex`。
容量検査が失敗したビルドのHEXは使用しない。

### 2. 同じHEXを両側へ書き込む

作業対象のKeyball44だけをUSB接続する。スクリプトはUSB ID `5957:0400`を探す。
認識しない場合は、その個体のマイコンと現行ファームウェアを確認する。
左右をつなぐケーブルは接続したままでよい。抜き差しする場合は先にUSBを外す。

USBを左側へ接続し、以下を実行する：

```sh
bash bin/nix-develop.sh --command python3 bin/keyball44-avr.py flash left --sudo
```

sudo認証後、表示されたSHA256を記録する。
`Press the physical reset button twice now`と表示されたら左リセットを素早く2回押す。
`left flash write and verification completed.`を確認する。
USBを外して右側へ挿し直し、右側も実行する：

```sh
bash bin/nix-develop.sh --command python3 bin/keyball44-avr.py flash right --sudo
```

同じSHA256であることを確認し、リセット待ちの表示後に右リセットを2回押す。
`right flash write and verification completed.`を確認する。
途中でソースやHEXを変えた場合は、両側を同じビルドへ揃える。

### 3. 入力とマウスを確認する

- 起動時はLinux US。Muteを保持し、左Backspace位置でMac、左Space位置でLinuxを選ぶ。
  選択はEEPROMへ保存しない。USBを接続する側でOSを使い分ける必要はない。
- 左右の全キー、記号、修飾キーのタップ／ホールド、TTの数字レイヤー、OLED・LEDを確認する。
- ボール移動でマウスレイヤーへ入り、**M＝左クリック、カンマ＝右クリック、
  ピリオド保持＝スクロール**。ドラッグ中に1秒以上止めても保持され、キーを離すと
  クリックやスクロールが解除されることを確認する。
- 数字・設定レイヤー中は自動マウスが優先されないこと、マウス操作後に
  M・カンマ・ピリオドが通常入力へ戻ること、OS切替後にキーが保持されないことを確認する。
- 接続先のLinux/Mac実機で確認する。Windows JISプロファイルはまだ未実装。

元のQMK版が異なる場合、初回起動でEEPROM設定が再初期化されることがある。
CPIやスクロール、LEDを好みに合わせて調整する。
LEDのオン／オフはMute保持中のTab位置で切り替える。
詳細は[個人キーマップREADME](./qmk_firmware/keyboards/keyball/keyball44/keymaps/kaorubb/README.md)、
実測値と復旧情報は[移行記録](./keyball44/doc/qmk-migration.md)を参照する。

### 今後の編集

`qmk_firmware/keyboards/keyball/keyball44/keymaps/kaorubb/keymap.c`を編集し、
ビルド・配列表テスト・両側への書き込み・実機確認を繰り返す。
VIAは無効のため、Remapでの編集はできない。
実機確認済みHEXと、そのソースのGitコミット、SHA256をセットで保管する。
Step 5の確認済みHEXは`keyball44/doc/recovery/keyball44-kaorubb-step5.hex`。
配列ソースのコミットは`1e87e2a`、SHA256は
`ae7cd08903ae1957189da5a32a207bb2f09cbb0d03d4e5d373e0aeb6dbe4c78f`。

### 任意：元のRemap配列も保存したい場合

書き込み前に左右それぞれへUSBを接続し、次のコマンドと物理リセットで保存できる：

```sh
bash bin/nix-develop.sh --command python3 bin/keyball44-avr.py backup left --sudo
bash bin/nix-develop.sh --command python3 bin/keyball44-avr.py backup right --sudo
```

保存先は`keyball44/doc/recovery/<side>-avr-backup-<UTC日時>/`。
元へ戻す場合は、その個体から取得した左右それぞれの保存先を使う。
復旧手順は[移行記録](./keyball44/doc/qmk-migration.md)を参照する。
