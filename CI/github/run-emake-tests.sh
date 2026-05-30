#!/usr/bin/env bash
# Usage: run-emake-tests.sh [path/to/emake-tests.xml]
set -euo pipefail
source "$(dirname "$0")/lib.sh"

export COVERAGE="${COVERAGE:-1}"
TEST_XML="${1:-${TEST_XML:-${RUNNER_TEMP:-/tmp}/emake-tests.xml}}"
mkdir -p "$(dirname "$TEST_XML")"

echo "==> Building and running emake-tests (COVERAGE=$COVERAGE)"
make emake-tests
./emake-tests --gtest_output="xml:${TEST_XML}"
if [[ ! -f "$TEST_XML" ]]; then
  echo "JUnit XML was not created: $TEST_XML" >&2
  exit 1
fi
echo "JUnit XML: $TEST_XML ($(wc -c <"$TEST_XML") bytes)"
