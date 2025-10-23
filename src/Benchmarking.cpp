
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

  Node *current = &linked_list[0];
  for (size_t i = 0; i < size; i++) {
    current = current->next;
  }
  doNotOptimizeAway(current);

  return 0; // Latency in Processor Clock Cycles
};

uint64_t bandwidth(uint64_t size_kb) {

  // 1. Initialize data structure

  uint64_t size = size_kb * 1024 / 64;
  std::vector<Node> array(size);
  for (size_t i = 0; i < size; i++) {
    array[i].data = i;
  }

  // 2. Measure data structure

  uint64_t mbps{0};
  volatile uint64_t res{0};

  for (size_t i = 0; i < size; i++) {
    res = array[i].data;
  }

  return mbps;
};

std::vector<uint64_t> benchmark_datastructure(uint64_t size, AccessPattern access_pattern) {

  // your solution here

  // 1. Data generation

  // 2. Data structure initialization

  // 3. Measurement
  return {0, 0, 0, 0, 0, 0, 0, 0};
}

