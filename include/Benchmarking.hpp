#pragma once

#include "PerfEvent.hpp"
#include <random>
#include <thread>
#include <cstdio>
#include <cstdlib>
#include <iomanip>
#include <algorithm>
#include <chrono>
#include <cstdint>
#include <iostream>
#include <map>
#include <unordered_map>
#include <vector>
#include <cassert>
#include <cstring>
#include <limits>

// Benchmarking Suite


enum AccessPattern{
   Sequential,
   Random
};


struct alignas(64) Node {
    uint64_t key;
    uint64_t data;
    Node *next;
    // padding to make struct 64 bytes (64 - 16 - pointer)
    char padding[64 - 16 - sizeof(Node *)];
};


struct IDataStructure {
    virtual ~IDataStructure() = default;
    virtual void insert(uint64_t key, const Node &node) = 0;
    virtual Node* lookup(uint64_t key) = 0;
};



// Data Structure #1 - Directly accessing an array where the key = index
class DirectAccessArray : public IDataStructure {
public:
    std::vector<Node> map_;
    inline DirectAccessArray(size_t size = 0) { map_.resize(size); }
    inline void reserve_and_set_size(size_t size) { map_.resize(size); }

    inline void insert(uint64_t key, const Node& node) override {
        if (key >= map_.size()) map_.resize(key + 1);
        map_[key] = node;
    }

    inline Node* lookup(uint64_t key) override {
        if (key >= map_.size()) return nullptr;
        return &map_[key];
    }
};

// Data Structure #2 - Accessing an array where keys have to be looked for in a binary search
class BinarySearch : public IDataStructure {
public:
    std::vector<Node> map_;
    inline BinarySearch(size_t size = 0) { map_.resize(size); }
    inline void reserve_and_set_size(size_t size) { map_.resize(size); }

    inline void insert(uint64_t key, const Node& node) override {
        if (key >= map_.size()) 
            map_.resize(key + 1);
        map_[key] = node;
    }

    inline Node* lookup(uint64_t key) override {

        if (key >= map_.size()) {
            return nullptr;
        }
        size_t lowIdx= 0;
        size_t highIdx= map_.size();
        
        while(lowIdx < highIdx) { 
            size_t midIdx= (lowIdx + highIdx)/2; 
            Node* node  = &map_[midIdx];

            if (node->key == key) {
                  return node; 
            } else if (key > node->key) {
                  lowIdx = midIdx + 1;
            } else {
                  highIdx = midIdx;
            }
        } 

        return nullptr;    
    }
};


// Data Structure #3 Chained hash table made using a 2D dynamic vector
class ChainedHashTable : public IDataStructure {
public:
    std::vector<std::vector<Node>> map_;
    explicit ChainedHashTable(std::size_t expectedCount, double bin_size = 1)
    {
        std::size_t numBuckets = std::max<std::size_t>( 1,
            nextPowerOfTwo( static_cast<std::size_t>(std::ceil(expectedCount / bin_size)) )
        );
        map_.resize(numBuckets);
        size_ = 0;
    }

    inline void insert(uint64_t key, const Node &node) override {
        auto& bucket = map_[indexFor(key)];
        for (auto& kv : bucket) {
            if (kv.key == key) { 
                kv = node; return; 
            } 
        }
        bucket.emplace_back(node);
        ++size_;
    }

    inline Node* lookup(uint64_t key) override { 
        cntLookup++;
        auto& bucket = map_[indexFor(key)];
        for (auto& kv : bucket) {
            cntBucketTraverse++; 
            if (kv.key == key) {
                return &kv; 
            }
        }   
        return nullptr; 
    }

private:

    std::size_t size_;
    uint64_t cntLookup= 0;
    uint64_t cntBucketTraverse= 0;

    inline std::size_t nextPowerOfTwo(std::size_t x) {
        if (x < 1) 
            return 1;
        return std::bit_ceil(x);
    }

    inline std::size_t indexFor(uint64_t key) const {
        return key & (map_.size() - 1);
    }
};


template <class T> inline void doNotOptimizeAway(T &&datum) {
  asm volatile("" : : "r,m"(datum) : "memory");
}

/**
   * Return the measured latency of the cache for a given data structure size (in KB)
   * in cycles per lookup
   */
uint64_t latency(uint64_t size_kb);


/**
   * Return the measured bandwidth of the cache for a given data structure size (in KB)
   * in MB/s per lookup (rounded to an integer).
   */
uint64_t bandwidth(uint64_t size_kb);

/**
   * Return the bandwidths and latencies of 4 distinct data structures:
   DirectAccessArray
   BinarySearch
   ChainedHashTable with maxLoad = 1
   ChainedHashTable with maxLoad = 16
   signature {bw_1, bw_2, bw_3, bw_4, lat_1, lat_2, lat_3, lat_4}
   */
std::vector<uint64_t> benchmark_datastructure(uint64_t size_kb, AccessPattern access_pattern);




