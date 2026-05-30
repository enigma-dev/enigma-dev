#!/usr/bin/env bash
set -euo pipefail
source "$(dirname "$0")/lib.sh"

if [[ "$(uname -s)" != "Linux" ]]; then
  echo "install-arch.sh: expected Linux" >&2
  exit 1
fi

packages="$(read_packages "$REPO_ROOT/CI/deps/arch.packages.txt" | tr '\n' ' ')"
if [[ -z "${packages// }" ]]; then
  echo "No packages listed in CI/deps/arch.packages.txt" >&2
  exit 1
fi

if command -v pacman >/dev/null 2>&1; then
  disable_pacman_sandbox
  export PACMAN_OPTS="${PACMAN_OPTS:---noconfirm --needed}"

  # CI/Docker: sync DB only. Full -Syu pulls huge upgrades and is slow on emulated amd64.
  if is_ephemeral_arch_container; then
    pacman -Sy --noconfirm
  else
    pacman -Syu --noconfirm
  fi
  # shellcheck disable=SC2086
  pacman -S --noconfirm --needed $packages
else
  echo "pacman not found; use the Arch CI Docker image" >&2
  exit 1
fi
