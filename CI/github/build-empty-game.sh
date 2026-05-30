#!/usr/bin/env bash
set -euo pipefail
source "$(dirname "$0")/lib.sh"

: "${ENIGMA_PLATFORM:?ENIGMA_PLATFORM is required}"
: "${ENIGMA_OUTPUT:?ENIGMA_OUTPUT is required}"

# Graphics must match an existing SHELL bridge (e.g. xlib-OpenGL1), not xlib-None.
ENIGMA_GRAPHICS="${ENIGMA_GRAPHICS:-}"
if [[ -z "$ENIGMA_GRAPHICS" ]]; then
  case "$ENIGMA_PLATFORM" in
    xlib|Cocoa|SDL) ENIGMA_GRAPHICS=OpenGL1 ;;
    Win32) ENIGMA_GRAPHICS=Direct3D9 ;;
    *) ENIGMA_GRAPHICS=OpenGL1 ;;
  esac
fi

ENIGMA_AUDIO="${ENIGMA_AUDIO:-None}"
emake_args=(
  -m Compile -o "$ENIGMA_OUTPUT"
  -p "$ENIGMA_PLATFORM" -g "$ENIGMA_GRAPHICS" -a "$ENIGMA_AUDIO"
  -c None -n None -w None -e None
)
if [[ -n "${ENIGMA_COMPILER:-}" ]]; then
  emake_args+=(-x "$ENIGMA_COMPILER")
fi

echo "==> Building empty game (platform=$ENIGMA_PLATFORM, graphics=$ENIGMA_GRAPHICS, output=$ENIGMA_OUTPUT${ENIGMA_COMPILER:+, compiler=$ENIGMA_COMPILER})"
./emake "${emake_args[@]}"

if [[ -f "$ENIGMA_OUTPUT" ]]; then
  echo "Empty game artifact: $ENIGMA_OUTPUT"
else
  echo "Empty game build finished (output path: $ENIGMA_OUTPUT)" >&2
fi
