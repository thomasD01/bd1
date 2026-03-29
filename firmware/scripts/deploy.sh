#!/bin/bash
set -e

cd "$(dirname "$0")"
cd ..

FQBN="arduino:zephyr:unoq"

echo "🌐 1/4 Configuring WSL-to-Windows ADB Bridge..."
WIN_IP=$(ip route show default | awk '{print $3}')
export ADB_SERVER_SOCKET="tcp:$WIN_IP:5037"
echo "   -> Bridge routed to $ADB_SERVER_SOCKET"

echo "🔨 2/4 Compiling BD-1 Firmware..."
arduino-cli compile --fqbn "$FQBN" --library lib/kinematics .

echo "🔍 3/4 Checking ADB Device Status..."
ADB_DEVICE=$(adb devices | grep -w "device" | head -n 1 | awk '{print $1}')

if [ -z "$ADB_DEVICE" ]; then
    echo "❌ Error: ADB cannot see the device."
    echo "Ensure the Windows ADB server is running via: adb.exe -a nodaemon server start"
    exit 1
fi
echo "   -> Found Uno Q via ADB: $ADB_DEVICE"

echo "🚀 4/4 Flashing Zephyr RTOS..."
arduino-cli upload --fqbn "$FQBN" -p "$ADB_DEVICE" --upload-property "upload.port.properties.serialNumber=$ADB_DEVICE" .

echo "✅ Deploy sequence complete!"
