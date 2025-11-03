# Sub-Task 2 Testing Guide

This guide will help you test your `benchmark_datastructure()` implementation comprehensively.

## Quick Start

### Step 1: Build the Main Tests (Basic Tests)

```bash
cd /home/u3076594/fomo_ws2025_lab1/build

# Clean previous build
make clean

# Rebuild with fixes
cmake ..
make benchmark_test_basic_local

# Run basic tests (Sub-task 1)
numactl --membind 0 --physcpubind 1 ./benchmark_test_basic_local
```

**Expected Output:**

```
===============================================================================
All tests passed (2 assertions in 2 test cases)
```

---

### Step 2: Test Sub-Task 2 Locally

I've created a comprehensive test program for you. Run:

```bash
cd /home/u3076594/fomo_ws2025_lab1

# Make the script executable
chmod +x test_subtask2.sh

# Run the test
./test_subtask2.sh
```

This will:
1. ✓ Compile `test_subtask2.cpp`
2. ✓ Run benchmarks on 4 different dataset sizes
3. ✓ Test both Sequential and Random access patterns
4. ✓ Show bandwidth and latency for all 4 data structures
5. ✓ Perform sanity checks on your results

---

### Step 3: Collect Full Data for Report

Once basic testing passes, collect comprehensive data:

```bash
# Compile the data collection program
g++ -std=c++20 -O3 -pthread \
    -I./include \
    collect_data.cpp src/Benchmarking.cpp \
    -o collect_data \
    -lnuma

# Run it (this takes ~5-10 minutes)
numactl --membind 0 --physcpubind 1 ./collect_data
```

This creates `benchmark_results.csv` with all your experiment data.

---

### Step 4: Create Plots

```bash
# Install matplotlib if needed
pip3 install matplotlib pandas numpy

# Run the plotting script
python3 plot_results.py
```

This creates:
- `benchmark_plots.pdf` - High quality plots for your report
- `benchmark_plots.png` - Preview image

---

## What the Tests Check

### Sub-Task 2 Test Coverage

The `test_subtask2.cpp` program tests:

#### ✅ **Small Dataset (16 KB - fits in L1 cache)**
- Sequential Access
- Random Access
- **Expected:** Very fast, minimal difference between data structures

#### ✅ **Medium Dataset (256 KB - fits in L2 cache)**
- Sequential Access
- Random Access
- **Expected:** Some performance degradation vs L1

#### ✅ **Large Dataset (4 MB - fits in L3 cache)**
- Sequential Access
- Random Access
- **Expected:** Clear difference between Sequential and Random

#### ✅ **Very Large Dataset (64 MB - doesn't fit in L3)**
- Sequential Access
- Random Access
- **Expected:** Major performance cliff, Random access very slow

---

## Understanding Your Results

### What to Look For

#### 1. **Bandwidth Trends**
```
GOOD Results:
  Sequential > Random (always)
  L1 > L2 > L3 > RAM (bandwidth decreases with cache level)
  DirectAccessArray >= ChainedHashTable for Sequential

Example:
  DirectAccessArray (16KB, Seq):  100,000 MB/s ✓
  DirectAccessArray (64MB, Seq):   20,000 MB/s ✓
  DirectAccessArray (64MB, Rand):   2,000 MB/s ✓
```

#### 2. **Latency Trends**
```
GOOD Results:
  Sequential < Random (always)
  L1 < L2 < L3 < RAM (latency increases with memory level)
  DirectAccessArray <= other structures

Example:
  DirectAccessArray (16KB, Seq):     5 cycles ✓
  DirectAccessArray (64MB, Seq):    50 cycles ✓
  DirectAccessArray (64MB, Rand):  150 cycles ✓
```

#### 3. **Data Structure Comparisons**

**For Sequential Access:**
```
DirectAccessArray ≈ ChainedHash(bin=1) > ChainedHash(bin=16) > BinarySearch
(All benefit from prefetching, but DirectAccess is simplest)
```

**For Random Access:**
```
DirectAccessArray ≈ ChainedHash(bin=1) > ChainedHash(bin=16) >> BinarySearch
(BinarySearch suffers most from random pattern)
```

---

## Troubleshooting

### Issue: All values are 0
**Problem:** Benchmark not running correctly
**Solution:**
```bash
# Check if PerfEvent is working
perf stat ls
# If that fails, you may need to enable performance counters
```

### Issue: Sequential and Random have same performance
**Problem:** Not enough iterations or dataset too small
**Solution:** The implementation already handles this, but you can increase `num_lookups` in the code if needed

### Issue: Results are inconsistent
**Problem:** Other processes interfering
**Solution:**
```bash
# Kill unnecessary processes
# Run with numactl for consistent NUMA placement
numactl --membind 0 --physcpubind 1 ./your_program
```

### Issue: Compilation fails with "undefined reference to numa_*"
**Problem:** libnuma not linked
**Solution:**
```bash
# Make sure -lnuma is at the end of g++ command
g++ ... -o output -lnuma
```

---

## Sample Expected Output

```
═══════════════════════════════════════════════════════════════
Testing: Size = 16 KB, Pattern = Sequential
═══════════════════════════════════════════════════════════════

BANDWIDTH (MB/s):
  DirectAccessArray:          98543 MB/s
  BinarySearch:               45231 MB/s
  ChainedHash(bin_size=1):    89234 MB/s
  ChainedHash(bin_size=16):   76543 MB/s

LATENCY (cycles/lookup):
  DirectAccessArray:              3 cycles
  BinarySearch:                   8 cycles
  ChainedHash(bin_size=1):        4 cycles
  ChainedHash(bin_size=16):       5 cycles

SANITY CHECKS:
  Bandwidth values > 0:     ✓ PASS
  Latency values > 0:       ✓ PASS
  DirectAccess fastest:     ✓ PASS
  BinarySearch reasonable:  ✓ PASS

═══════════════════════════════════════════════════════════════
Testing: Size = 64 MB, Pattern = Random
═══════════════════════════════════════════════════════════════

BANDWIDTH (MB/s):
  DirectAccessArray:           2134 MB/s
  BinarySearch:                 543 MB/s
  ChainedHash(bin_size=1):     1876 MB/s
  ChainedHash(bin_size=16):    1234 MB/s

LATENCY (cycles/lookup):
  DirectAccessArray:            156 cycles
  BinarySearch:                 876 cycles
  ChainedHash(bin_size=1):      189 cycles
  ChainedHash(bin_size=16):     234 cycles

SANITY CHECKS:
  Bandwidth values > 0:     ✓ PASS
  Latency values > 0:       ✓ PASS
  BinarySearch reasonable:  ✓ PASS
```

---

## Files Created for Testing

| File | Purpose |
|------|---------|
| `test_subtask2.cpp` | Comprehensive test program |
| `test_subtask2.sh` | Build and run script |
| `collect_data.cpp` | Full experiment data collection |
| `plot_results.py` | Create plots from CSV data |
| `TESTING_GUIDE.md` | This file |

---

## Checklist

Before submitting, verify:

- [ ] ✓ Basic tests pass (sub-task 1)
- [ ] ✓ Sub-task 2 tests show reasonable results
- [ ] ✓ Sequential access is faster than Random
- [ ] ✓ Larger datasets have higher latency
- [ ] ✓ Larger datasets have lower bandwidth
- [ ] ✓ All 4 data structures return valid values
- [ ] ✓ CSV file generated successfully
- [ ] ✓ Plots look reasonable
- [ ] ✓ Ready to write the report

---

## Next: Writing the Report

Once testing is complete:

1. ✅ Review `benchmark_plots.pdf`
2. ✅ Analyze `benchmark_results.csv`
3. ✅ Write 1-2 page report using template in `SUMMARY.md`
4. ✅ Include both plots in report
5. ✅ Answer all 6 required analysis questions
6. ✅ Save as `report.pdf`
7. ✅ Push to GitLab

---

**Good luck! 🚀**
