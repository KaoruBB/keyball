#!/usr/bin/env bash
set -euo pipefail

repo_dir=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")/.." && pwd)
shell_dir="$repo_dir/tmp/nix-shell"

# Keep QMK's large ignored checkout out of Nix's flake source snapshot.
mkdir -p "$shell_dir"
cp "$repo_dir/flake.nix" "$repo_dir/flake.lock" "$shell_dir/"
cd "$repo_dir"
exec nix develop "path:$shell_dir" --no-write-lock-file "$@"
