#!/bin/bash
set -e

cd "$(dirname "$0")"
cd ..

FQBN="arduino:zephyr:unoq"

echo "🔨 Compiling BD-1 firmware for STM32 MCU..."
arduino-cli compile --fqbn "$FQBN" --library "lib/kinemnatics" .

echo "✅ Build successful!"
