#include <benchmark/benchmark.h>
#include <wellnmea/sentence.hpp>
#include <wellnmea/formats/hdt.hpp>
#include <wellnmea/formats/hdt.hpp>

#include <list>
#include <string>
#include <random>

static void BM_hdt(benchmark::State &state)
{
  Hdt format;

  wellnmea::Sentence sentence;
  sentence.fields.push_back("126.9");
  sentence.fields.push_back("T");

  for (auto _ : state)
  {
    for (size_t i = 0; i < state.range(0); i++)
    {
      format.parse(sentence);
    }
  }
}

BENCHMARK(BM_hdt)
    ->Range(1, 8 << 9)
    ->Unit(benchmark::TimeUnit::kMicrosecond);

BENCHMARK_MAIN();