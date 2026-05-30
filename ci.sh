#!/usr/bin/env bash
# ENIGMA local CI runner — same jobs as .github/workflows/ci.yml
set -euo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$ROOT"

ACT_EVENT="${ENIGMA_CI_ACT_EVENT:-pull_request}"

usage() {
  cat <<'EOF'
Usage: ./ci.sh <command> [options]

Run ENIGMA CI locally (build emake, emake-tests, empty game, coverage).

Commands:
  act <job>              Run a workflow job with nektos/act (Arch on any host with Docker)
  docker arch            Run Arch CI in Docker (uses enigma-ci-arch image if built)
  docker-image           Build enigma-ci-arch image (recommended once on Apple Silicon)
  arch                   Native Arch CI (pacman; run on Arch Linux)
  macos                  Native macOS CI (Homebrew; run on macOS)
  windows                Native Windows CI (MSYS2 MinGW64 shell)

  help, -h, --help       Show this message

act jobs (GitHub workflow job names):
  arch                   Arch Linux container job
  macos                  macOS job (act: Windows host only; on Linux use: ./ci.sh macos)
  windows                Windows MSYS2 job (act: Windows host only; on Linux use: ./ci.sh windows)
  coverage               Coverage merge job (needs prior job artifacts on GitHub)

Environment:
  ENIGMA_CI_ACT_EVENT    act event name (default: pull_request)
  COVERAGE               Set to 0 to disable coverage (default: 1 in ci-job.sh)
  OBJ_DIR                Object dir (default via ci-job: .eobjs-ci-arch|macos|windows)

Codecov upload runs on GitHub only (CODECOV_TOKEN repo secret). act skips that step.

Examples:
  ./ci.sh act arch
  ./ci.sh docker-image && ./ci.sh docker arch   # recommended on macOS (arm64)
  ./ci.sh macos
  ./ci.sh windows
EOF
}

ensure_scripts_executable() {
  chmod +x CI/github/*.sh 2>/dev/null || true
}

cmd_act() {
  local job="${1:?act job required (e.g. arch). Run: ./ci.sh help}"
  shift || true
  if ! command -v act >/dev/null 2>&1; then
    echo "act not found. Install: https://github.com/nektos/act" >&2
    exit 1
  fi
  exec act "$ACT_EVENT" -j "$job" "$@"
}

cmd_docker_arch() {
  ensure_scripts_executable
  mkdir -p CI/github/out

  # shellcheck source=CI/github/lib.sh
  source "$ROOT/CI/github/lib.sh"
  local platform image skip_install=0
  platform="$(docker_platform)"
  image="archlinux/archlinux:latest"

  if docker image inspect enigma-ci-arch >/dev/null 2>&1; then
    image="enigma-ci-arch"
    skip_install=1
    echo "Using pre-built image enigma-ci-arch ($platform)"
  else
    echo "Using $image ($platform); run ./ci.sh docker-image first to bake deps and skip large downloads"
  fi

  local -a run_env=(-e TEST_XML=/workspaces/enigma-dev/CI/github/out/emake-tests-arch.xml)
  if [[ "$skip_install" -eq 1 ]]; then
    run_env+=(-e ENIGMA_CI_SKIP_INSTALL=1)
  fi

  exec docker run --rm \
    --platform "$platform" \
    --security-opt seccomp=unconfined \
    -v "$ROOT:/workspaces/enigma-dev" \
    -w /workspaces/enigma-dev \
    "${run_env[@]}" \
    "$image" \
    bash -lc 'chmod +x CI/github/*.sh && CI/github/ci-job.sh arch'
}

cmd_docker_image() {
  # shellcheck source=CI/github/lib.sh
  source "$ROOT/CI/github/lib.sh"
  local platform
  platform="$(docker_platform)"
  echo "Building enigma-ci-arch ($platform) ..."
  docker pull --platform "$platform" archlinux/archlinux:latest

  if ! docker buildx version >/dev/null 2>&1; then
    echo "docker buildx is required" >&2
    exit 1
  fi

  docker buildx build \
    --pull \
    --platform "$platform" \
    --load \
    -f "$ROOT/CI/docker/Dockerfile.arch" \
    -t enigma-ci-arch \
    "$ROOT"
}

cmd_native() {
  local platform="${1:?platform required: arch|macos|windows}"
  ensure_scripts_executable
  exec ./CI/github/ci-job.sh "$platform"
}

main() {
  local cmd="${1:-help}"
  case "$cmd" in
    help|-h|--help) usage ;;
    act)
      shift
      cmd_act "$@"
      ;;
    docker)
      shift
      local sub="${1:-arch}"
      case "$sub" in
        arch) cmd_docker_arch ;;
        *) echo "Unknown docker target: $sub (try: docker arch)" >&2; exit 1 ;;
      esac
      ;;
    docker-image) cmd_docker_image ;;
    arch|macos|windows) cmd_native "$cmd" ;;
    *) echo "Unknown command: $cmd" >&2; echo >&2; usage >&2; exit 1 ;;
  esac
}

main "$@"
