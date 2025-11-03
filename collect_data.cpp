#include "Benchmarking.hpp"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <string>

// Data collection program for the report
// Runs comprehensive benchmarks and outputs CSV for plotting

int main() {
    std::cout << "Starting comprehensive benchmark data collection..." << std::endl;
    std::cout << "This will take several minutes. Please wait..." << std::endl;
    std::cout << std::endl;

    // Output file
    std::ofstream csv_file("benchmark_results.csv");

    // CSV Header
    csv_file << "size_kb,pattern,";
    csv_file << "bw_directaccess,bw_binarysearch,bw_hash1,bw_hash16,";
    csv_file << "lat_directaccess,lat_binarysearch,lat_hash1,lat_hash16";
    csv_file << std::endl;

    // Test sizes: 16KB to 512MB
    std::vector<uint64_t> sizes = {
        16,      // 16 KB  - L1 cache
        64,      // 64 KB  - L1/L2 boundary
        256,     // 256 KB - L2 cache
        1024,    // 1 MB   - L2/L3 boundary
        4096,    // 4 MB   - L3 cache
        16384,   // 16 MB  - L3/RAM boundary
        65536,   // 64 MB  - RAM
        262144,  // 256 MB - RAM
        524288   // 512 MB - RAM
    };

    std::vector<std::string> pattern_names = {"Sequential", "Random"};
    std::vector<AccessPattern> patterns = {AccessPattern::Sequential, AccessPattern::Random};

    int total_tests = sizes.size() * patterns.size();
    int current_test = 0;

    for (size_t p = 0; p < patterns.size(); p++) {
        for (size_t s = 0; s < sizes.size(); s++) {
            current_test++;

            uint64_t size_kb = sizes[s];
            AccessPattern pattern = patterns[p];
            std::string pattern_name = pattern_names[p];

            std::cout << "[" << current_test << "/" << total_tests << "] ";
            std::cout << "Testing " << size_kb << " KB, " << pattern_name << "... ";
            std::cout.flush();

            try {
                auto results = benchmark_datastructure(size_kb, pattern);

                // Write to CSV
                csv_file << size_kb << ",";
                csv_file << pattern_name << ",";

                // Bandwidth values
                csv_file << results[0] << "," << results[1] << ","
                         << results[2] << "," << results[3] << ",";

                // Latency values
                csv_file << results[4] << "," << results[5] << ","
                         << results[6] << "," << results[7];

                csv_file << std::endl;

                std::cout << "✓ Done" << std::endl;
            } catch (const std::exception& e) {
                std::cout << "✗ Failed: " << e.what() << std::endl;
                csv_file << size_kb << "," << pattern_name << ",";
                csv_file << "0,0,0,0,0,0,0,0" << std::endl;
            }
        }
    }

    csv_file.close();

    std::cout << std::endl;
    std::cout << "╔════════════════════════════════════════════════════════════════╗" << std::endl;
    std::cout << "║              DATA COLLECTION COMPLETE!                         ║" << std::endl;
    std::cout << "╚════════════════════════════════════════════════════════════════╝" << std::endl;
    std::cout << std::endl;
    std::cout << "Results saved to: benchmark_results.csv" << std::endl;
    std::cout << std::endl;
    std::cout << "Next steps:" << std::endl;
    std::cout << "1. Review the CSV file" << std::endl;
    std::cout << "2. Run the plotting script: python3 plot_results.py" << std::endl;
    std::cout << "3. Use the plots in your report" << std::endl;
    std::cout << std::endl;

    return 0;
}
