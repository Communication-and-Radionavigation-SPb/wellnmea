#include <benchmark/benchmark.h>
#include <wellnmea/parser.hpp>
#include <wellnmea/sentence.hpp>
#include <vector>
#include <string>
#include <random>

const std::vector<std::string> message_variants{
    "$GPGGA,092750.000,5321.6802,N,00630.3372,W,1,8,1.03,61.7,M,55.2,M,,*76",
    "$GPGSA,A,3,10,07,05,02,29,04,08,13,,,,,1.72,1.03,1.38*0A",
    "$GPGSV,3,1,11,10,63,137,17,07,61,098,15,05,59,290,20,08,54,157,30*70",
    "$GPGSV,3,2,11,02,39,223,19,13,28,070,17,26,23,252,,04,14,186,14*79",
    "$GPGSV,3,3,11,29,09,301,24,16,09,020,,36,,,*76",
    "$GPRMC,092750.000,A,5321.6802,N,00630.3372,W,0.02,31.66,280511,,,A*43",
    "$GPGGA,092751.000,5321.6802,N,00630.3371,W,1,8,1.03,61.7,M,55.3,M,,*75",
    "$GPGSA,A,3,10,07,05,02,29,04,08,13,,,,,1.72,1.03,1.38*0A",
    "$GPGSV,3,1,11,10,63,137,17,07,61,098,15,05,59,290,20,08,54,157,30*70",
    "$GPGSV,3,2,11,02,39,223,16,13,28,070,17,26,23,252,,04,14,186,15*77",
    "$GPGSV,3,3,11,29,09,301,24,16,09,020,,36,,,*76",
    "$GPRMC,092751.000,A,5321.6802,N,00630.3371,W,0.06,31.66,280511,,,A*45",
    // Bad data
    "$*-->The,following,should,fail*"
    // Bad checksum 47 -> 55
    "$GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,*55",
    // Missing last parameter
    "$GPGGA,205630.945,3346.1070,N,08423.6687,W,0,03,,30.8,M,-30.8,M,*73"};

template <typename Iter, typename RandomGenerator>
Iter select_randomly(Iter start, Iter end, RandomGenerator &g)
{
  std::uniform_int_distribution<> dis(0, std::distance(start, end) - 1);
  std::advance(start, dis(g));
  return start;
}

template <typename Iter>
Iter select_randomly(Iter start, Iter end)
{
  static std::random_device rd;
  static std::mt19937 gen(rd());
  return select_randomly(start, end, gen);
}

std::string messageWithFields(int count)
{
  std::string message = "$GPHDT";
  for (size_t i = 0; i < count; i++)
  {
    message += ",90";
  }
  return message;
}

static void BM_Length(benchmark::State &state)
{
  wellnmea::Parser parser;

  for (auto _ : state)
  {
    state.PauseTiming();
    std::string message = messageWithFields(state.range(0));
    wellnmea::Sentence sentence;
    state.ResumeTiming();
    parser.parseInto(sentence, message);
  }
}

static void BM_Parse(benchmark::State &state)
{
  wellnmea::Parser parser;
  std::vector<std::string> messages;

  uint16_t validCount = 0;
  uint16_t invalidCount = 0;

  for (auto _ : state)
  {
    for (int i = 0; i < state.range(0); i++)
    {
      // Select random message
      state.PauseTiming();
      std::string message = *select_randomly(message_variants.begin(), message_variants.end());
      wellnmea::Sentence sentence;
      state.ResumeTiming();
      // Create sentence
      // Parse message
      parser.parseInto(sentence, message);
      state.PauseTiming();
      if (sentence.isValid())
        ++validCount;
      else
        ++invalidCount;
      state.ResumeTiming();
    }
  }

  state.counters["Valid"] = validCount;
  state.counters["Invalid"] = invalidCount;
}

static void BM_InvalidOnly(benchmark::State &state)
{
  wellnmea::Parser parser;
  wellnmea::Sentence sentence;

  std::string message = "$*-->The,following,should,fail*";

  uint16_t validCount = 0;
  uint16_t invalidCount = 0;

  for (auto _ : state)
  {
    for (int i = 0; i < state.range(0); i++)
    {
      state.PauseTiming();
      wellnmea::Sentence sentence;
      state.ResumeTiming();

      parser.parseInto(sentence, message);

      state.PauseTiming();
      if (sentence.isValid())
        ++validCount;
      else
        ++invalidCount;
      state.ResumeTiming();
    }
  }

  state.counters["Valid"] = validCount;
  state.counters["Invalid"] = invalidCount;
}

BENCHMARK(BM_Parse)
    ->ComputeStatistics("max", [](const std::vector<double> &v) -> double
                        { return *(std::max_element(std::begin(v), std::end(v))); })
    ->Arg(1)
    ->Arg(10)
    ->Arg(100)
    ->Arg(1000)
    ->Arg(10000)
    ->Arg(100000)
    ->Unit(::benchmark::TimeUnit::kMillisecond);

BENCHMARK(BM_InvalidOnly)
    ->ComputeStatistics("max", [](const std::vector<double> &v) -> double
                        { return *(std::max_element(std::begin(v), std::end(v))); })
    ->Arg(1)
    ->Arg(10)
    ->Arg(100)
    ->Arg(1000)
    ->Arg(10000)
    ->Arg(100000)
    ->Unit(::benchmark::TimeUnit::kMillisecond);

BENCHMARK(BM_Length)
    ->Arg(1)
    ->Arg(10)
    ->Arg(25)
    ->Arg(50)
    ->Arg(100)
    ->Arg(1000)
    ->Arg(5000)
    ->Arg(10000)
    ->Unit(::benchmark::TimeUnit::kMillisecond);

BENCHMARK_MAIN();