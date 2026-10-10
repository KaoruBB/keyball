#!/usr/bin/env bash
set -euo pipefail

repo_dir=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")/.." && pwd)
qmk_dir="$repo_dir/tmp/qmk-0.29.0"
qmk_revision=75a037d2f0f9c54bad8e2f68ae82134559c92f22

if [[ ! -d "$qmk_dir" ]]; then
    mkdir -p "$repo_dir/tmp"
    # Public sources need no credentials; avoid user URL rewrites to SSH.
    GIT_CONFIG_GLOBAL=/dev/null git clone --depth 1 --branch 0.29.0 https://github.com/qmk/qmk_firmware.git "$qmk_dir"
fi

if [[ $(git -C "$qmk_dir" rev-parse HEAD) != "$qmk_revision" ]]; then
    echo "Expected QMK 0.29.0 at $qmk_dir. Keep existing work and use the pinned revision." >&2
    exit 1
fi

GIT_CONFIG_GLOBAL=/dev/null git -C "$qmk_dir" submodule update --init --recursive --depth 1

keyball_source="$repo_dir/qmk_firmware/keyboards/keyball"
keyball_link="$qmk_dir/keyboards/keyball"
if [[ -L "$keyball_link" && $(readlink "$keyball_link") == "$keyball_source" ]]; then
    :
elif [[ -e "$keyball_link" || -L "$keyball_link" ]]; then
    echo "Cannot install Keyball definitions: $keyball_link already exists." >&2
    exit 1
else
    ln -s "$keyball_source" "$keyball_link"
fi

echo "QMK 0.29.0 ready at $qmk_dir"
