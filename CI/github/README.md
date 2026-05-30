# Local CI (`./ci.sh`)

Run the same checks as [`.github/workflows/ci.yml`](../../.github/workflows/ci.yml) from the repo root: build `emake`, `emake-tests`, empty game, coverage.

```bash
./ci.sh help
```

## Commands

| Command | When to use |
|---------|-------------|
| `./ci.sh arch` | On Arch Linux (native) |
| `./ci.sh docker arch` | Arch CI in Docker (any host with Docker) |
| `./ci.sh docker-image` | One-time: bake deps into `enigma-ci-arch` (faster repeat runs; good on Apple Silicon) |
| `./ci.sh macos` | On macOS (Homebrew deps; needs `llvm` — see `CI/deps/brew.packages.txt`) |
| `./ci.sh windows` | In MSYS2 **MinGW64** shell on Windows |
| `./ci.sh act arch` | Simulate the Arch GitHub job with [act](https://github.com/nektos/act) + Docker |

`act` cannot run `macos` jobs on Linux — use `./ci.sh macos` on a Mac instead.

## Examples

```bash
./ci.sh docker-image && ./ci.sh docker arch
./ci.sh macos
./ci.sh windows
./ci.sh act arch
```

Codecov upload only runs on GitHub (`CODECOV_TOKEN` secret). Local runs skip it.
