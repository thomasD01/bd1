#!/bin/bash

set -e

cd "$(dirname "$0")"
cd ../lib/kinematics/simulation

echo "🔍 Starting BD-1 Kinematics Simulation..."
echo "   -> This will run the Python simulation for the BD-1 robot's kinematics"

python3 visualize.py
