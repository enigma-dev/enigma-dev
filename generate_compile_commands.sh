#!/bin/bash
# Script to generate compile_commands.json for VSCode IntelliSense
# Requires 'bear' to be installed: brew install bear

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$SCRIPT_DIR"

if ! command -v bear &> /dev/null; then
    echo "Error: 'bear' is not installed."
    echo "Install it with: brew install bear"
    echo ""
    echo "Alternatively, you can use the c_cpp_properties.json file"
    echo "that has been configured for IntelliSense."
    exit 1
fi

echo "Generating compile_commands.json..."
echo "This will run 'make clean' and then 'make all' with bear..."
echo ""

# Clean first to ensure a fresh build
make clean > /dev/null 2>&1 || true

# Run make with bear to capture compilation commands
bear -- make all

if [ -f "compile_commands.json" ]; then
    echo ""
    echo "✓ Successfully generated compile_commands.json"
    echo "  VSCode should now use this for better IntelliSense"
    echo "  You may need to reload the window (Cmd+Shift+P -> 'Reload Window')"
else
    echo ""
    echo "✗ Failed to generate compile_commands.json"
    exit 1
fi
