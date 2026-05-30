#!/usr/bin/env bash
# Install MSYS2 packages from CI/deps/msys2.packages.txt (run inside MSYS2 MinGW64 shell).
set -euo pipefail
source "$(dirname "$0")/lib.sh"

packages="$(read_packages "$REPO_ROOT/CI/deps/msys2.packages.txt" | tr '\n' ' ')"
if [[ -z "${packages// }" ]]; then
  echo "No packages listed in CI/deps/msys2.packages.txt" >&2
  exit 1
fi

pacman -Syu --noconfirm
# shellcheck disable=SC2086
pacman -S --noconfirm --needed $packages
