#!/usr/bin/env bash
set -euo pipefail
source "$(dirname "$0")/lib.sh"

export COVERAGE="${COVERAGE:-1}"
jobs="$(nproc_safe)"

echo "==> Building emake (COVERAGE=$COVERAGE, OBJ_DIR=$OBJ_DIR, CXX=${CXX:-c++}, -j$jobs)"
make -j"$jobs" emake
