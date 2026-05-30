#!/usr/bin/env bash
# Usage: collect-coverage.sh <output.info> [platform: arch|macos|windows]
set -euo pipefail
source "$(dirname "$0")/lib.sh"

OUTPUT="${1:?output .info path required}"
PLATFORM="${2:-arch}"

if ! command -v lcov >/dev/null 2>&1; then
  echo "lcov not installed; skipping coverage (install lcov or use full arch.packages.txt)" >&2
  exit 0
fi

rm -f "$OUTPUT"

# GCC 12+ / gcov quirks; --ignore-errors needs lcov 2+ (MSYS2 often ships 1.x).
lcov_ignore="inconsistent,deprecated,format,unsupported,unused"
if [[ "$(lcov --version 2>&1 | sed -n 's/.*version \([0-9]*\).*/\1/p' | head -1)" -lt 2 ]]; then
  lcov_ignore=""
fi

lcov_capture() {
  local gcov_tool="${1:-}"
  local -a lcov_args=(
    --capture --directory . --output-file "$OUTPUT"
    --rc branch_coverage=0
    --rc geninfo_unexecuted_blocks=1
  )
  if [[ -n "$lcov_ignore" ]]; then
    lcov_args+=(--ignore-errors "$lcov_ignore")
  fi
  if [[ -n "$gcov_tool" ]]; then
    lcov --gcov-tool "$gcov_tool" "${lcov_args[@]}"
  else
    lcov "${lcov_args[@]}"
  fi
}

case "$PLATFORM" in
  arch|macos)
    lcov_capture
    ;;
  windows)
    if command -v x86_64-w64-mingw32-gcov >/dev/null 2>&1; then
      lcov_capture "x86_64-w64-mingw32-gcov"
    else
      lcov_capture
    fi
    ;;
  *)
    echo "Unknown platform: $PLATFORM" >&2
    exit 1
    ;;
esac

# Drop system and test-only sources from the report.
if [[ -n "$lcov_ignore" ]]; then
  lcov --remove "$OUTPUT" \
    '/usr/*' '*/usr/*' '*/mingw64/*' '*/emake-tests/*' '*/gtest/*' '*/gmock/*' \
    --output-file "$OUTPUT" --ignore-errors "$lcov_ignore" || true
else
  lcov --remove "$OUTPUT" \
    '/usr/*' '*/usr/*' '*/mingw64/*' '*/emake-tests/*' '*/gtest/*' '*/gmock/*' \
    --output-file "$OUTPUT" || true
fi

echo "Coverage written to $OUTPUT"
if [[ -n "$lcov_ignore" ]]; then
  lcov --summary "$OUTPUT" --ignore-errors "$lcov_ignore" || true
else
  lcov --summary "$OUTPUT" || true
fi
