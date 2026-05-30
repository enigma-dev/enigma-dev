#!/usr/bin/env bash
# Usage: ci-job.sh <arch|macos|windows>
# Run full CI locally (setup → build → tests → empty game → coverage).
set -euo pipefail
source "$(dirname "$0")/lib.sh"

PLATFORM_ID="${1:?platform id required: arch|macos|windows}"
# shellcheck source=CI/github/ci-setup.sh
source "$(dirname "$0")/ci-setup.sh" "$PLATFORM_ID"
CI/github/build-emake.sh
CI/github/run-emake-tests.sh "${TEST_XML}"
CI/github/build-empty-game.sh
CI/github/collect-coverage.sh "${COVERAGE_OUTPUT}" "$PLATFORM_ID"

echo "==> CI job complete ($PLATFORM_ID)"
