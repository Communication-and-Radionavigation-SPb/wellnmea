#include <benchmark/benchmark.h>
#include <vector>
#include <list>
#include <string>

std::string gen_random(const int len)
{
  static const char alphanum[] =
      "0123456789"
      "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
      "abcdefghijklmnopqrstuvwxyz";
  std::string tmp_s;
  tmp_s.reserve(len);

  for (int i = 0; i < len; ++i)
  {
    tmp_s += alphanum[rand() % (sizeof(alphanum) - 1)];
  }

  return tmp_s;
}

static void BM_ListWithString(benchmark::State &state)
{

  std::string message = gen_random(state.range(0));
  for (auto _ : state)
  {
    std::list<std::string> ls;
    ls.push_back(message);
  }

  state.SetBytesProcessed(
      int64_t(state.iterations()) * int64_t(state.range(0)));
}

static void BM_VectorWithString(benchmark::State &state)
{
  std::string message = gen_random(state.range(0));
  for (auto _ : state)
  {
    std::vector<std::string> v;
    v.push_back(message);
  }

  state.SetBytesProcessed(
      int64_t(state.iterations()) * int64_t(state.range(0)));
}

static void BM_ListWithStringView(benchmark::State &state)
{
  std::string message_str = gen_random(state.range(0));
  std::string_view message(message_str.c_str(), message_str.size());
  for (auto _ : state)
  {
    std::list<std::string_view> ls;
    ls.push_back(message);
  }

  state.SetBytesProcessed(
      int64_t(state.iterations()) * int64_t(state.range(0)));
}

static void BM_VectorWithStringView(benchmark::State &state)
{
  std::string message_str = gen_random(state.range(0));
  std::string_view message(message_str.c_str(), message_str.size());
  for (auto _ : state)
  {
    std::vector<std::string_view> ls;
    ls.push_back(message);
  }

  state.SetBytesProcessed(
      int64_t(state.iterations()) * int64_t(state.range(0)));
}

BENCHMARK(BM_ListWithString)->Range(8, 8 << 10);
BENCHMARK(BM_VectorWithString)->Range(8, 8 << 10);

BENCHMARK(BM_ListWithStringView)->Range(8, 8 << 10);
BENCHMARK(BM_VectorWithStringView)->Range(8, 8 << 10);

BENCHMARK_MAIN();