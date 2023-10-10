#include <benchmark/benchmark.h>
#include <wellnmea/sentence.hpp>
#include <wellnmea/formats/format.hpp>
#include <wellnmea/instructions/instruction.hpp>
#include <list>
#include <string>
#include <random>

#include "shot_instruction.hpp"

static void BM_OneInstruction(benchmark::State &state)
{

  wellnmea::formats::Format format({new ShotInstruction()});
  wellnmea::Sentence sentence;
  sentence.fields.push_back("");

  for (auto _ : state)
  {
    for (size_t i = 0; i < state.range(0); i++)
    {
      format.parse(sentence);
    }
  }
}

static void BM_Fields(benchmark::State &state)
{

  wellnmea::formats::Format::Items instructions;
  wellnmea::Sentence sentence;
  for (size_t i = 0; i < state.range(0); i++)
  {
    instructions.push_back(new ShotInstruction);
    sentence.fields.push_back("field");
  }
  wellnmea::formats::Format format(instructions);
  for (auto _ : state)
  {
    format.parse(sentence);
  }
}

BENCHMARK(BM_OneInstruction)
    ->Range(8, 8 << 10)
    ->Unit(::benchmark::TimeUnit::kMillisecond);

BENCHMARK(BM_Fields)
    ->Range(8, 8 << 10)
    ->Unit(::benchmark::TimeUnit::kMillisecond);

BENCHMARK_MAIN();