#!/usr/bin/env bash
# Shared helpers for ENIGMA GitHub CI scripts.

set -euo pipefail

REPO_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)"
cd "$REPO_ROOT"

read_packages() {
  local file="$1"
  grep -v '^[[:space:]]*#' "$file" | grep -v '^[[:space:]]*$' || true
}

nproc_safe() {
  if command -v nproc >/dev/null 2>&1; then
    nproc
  else
    sysctl -n hw.ncpu 2>/dev/null || echo 4
  fi
}

is_act() {
  [[ -n "${ACT:-}" ]]
}

# Persist env for later GitHub Actions steps in the same job.
ci_export_env() {
  local name="$1" value="$2"
  export "${name}=${value}"
  if [[ -n "${GITHUB_ENV:-}" ]]; then
    printf '%s=%s\n' "$name" "$value" >>"$GITHUB_ENV"
  fi
}

# Docker platform for Arch images (native arm64 on Apple Silicon, amd64 elsewhere).
docker_platform() {
  case "$(uname -m)" in
    arm64|aarch64) echo linux/arm64 ;;
    *) echo linux/amd64 ;;
  esac
}

is_ephemeral_arch_container() {
  [[ -n "${GITHUB_ACTIONS:-}" ]] || [[ -n "${ACT:-}" ]] || [[ -f /.dockerenv ]]
}

# pacman 7 seccomp sandbox fails in nested Docker (act, job containers). errno 22.
disable_pacman_sandbox() {
  if [[ ! -f /etc/pacman.conf ]]; then
    return 0
  fi
  # Arch ships #DisableSandboxSyscalls commented; enable it (DisableSandbox is obsolete).
  if grep -qE '^[[:space:]]*#DisableSandboxSyscalls' /etc/pacman.conf; then
    sed -i 's/^[[:space:]]*#DisableSandboxSyscalls/DisableSandboxSyscalls/' /etc/pacman.conf
    echo "Enabled DisableSandboxSyscalls in pacman.conf"
  elif ! grep -qE '^[[:space:]]*DisableSandboxSyscalls' /etc/pacman.conf; then
    sed -i '/^\[options\]/a DisableSandboxSyscalls' /etc/pacman.conf
    echo "Added DisableSandboxSyscalls to pacman.conf"
  fi
}
