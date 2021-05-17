

#include <benchmark/benchmark.h>

#include "zipf/rejection_inversion_generator.h"
#include "zipf/ycsb_generator.h"

static void YCSB_Create(benchmark::State& state) {
    long long N = state.range(0);
    double s = state.range(1) / 10.0;

    for (auto _ : state) {
        zipf::YCSBGenerator z(N, s);
    }
}
BENCHMARK(YCSB_Create)->Ranges({{1000000, 1000000000}, {5, 15}});

static void RIG_Create(benchmark::State& state) {
    long long N = state.range(0);
    double s = state.range(1) / 10.0;

    for (auto _ : state) {
        zipf::RejectionInverstionGenerator z(N, s);
    }
}
BENCHMARK(RIG_Create)->Ranges({{1000000, 1000000000}, {5, 15}});

static void YCSB_Sample(benchmark::State& state) {
    long long N = state.range(0);
    double s = state.range(1) / 10.0;
    zipf::YCSBGenerator z(N, s);

    for (auto _ : state) {
        z.sample();
    }
}
// Too slow for with 1B keys, so just go to 100M
BENCHMARK(YCSB_Sample)->Ranges({{1000000, 100000000}, {5, 15}});

static void RIG_Sample(benchmark::State& state) {
    long long N = state.range(0);
    double s = state.range(1) / 10.0;
    zipf::RejectionInverstionGenerator z(N, s);

    for (auto _ : state) {
        z.sample();
    }
}
BENCHMARK(RIG_Sample)->Ranges({{1000000, 1000000000}, {5, 15}});

BENCHMARK_MAIN();