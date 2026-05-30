#!/usr/bin/env bash
# Usage: ci-setup.sh <arch|macos|windows>
# Install deps (unless skipped) and set platform env for later CI steps.
set -euo pipefail
source "$(dirname "$0")/lib.sh"

PLATFORM_ID="${1:?platform id required: arch|macos|windows}"

ci_export_env COVERAGE "${COVERAGE:-1}"
ci_export_env OBJ_DIR "${OBJ_DIR:-.eobjs-ci-${PLATFORM_ID}}"
ci_export_env COVERAGE_OUTPUT "${COVERAGE_OUTPUT:-coverage-${PLATFORM_ID}.info}"
ci_export_env TEST_XML "${TEST_XML:-${REPO_ROOT}/CI/github/out/emake-tests-${PLATFORM_ID}.xml}"

case "$PLATFORM_ID" in
  arch)
    if [[ -n "${GITHUB_ACTIONS:-}" ]]; then
      ci_export_env TEST_XML "${GITHUB_WORKSPACE}/CI/github/out/emake-tests-${PLATFORM_ID}.xml"
    fi
    if [[ -z "${ENIGMA_CI_SKIP_INSTALL:-}" ]]; then
      CI/github/install-arch.sh
    fi
    ci_export_env ENIGMA_PLATFORM "${ENIGMA_PLATFORM:-xlib}"
    ci_export_env ENIGMA_OUTPUT "${ENIGMA_OUTPUT:-/tmp/enigma-empty}"
    ;;
  macos)
    CI/github/install-macos.sh
    ci_export_env ENIGMA_PLATFORM "${ENIGMA_PLATFORM:-SDL}"
    ci_export_env ENIGMA_COMPILER "${ENIGMA_COMPILER:-clang}"
    ci_export_env ENIGMA_AUDIO "${ENIGMA_AUDIO:-None}"
    ci_export_env ENIGMA_OUTPUT "${ENIGMA_OUTPUT:-/tmp/enigma-empty}"
    ;;
  windows)
    if [[ -z "${ENIGMA_CI_SKIP_INSTALL:-}" ]]; then
      CI/github/install-msys2.sh
    fi
    ci_export_env ENIGMA_PLATFORM "${ENIGMA_PLATFORM:-Win32}"
    ci_export_env ENIGMA_OUTPUT "${ENIGMA_OUTPUT:-/tmp/enigma-empty.exe}"
    ;;
  *)
    echo "Unknown platform id: $PLATFORM_ID" >&2
    exit 1
    ;;
esac

echo "==> CI setup complete ($PLATFORM_ID)"
