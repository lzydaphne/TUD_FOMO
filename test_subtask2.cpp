#include "Benchmarking.hpp"
#include <iostream>
#include <iomanip>
#include <vector>

void print_separator() {
    std::cout << std::string(80, '=') << std::endl;
}

void test_benchmark_datastructure(uint64_t size_kb, AccessPattern pattern) {
    const char* pattern_name = (pattern == AccessPattern::Sequential) ? "Sequential" : "Random";

    std::cout << "\n";
    print_separator();
    std::cout << "Testing: Size = " << size_kb << " KB, Pattern = " << pattern_name << std::endl;
    print_separator();

    auto results = benchmark_datastructure(size_kb, pattern);

    // Unpack results: {bw_1, bw_2, bw_3, bw_4, lat_1, lat_2, lat_3, lat_4}
    uint64_t bw_direct = results[0];
    uint64_t bw_binary = results[1];
    uint64_t bw_hash1 = results[2];
    uint64_t bw_hash16 = results[3];
    uint64_t lat_direct = results[4];
    uint64_t lat_binary = results[5];
    uint64_t lat_hash1 = results[6];
    uint64_t lat_hash16 = results[7];

    std::cout << "\nBANDWIDTH (MB/s):" << std::endl;
    std::cout << "  DirectAccessArray:       " << std::setw(10) << bw_direct << " MB/s" << std::endl;
    std::cout << "  BinarySearch:            " << std::setw(10) << bw_binary << " MB/s" << std::endl;
    std::cout << "  ChainedHash(bin_size=1): " << std::setw(10) << bw_hash1 << " MB/s" << std::endl;
    std::cout << "  ChainedHash(bin_size=16):" << std::setw(10) << bw_hash16 << " MB/s" << std::endl;

    std::cout << "\nLATENCY (cycles/lookup):" << std::endl;
    std::cout << "  DirectAccessArray:       " << std::setw(10) << lat_direct << " cycles" << std::endl;
    std::cout << "  BinarySearch:            " << std::setw(10) << lat_binary << " cycles" << std::endl;
    std::cout << "  ChainedHash(bin_size=1): " << std::setw(10) << lat_hash1 << " cycles" << std::endl;
    std::cout << "  ChainedHash(bin_size=16):" << std::setw(10) << lat_hash16 << " cycles" << std::endl;

    // Sanity checks
    std::cout << "\nSANITY CHECKS:" << std::endl;

    // Check that values are reasonable
    bool bw_reasonable = (bw_direct > 0 && bw_binary > 0 && bw_hash1 > 0 && bw_hash16 > 0);
    bool lat_reasonable = (lat_direct > 0 && lat_binary > 0 && lat_hash1 > 0 && lat_hash16 > 0);

    std::cout << "  Bandwidth values > 0:     " << (bw_reasonable ? "✓ PASS" : "✗ FAIL") << std::endl;
    std::cout << "  Latency values > 0:       " << (lat_reasonable ? "✓ PASS" : "✗ FAIL") << std::endl;

    // For sequential access, DirectAccess should be fastest
    if (pattern == AccessPattern::Sequential) {
        bool direct_fastest = (lat_direct <= lat_binary) && (lat_direct <= lat_hash1);
        std::cout << "  DirectAccess fastest:     " << (direct_fastest ? "✓ PASS" : "⚠ WARNING") << std::endl;
    }

    // BinarySearch should be slower for Random than Sequential (if we had both)
    // For now, just check that BinarySearch has reasonable latency
    bool binary_reasonable = (lat_binary < 10000); // Less than 10k cycles is reasonable
    std::cout << "  BinarySearch reasonable:  " << (binary_reasonable ? "✓ PASS" : "⚠ WARNING") << std::endl;

    print_separator();
}

int main() {
    std::cout << "\n";
    std::cout << "╔════════════════════════════════════════════════════════════════╗\n";
    std::cout << "║         SUB-TASK 2: DATA STRUCTURE BENCHMARKING TEST          ║\n";
    std::cout << "╚════════════════════════════════════════════════════════════════╝\n";

    // Test cases covering small, medium, and large datasets
    // Both sequential and random access patterns

    std::vector<uint64_t> sizes = {
        16,      // Small - fits in L1
        256,     // Medium - fits in L2
        4096,    // Large - fits in L3
        65536    // Very large - doesn't fit in L3
    };

    std::vector<AccessPattern> patterns = {
        AccessPattern::Sequential,
        AccessPattern::Random
    };

    for (auto size : sizes) {
        for (auto pattern : patterns) {
            try {
                test_benchmark_datastructure(size, pattern);
            } catch (const std::exception& e) {
                std::cerr << "ERROR: " << e.what() << std::endl;
            }
        }
    }

    std::cout << "\n";
    std::cout << "╔════════════════════════════════════════════════════════════════╗\n";
    std::cout << "║                        TEST COMPLETE                           ║\n";
    std::cout << "╚════════════════════════════════════════════════════════════════╝\n";
    std::cout << "\n";
    std::cout << "Next steps:\n";
    std::cout << "1. Review the results above\n";
    std::cout << "2. Verify that patterns make sense (Sequential should be faster)\n";
    std::cout << "3. Run full experiment suite for the report\n";
    std::cout << "4. Create plots from the data\n";
    std::cout << "\n";

    return 0;
}
