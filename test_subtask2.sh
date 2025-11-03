#!/bin/bash

# Test Sub-task 2: Data Structure Benchmarking
# This script compiles and runs a comprehensive test of your benchmark_datastructure() function

set -e  # Exit on error

echo "╔════════════════════════════════════════════════════════════════╗"
echo "║         SUB-TASK 2: BUILDING AND TESTING                       ║"
echo "╚════════════════════════════════════════════════════════════════╝"
echo ""

# Step 1: Compile the test program
echo "[1/3] Compiling test program..."
g++ -std=c++20 -O3 -pthread \
    -I./include \
    test_subtask2.cpp src/Benchmarking.cpp \
    -o test_subtask2 \
    -lnuma

if [ $? -eq 0 ]; then
    echo "✓ Compilation successful!"
else
    echo "✗ Compilation failed!"
    exit 1
fi

echo ""

# Step 2: Run the test
echo "[2/3] Running benchmarks..."
echo "      (This may take a few minutes...)"
echo ""

# Use numactl if available, otherwise run directly
if command -v numactl &> /dev/null; then
    numactl --membind 0 --physcpubind 1 ./test_subtask2
else
    echo "⚠ Warning: numactl not found, running without NUMA binding"
    ./test_subtask2
fi

echo ""

# Step 3: Summary
echo "[3/3] Test complete!"
echo ""
echo "Review the results above. You should see:"
echo "  - Higher bandwidth for Sequential vs Random access"
echo "  - Lower latency for Sequential vs Random access"
echo "  - DirectAccessArray should be fastest for sequential"
echo "  - Larger datasets should have lower bandwidth and higher latency"
echo ""
echo "If results look good, proceed to running full experiments for the report!"
echo ""
