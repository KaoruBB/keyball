#!/usr/bin/env bash
set -euo pipefail

repo_dir=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")/.." && pwd)
keymap=${1:-default}

bash "$repo_dir/bin/setup-qmk.sh"
cd "$repo_dir/tmp/qmk-0.29.0"
make -j4 SKIP_GIT=yes "keyball/keyball44:$keymap"

echo "Firmware: $PWD/keyball_keyball44_${keymap}.hex"
