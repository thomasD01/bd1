#!/bin/bash
set -e

cd "$(dirname "$0")"
cd ..

FQBN="arduino:zephyr:unoq"
SKETCH_DIR="firmware"

echo "🔍 Checking ADB Bridge..."
ADB_DEVICE=$(adb devices | grep -w "device" | head -n 1 | awk '{print $1}')

if [ -z "$ADB_DEVICE" ]; then
    echo "❌ Error: ADB cannot see the device over the Windows bridge."
    echo "Run 'adb devices' manually to check the connection."
    exit 1
fi

echo "🎯 Found Uno Q via ADB: $ADB_DEVICE"
echo "🚀 Flashing Zephyr RTOS..."

arduino-cli upload --fqbn "$FQBN" -p "$ADB_DEVICE" --upload-property "upload.port.properties.serialNumber=$ADB_DEVICE" .

echo "✅ Upload complete!"