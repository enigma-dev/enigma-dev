#!/usr/bin/env bash
set -euo pipefail
source "$(dirname "$0")/lib.sh"

if [[ "$(uname -s)" != "Darwin" ]]; then
  echo "install-macos.sh: expected macOS" >&2
  exit 1
fi

packages="$(read_packages "$REPO_ROOT/CI/deps/brew.packages.txt" | tr '\n' ' ')"
if [[ -z "${packages// }" ]]; then
  echo "No packages listed in CI/deps/brew.packages.txt" >&2
  exit 1
fi

brew update
# shellcheck disable=SC2086
brew install $packages || brew upgrade $packages
