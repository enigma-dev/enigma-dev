#!/usr/bin/env bash
# Emit a markdown table for GITHUB_STEP_SUMMARY from coverage-*.info files in cwd.
set -euo pipefail

LCOV_IGNORE="inconsistent,deprecated,format,unsupported,unused"
if [[ "$(lcov --version 2>&1 | sed -n 's/.*version \([0-9]*\).*/\1/p' | head -1)" -lt 2 ]]; then
  LCOV_IGNORE=""
fi

lcov_field() {
  local field="$1" text="$2"
  echo "$text" | grep -E "^[[:space:]]*${field}" | head -1 | sed 's/^[[:space:]]*//' || true
}

lcov_summary() {
  local info="$1"
  # Arch CI may use lcov 2.4+ (FNL records); older distro lcov needs those lines stripped.
  local compat
  compat="$(mktemp)"
  grep -v '^FNL:' "$info" >"$compat"
  if [[ -n "$LCOV_IGNORE" ]]; then
    lcov --summary "$compat" --ignore-errors "$LCOV_IGNORE" 2>&1
  else
    lcov --summary "$compat" 2>&1
  fi
  rm -f "$compat"
}

echo "## Coverage summary"
echo ""
echo "| Platform | Lines | Functions |"
echo "|----------|-------|-----------|"

found=0
while IFS= read -r info; do
  [[ -z "$info" ]] && continue
  found=1
  platform="$(basename "$info")"
  platform="${platform#coverage-}"
  platform="${platform%.info}"
  if ! summary="$(lcov_summary "$info")"; then
    echo "lcov --summary failed for ${info}:" >&2
    echo "$summary" >&2
    echo "| ${platform} | (lcov error) | |"
    continue
  fi
  lines="$(lcov_field lines "$summary")"
  funcs="$(lcov_field functions "$summary")"
  echo "| ${platform} | ${lines:-n/a} | ${funcs:-n/a} |"
done < <(find . -maxdepth 3 -type f -name 'coverage-*.info' | sort)

if [[ "$found" -eq 0 ]]; then
  echo "| (none) | no coverage artifacts found | |"
  echo "" >&2
  echo "No coverage-*.info under $(pwd). Downloaded artifacts:" >&2
  find . -maxdepth 3 -type f \( -name '*.info' -o -name 'coverage-*' \) 2>/dev/null | head -20 >&2 || true
fi

echo ""
echo "Codecov upload (PR comments and checks) runs in a later workflow step when \`CODECOV_TOKEN\` is set as a repository **secret** and \`.info\` artifacts exist."
