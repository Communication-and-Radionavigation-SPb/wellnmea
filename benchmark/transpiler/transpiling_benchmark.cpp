#include <benchmark/benchmark.h>
#include <wellnmea/sentence.hpp>
#include <wellnmea/formats/format.hpp>
#include <wellnmea/instructions/instruction.hpp>
#include <wellnmea/transpiler.hpp>

#include <iostream>
#include <random>
#include <sstream>

#include "mocked_instruction.hpp"

static void BM_Transpiling(benchmark::State &state)
{
  wellnmea::Transpiler transpiler;

  wellnmea::Sentence sentence;
  sentence.formatter = "TST";
  sentence.talker = "TS";
  sentence.fields.push_back("127.0");

  auto fmt_ptr = new wellnmea::formats::Format({new MockedInstruction()});
  auto fmt = wellnmea::Transpiler::FmtPtr(fmt_ptr);

  transpiler.connect("TST", fmt);

  for (auto _ : state)
  {
    for (size_t i = 0; i < state.range(0); i++)
    {
      transpiler.parse(sentence);
    }
  }
}

static void BM_MultipleTranspiling(benchmark::State &state)
{
  wellnmea::Transpiler transpiler;

  wellnmea::Sentence sentence;
  sentence.formatter = "TST";
  sentence.talker = "TS";
  sentence.fields.push_back("127.0");
  sentence.fields.push_back("127.0");

  auto fmt_ptr = new wellnmea::formats::Format({new MockedInstruction(), new MockedInstruction()});
  auto fmt = wellnmea::Transpiler::FmtPtr(fmt_ptr);

  transpiler.connect("TS0", fmt);
  transpiler.connect("TS1", fmt);
  transpiler.connect("TS2", fmt);
  transpiler.connect("TS3", fmt);
  transpiler.connect("TS4", fmt);

  for (auto _ : state)
  {
    state.PauseTiming();
    int index = std::rand() % 5;
    std::stringstream ss;
    ss << "TS" << index;
    sentence.formatter = ss.str();
    state.ResumeTiming();
    for (size_t i = 0; i < state.range(0); i++)
    {
      transpiler.parse(sentence);
    }
  }
}

BENCHMARK(BM_Transpiling)->Range(8, 8 << 12)->Unit(::benchmark::TimeUnit::kMillisecond);
BENCHMARK(BM_MultipleTranspiling)->Range(8, 8 << 12)->Unit(::benchmark::TimeUnit::kMillisecond);

BENCHMARK_MAIN();