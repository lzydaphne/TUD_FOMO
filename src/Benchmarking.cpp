
#include "Benchmarking.hpp"

constexpr size_t cache_line = 64;

static_assert(sizeof(Node) == cache_line);
static_assert(alignof(Node) == cache_line);

static int64_t seconds = 10;


uint64_t latency(uint64_t size_kb) {
  // 1. Generate dataset

  uint64_t size = size_kb * 1024 / 64;
  std::vector<size_t> order(size);
  for (size_t i = 0; i < size; i++) {
    order[i] = i;
  }
  std::shuffle(order.begin(), order.end(), std::random_device());


  // 2. Initialize data structure

  std::vector<Node> linked_list(size);
  for (size_t i = 0; i < size - 1; i++) {
    linked_list[i].data = i;
  }

  for (size_t i = 0; i < size - 1; i++) {
    linked_list[order[i]].next = &linked_list[order[i + 1]];
  }
  linked_list[order[size - 1]].next = &linked_list[order[0]];


  // 3. Measure data structure

  // Warm-up phase: traverse once to load into cache if it fits
  Node *current = &linked_list[0];
  for (size_t i = 0; i < size; i++) {
    current = current->next;
  }
  doNotOptimizeAway(current);

  // Actual measurement: traverse multiple times for accurate measurement
  uint64_t num_iterations = std::max<uint64_t>(1000000 / size, 100);

  PerfEvent e;
  e.startCounters();

  current = &linked_list[0];
  for (size_t iter = 0; iter < num_iterations; iter++) {
    for (size_t i = 0; i < size; i++) {
      current = current->next;
    }
  }
  doNotOptimizeAway(current);

  e.stopCounters();

  uint64_t total_lookups = num_iterations * size;
  uint64_t cycles = static_cast<uint64_t>(e.getCounter("cycles"));
  uint64_t cycles_per_lookup = cycles / total_lookups;

  return cycles_per_lookup; // Latency in Processor Clock Cycles
};

uint64_t bandwidth(uint64_t size_kb) {

  // 1. Initialize data structure

  uint64_t size = size_kb * 1024 / 64;
  std::vector<Node> array(size);
  for (size_t i = 0; i < size; i++) {
    array[i].data = i;
  }

  // 2. Measure data structure

  // Warm-up phase
  volatile uint64_t res{0};
  for (size_t i = 0; i < size; i++) {
    res = array[i].data;
  }

  // Actual measurement: read sequentially multiple times
  uint64_t num_iterations = std::max<uint64_t>(1000000 / size, 100);

  auto start = std::chrono::high_resolution_clock::now();

  for (size_t iter = 0; iter < num_iterations; iter++) {
    for (size_t i = 0; i < size; i++) {
      res = array[i].data;
    }
  }
  doNotOptimizeAway(res);

  auto end = std::chrono::high_resolution_clock::now();

  // Calculate bandwidth
  uint64_t total_bytes = num_iterations * size * sizeof(Node);
  auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
  double seconds_elapsed = duration / 1e9;

  // Convert to MB/s
  uint64_t mbps = static_cast<uint64_t>((total_bytes / seconds_elapsed) / (1024.0 * 1024.0));

  return mbps;
};

std::vector<uint64_t> benchmark_datastructure(uint64_t size_kb, AccessPattern access_pattern) {

  // 1. Data generation
  uint64_t num_nodes = size_kb * 1024 / 64; // Each Node is 64 bytes

  // Generate keys in ascending order
  std::vector<uint64_t> keys(num_nodes);
  for (uint64_t i = 0; i < num_nodes; i++) {
    keys[i] = i;
  }

  // Create lookup sequence
  std::vector<uint64_t> lookup_sequence;
  uint64_t num_lookups = std::max<uint64_t>(num_nodes * 10, 10000);

  if (access_pattern == AccessPattern::Sequential) {
    // Sequential access: iterate through keys in order, repeating as needed
    for (uint64_t i = 0; i < num_lookups; i++) {
      lookup_sequence.push_back(keys[i % num_nodes]);
    }
  } else { // Random access
    // Create a shuffled sequence ensuring each key is looked up equally
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
  ChainedHashTable ds3(num_nodes, 1.0);   // bin_size = 1
  ChainedHashTable ds4(num_nodes, 16.0);  // bin_size = 16

  // Populate data structures with nodes in ascending order
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
  auto measure = [&](IDataStructure& ds) -> std::pair<uint64_t, uint64_t> {
    // Warm-up
    uint64_t warmup_sum = 0;
    for (size_t i = 0; i < std::min<size_t>(1000, lookup_sequence.size()); i++) {
      Node* n = ds.lookup(lookup_sequence[i]);
      if (n) warmup_sum = warmup_sum + n->data;
    }
    doNotOptimizeAway(warmup_sum);

    // Measure latency using cycles
    PerfEvent e;
    e.startCounters();

    uint64_t sum = 0;
    for (const auto& key : lookup_sequence) {
      Node* n = ds.lookup(key);
      if (n) sum = sum + n->data;
    }

    e.stopCounters();

    uint64_t cycles = static_cast<uint64_t>(e.getCounter("cycles"));
    uint64_t latency_per_lookup = cycles / lookup_sequence.size();

    // Measure bandwidth using time
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

    return {bandwidth_mbps, latency_per_lookup};
  };

  // Measure all data structures
  auto [bw1, lat1] = measure(ds1);
  auto [bw2, lat2] = measure(ds2);
  auto [bw3, lat3] = measure(ds3);
  auto [bw4, lat4] = measure(ds4);

  // Return in the specified order: {bw_1, bw_2, bw_3, bw_4, lat_1, lat_2, lat_3, lat_4}
  return {bw1, bw2, bw3, bw4, lat1, lat2, lat3, lat4};
}

