**Implement a benchmark for evaluating data structure performance under real-world scenarios:**



**Commands to get the project up and running:**

```
mkdir build && cd build
cmake ..
make benchmark_test_basic_local
numactl --membind 0 --physcpubind 1 ./benchmark_test_basic_local
```
