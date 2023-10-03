#include <benchmark/benchmark.h>

#include <vector>
#include <string>

class SymbolInstructionExtraction : public benchmark::Fixture
{
  public:
    SymbolInstructionExtraction() {}
  public:
    std::vector<std::string> sources;
  public:
  void SetUp(const ::benchmark::State &state)
  {
    sources = std::vector<std::string>(state.range(0));

    for (size_t i = 0; i < sources.size(); i++)
    {
      sources[i] = generate();
    }
  }

  std::string generate()
  {
    return "";
  }
};



BENCHMARK_MAIN();