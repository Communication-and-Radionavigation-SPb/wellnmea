#pragma once

#include <memory>
#include <type_traits>

#include <wellnmea/instructions/instruction.hpp>
#include <wellnmea/formats/instructions_registry.hpp>


namespace wellnmea
{
  using namespace std;
  using namespace wellnmea::instructions;

  template<class... IList>
  struct InstructionsRegistrar;
  template<class I, class... IList>
  struct InstructionsRegistrar<I, IList...> {
    static void use() {
      static_assert(is_base_of_v<Instruction, I>, "I must be derived from Instruction class");
      InstructionsRegistry::add(make_shared<I>(""));
      InstructionsRegistrar<IList...> r;
      r.use();
    }
  };

  template<>
  struct InstructionsRegistrar<>{
    static void use() {}
  };
}