#!/bin/bash
set -e

cd "$(dirname "$0")"
cd ..

FQBN="arduino:zephyr:unoq"
LIB_DIR="lib/kinematics"

echo "🔨 Compiling BD-1 firmware for STM32 MCU..."
arduino-cli compile --fqbn "$FQBN" --library "$LIB_DIR" .

echo "✅ Build successful!"
