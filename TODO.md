
# Project Tasks from Specification

This file outlines the tasks required to complete the project as described in `FOMO_Project_1_Benchmarking.pdf`.

## Sub-task 1: Basic Hardware Benchmarking (3 points)

- [x] **Implement Measurement Logic:**
  - [x] Implement logic to measure average latency of each memory hierarchy level (L1, L2, L3, DDR).
  - [x] Implement logic to measure average bandwidth of each memory hierarchy level.
- [x] **Latency Benchmark:**
  - [x] Use the shuffled linked list to measure access latency.
  - [x] Incrementally increase the size of the linked list to observe performance degradation.
- [x] **Bandwidth Benchmark:**
  - [x] Use a simple array-based workload to measure memory bandwidth.
- [ ] **Pass Test Cases:**
  - [x] Pass 2 basic test cases (1 point total).
    - [x] 0.5 point Basic Bandwidth
    - [x] 0.5 point Basic Latency
  - [ ] Pass 2 advanced test cases (2 points total). **=> should upload to test**
    - [ ] 1 point Advanced Bandwidth
    - [ ] 1 point Advanced Latency

## Sub-task 2: Data Structure Benchmarking (6 points)

- [ ] **Implement Benchmarking Logic:**
  - [ ] Create a dataset generator for given input sizes (16KB to 512MB).
  - [ ] Implement lookup logic for sequential and random access patterns.
  - [ ] Implement benchmarking logic to load the dataset and measure lookup performance (bandwidth and latency) for:
    - Directly accessed array
    - Binary search over a list
    - Chained hash table
- [ ] **Pass Advanced Test Cases (6 points total):**
  - [ ] 1 point Random Access (Small Dataset)
  - [ ] 1 point Random Access (Medium Dataset)
  - [ ] 1 point Random Access (Large Dataset)
  - [ ] 1 point Sequential Access (Small Dataset)
  - [ ] 1 point Sequential Access (Medium Dataset)
  - [ ] 1 point Sequential Access (Large Dataset)

## Sub-task 3: The Report (6 points)

- [ ] **Write a 1-2 Page Report:**
  - [ ] Font size 11.
  - [ ] Add your name and matriculation number at the top.
- [ ] **Content:**
  - [ ] **(1 point)** Explanation of the benchmarking setup (dataset preparation, data structure evaluation).
  - [ ] **(1 point)** A plot showing the bandwidth of all data structures across varied access patterns and dataset sizes.
  - [ ] **(1 point)** A plot showing the latency of all data structures across varied access patterns and dataset sizes.
  - [ ] **(1 point)** Compare the performance of `DirectAccessArray` and `ChainedHashTable` (bin size 1).
  - [ ] **(1 point)** Compare the performance of `BinarySearch` with random vs. sequential access.
  - [ ] **(1 point)** Compare the performance of `ChainedHashTable` with a bin size of 1 vs. a bin size of 16.
  - [ ] Back up all claims with analysis of performance counters (e.g., LLC misses, L1 misses, branch miss-predictions).

## Submission Guidelines

- [ ] Fork the provided GitLab repository.
- [ ] Modify the code in `src/Benchmarking.cpp` to pass the tests.
- [ ] Consult the `README` for the compilation flow.
- [ ] Push to the remote repository to trigger the CI runner for evaluation.
- [ ] The final report should be submitted as a `.pdf` file in the repository.
