#include "Benchmarking.hpp"
#include "catch.hpp"
#include <thread>


///// ----------------------- BASIC TEST CASES ----------------------- /////

TEST_CASE("TEST LATENCY - 4KB to 512MB simple test", "[benchmark-latency]") {
  uint64_t lat_small = latency(4);
  uint64_t lat_medium = latency(4096);
  uint64_t lat_large = latency(524288);
  REQUIRE(((lat_small < lat_medium) && (lat_medium < lat_large)));
}

TEST_CASE("TEST BANDWIDTH - 4KB to 512MB simple test", "[benchmark-bandwidth]") {
  uint64_t bw_small = bandwidth(4);
  uint64_t bw_medium = bandwidth(4096);
  uint64_t bw_large = bandwidth(524288);
  REQUIRE(((bw_small > bw_medium) && (bw_medium > bw_large)));
}

