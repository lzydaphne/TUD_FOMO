#include "Benchmarking.hpp"
#include "catch.hpp"
#include <thread>
#include <iostream>
#include <iomanip>
#include "PerfEvent.hpp"

// Copied and modified from src/Benchmarking.cpp to include cache miss measurements

class CacheMisses : public PerfEvent {
public:
    CacheMisses() : PerfEvent(PERF_TYPE_HARDWARE, PERF_COUNT_HW_CACHE_MISSES) {}
};

std::vector<uint64_t> benchmark_datastructure_with_cache_misses(uint64_t size_kb, AccessPattern access_pattern) {

  // 1. Data generation
  uint64_t num_nodes = size_kb * 1024 / 64; // Each Node is 64 bytes

  std::vector<uint64_t> keys(num_nodes);
  for (uint64_t i = 0; i < num_nodes; i++) {
    keys[i] = i;
  }

  std::vector<uint64_t> lookup_sequence;
  uint64_t num_lookups = std::max<uint64_t>(num_nodes * 10, 10000);

  if (access_pattern == AccessPattern::Sequential) {
    for (uint64_t i = 0; i < num_lookups; i++) {
      lookup_sequence.push_back(keys[i % num_nodes]);
    }
  } else { // Random access
    uint64_t reps = num_lookups / num_nodes;
    for (uint64_t r = 0; r < reps; r++) {
      std::vector<uint64_t> shuffled_keys = keys;
      std::shuffle(shuffled_keys.begin(), shuffled_keys.end(), std::mt19937(r));
      lookup_sequence.insert(lookup_sequence.end(), shuffled_keys.begin(), shuffled_keys.end());
    }
  }

  // 2. Data structure initialization
  DirectAccessArray ds1(num_nodes);
  BinarySearch ds2(num_nodes);
  ChainedHashTable ds3(num_nodes, 1.0);
  ChainedHashTable ds4(num_nodes, 16.0);

  for (uint64_t i = 0; i < num_nodes; i++) {
    Node node;
    node.key = keys[i];
    node.data = i;
    node.next = nullptr;

    ds1.insert(keys[i], node);
    ds2.insert(keys[i], node);
    ds3.insert(keys[i], node);
    ds4.insert(keys[i], node);
  }

  // 3. Measurement helper function
  auto measure = [&](IDataStructure& ds) -> std::tuple<uint64_t, uint64_t, uint64_t> {
    // Warm-up
    uint64_t warmup_sum = 0;
    for (size_t i = 0; i < std::min<size_t>(1000, lookup_sequence.size()); i++) {
      Node* n = ds.lookup(lookup_sequence[i]);
      if (n) warmup_sum = warmup_sum + n->data;
    }
    doNotOptimizeAway(warmup_sum);

    // Measure latency and cache misses
    PerfEvent e;
    CacheMisses cm;
    e.startCounters();
    cm.start();

    uint64_t sum = 0;
    for (const auto& key : lookup_sequence) {
      Node* n = ds.lookup(key);
      if (n) sum = sum + n->data;
    }

    cm.stop();
    e.stopCounters();
    doNotOptimizeAway(sum);

    uint64_t cycles = static_cast<uint64_t>(e.getCounter("cycles"));
    uint64_t latency_per_lookup = cycles / lookup_sequence.size();
    uint64_t cache_misses = cm.read();

    // Measure bandwidth
    auto start = std::chrono::high_resolution_clock::now();

    sum = 0;
    for (const auto& key : lookup_sequence) {
      Node* n = ds.lookup(key);
      if (n) sum = sum + n->data;
    }
    doNotOptimizeAway(sum);

    auto end = std::chrono::high_resolution_clock::now();

    uint64_t total_bytes = lookup_sequence.size() * sizeof(Node);
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
    double seconds_elapsed = duration / 1e9;
    uint64_t bandwidth_mbps = static_cast<uint64_t>((total_bytes / seconds_elapsed) / (1024.0 * 1024.0));

    return {bandwidth_mbps, latency_per_lookup, cache_misses};
  };

  // Measure all data structures
  auto [bw1, lat1, cm1] = measure(ds1);
  auto [bw2, lat2, cm2] = measure(ds2);
  auto [bw3, lat3, cm3] = measure(ds3);
  auto [bw4, lat4, cm4] = measure(ds4);

  return {bw1, bw2, bw3, bw4, lat1, lat2, lat3, lat4, cm1, cm2, cm3, cm4};
}


///// ----------------------- SUB-TASK 2 TEST CASES ----------------------- /////

void print_separator() {
  std::cout << std::string(70, '=') << std::endl;
}

void print_results(const std::string& title, uint64_t size_kb, const std::string& pattern_name,
                   const std::vector<uint64_t>& results) {
  std::cout << "\n";
  print_separator();
  std::cout << title << " - Size: " << size_kb << " KB, Pattern: " << pattern_name << std::endl;
  print_separator();

  std::cout << "\nBANDWIDTH (MB/s):" << std::endl;
  std::cout << "  DirectAccessArray:       " << std::setw(10) << results[0] << " MB/s" << std::endl;
  std::cout << "  BinarySearch:            " << std::setw(10) << results[1] << " MB/s" << std::endl;
  std::cout << "  ChainedHash(bin_size=1): " << std::setw(10) << results[2] << " MB/s" << std::endl;
  std::cout << "  ChainedHash(bin_size=16):" << std::setw(10) << results[3] << " MB/s" << std::endl;

  std::cout << "\nLATENCY (cycles/lookup):" << std::endl;
  std::cout << "  DirectAccessArray:       " << std::setw(10) << results[4] << " cycles" << std::endl;
  std::cout << "  BinarySearch:            " << std::setw(10) << results[5] << " cycles" << std::endl;
  std::cout << "  ChainedHash(bin_size=1): " << std::setw(10) << results[6] << " cycles" << std::endl;
  std::cout << "  ChainedHash(bin_size=16):" << std::setw(10) << results[7] << " cycles" << std::endl;

  std::cout << "\nCACHE MISSES:" << std::endl;
  std::cout << "  DirectAccessArray:       " << std::setw(10) << results[8] << std::endl;
  std::cout << "  BinarySearch:            " << std::setw(10) << results[9] << std::endl;
  std::cout << "  ChainedHash(bin_size=1): " << std::setw(10) << results[10] << std::endl;
  std::cout << "  ChainedHash(bin_size=16):" << std::setw(10) << results[11] << std::endl;

  print_separator();
}

TEST_CASE("SUB-TASK 2: Data Structures - Sequential Small (16KB)", "[benchmark-ds-seq-small]") {
  std::cout << "\n>>> Testing Sequential Access - Small Dataset (L1 Cache)" << std::endl;

  uint64_t size_kb = 16;
  auto results = benchmark_datastructure_with_cache_misses(size_kb, AccessPattern::Sequential);

  print_results("Sequential Small", size_kb, "Sequential", results);

  for (size_t i = 0; i < 12; i++) {
    REQUIRE(results[i] > 0);
  }
  REQUIRE(results[4] < 20);

  std::cout << "✓ Sequential Small Dataset: PASS" << std::endl;
}

TEST_CASE("SUB-TASK 2: Data Structures - Sequential Large (64MB)", "[benchmark-ds-seq-large]") {
  std::cout << "\n>>> Testing Sequential Access - Large Dataset (RAM)" << std::endl;

  uint64_t size_kb = 65536;  // 64 MB
  auto results = benchmark_datastructure_with_cache_misses(size_kb, AccessPattern::Sequential);

  print_results("Sequential Large", size_kb, "Sequential", results);

  for (size_t i = 0; i < 12; i++) {
    REQUIRE(results[i] > 0);
  }

  std::cout << "✓ Sequential Large Dataset: PASS" << std::endl;
}

TEST_CASE("SUB-TASK 2: Data Structures - Random Small (16KB)", "[benchmark-ds-rand-small]") {
  std::cout << "\n>>> Testing Random Access - Small Dataset (L1 Cache)" << std::endl;

  uint64_t size_kb = 16;
  auto results = benchmark_datastructure_with_cache_misses(size_kb, AccessPattern::Random);

  print_results("Random Small", size_kb, "Random", results);

  for (size_t i = 0; i < 12; i++) {
    REQUIRE(results[i] > 0);
  }

  std::cout << "✓ Random Small Dataset: PASS" << std::endl;
}

TEST_CASE("SUB-TASK 2: Data Structures - Random Large (64MB)", "[benchmark-ds-rand-large]") {
  std::cout << "\n>>> Testing Random Access - Large Dataset (RAM)" << std::endl;

  uint64_t size_kb = 65536;  // 64 MB
  auto results = benchmark_datastructure_with_cache_misses(size_kb, AccessPattern::Random);

  print_results("Random Large", size_kb, "Random", results);

  for (size_t i = 0; i < 12; i++) {
    REQUIRE(results[i] > 0);
  }

  std::cout << "✓ Random Large Dataset: PASS" << std::endl;
}