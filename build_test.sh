#!/usr/bin/env bash
# build_test.sh – Build and run ArduinoMock tests using g++ directly.
# Useful when PlatformIO cannot reach its package registry (CI without internet,
# offline development, etc.).
#
# Requirements: g++ with C++17 support (gcc 7+ or clang 5+)
#
# Usage:
#   ./build_test.sh          # build and run tests
#   ./build_test.sh --build  # build only

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD_DIR="${TMPDIR:-/tmp}/ArduinoMock_build"

mkdir -p "$BUILD_DIR"

echo "==> Building ArduinoMock tests ..."
g++ -std=c++17 -Wall -Wextra \
    -I "$SCRIPT_DIR/src" \
    -I "$SCRIPT_DIR/tools/unity" \
    "$SCRIPT_DIR/src/Arduino.cpp" \
    "$SCRIPT_DIR/src/Print.cpp" \
    "$SCRIPT_DIR/src/Stream.cpp" \
    "$SCRIPT_DIR/src/WString.cpp" \
    "$SCRIPT_DIR/src/HardwareSerial.cpp" \
    "$SCRIPT_DIR/src/SPI.cpp" \
    "$SCRIPT_DIR/src/Wire.cpp" \
    "$SCRIPT_DIR/tools/unity/unity.c" \
    "$SCRIPT_DIR/test/test_main/main.cpp" \
    -o "$BUILD_DIR/test_main"

echo "==> Build successful: $BUILD_DIR/test_main"

if [[ "${1:-}" != "--build" ]]; then
    echo "==> Running tests ..."
    "$BUILD_DIR/test_main"
fi
