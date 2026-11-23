#!/usr/bin/env bash
# Usage: run-emake-tests.sh [path/to/emake-tests.xml]
set -euo pipefail
source "$(dirname "$0")/lib.sh"

export COVERAGE="${COVERAGE:-1}"
TEST_XML="${1:-${TEST_XML:-${RUNNER_TEMP:-/tmp}/emake-tests.xml}}"
mkdir -p "$(dirname "$TEST_XML")"
TEST_LOG="$(dirname "$TEST_XML")/emake-tests.log"

echo "==> Building and running emake-tests (COVERAGE=$COVERAGE)"
make emake-tests

set +e
./emake-tests --gtest_output="xml:${TEST_XML}" 2>&1 | tee "$TEST_LOG"
status=${PIPESTATUS[0]}
set -e

# Turn each gtest failure into a GitHub annotation, so the checks view shows
# the test name, location, and message instead of a bare exit code.
if [[ -n "${GITHUB_ACTIONS:-}" ]]; then
  awk '
    function flush() {
      if (file != "") {
        gsub(/%/, "%25", msg); gsub(/\r/, "", msg); gsub(/\n/, "%0A", msg)
        printf "::error file=%s,line=%s,title=%s::%s\n", file, line, test, msg
      }
      file = ""; msg = ""
    }
    /^\[ RUN      \] / { flush(); test = $4; next }
    /^\[  FAILED  \] / { flush(); next }
    /^\[       OK \] / { flush(); next }
    match($0, /^[^ ]*emake-tests\/[^:]+:[0-9]+: Failure/) {
      flush()
      loc = substr($0, 1, RLENGTH - length(": Failure"))
      sub(/^.*emake-tests\//, "CommandLine/emake-tests/", loc)
      n = split(loc, parts, ":")
      line = parts[n]
      file = substr(loc, 1, length(loc) - length(line) - 1)
      next
    }
    file != "" && NF { msg = (msg == "" ? $0 : msg "\n" $0) }
    END { flush() }
  ' "$TEST_LOG"
fi

if [[ ! -f "$TEST_XML" ]]; then
  echo "JUnit XML was not created: $TEST_XML" >&2
  exit 1
fi
echo "JUnit XML: $TEST_XML ($(wc -c <"$TEST_XML") bytes)"
exit "$status"
