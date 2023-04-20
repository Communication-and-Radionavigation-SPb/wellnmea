#pragma once

#include <list>
#include <wellnmea/formats/instruction.hpp>
#include <wellnmea/values/repeated.hpp>

namespace wellnmea
{
  namespace formats
  {
    using std::list;
    using namespace wellnmea::values;


    /**
     * @brief Internal instruction for variadic part of NMEA message
     * 
     * Internal instruction for variadic part of NMEA message
     * should not be registered.
     * 
     */
    class RepeatedInstruction : public Instruction
    {
    public:
      using Subfield = std::shared_ptr<Instruction>;

    private:
      list<Subfield> m_subfields;

    public:
      RepeatedInstruction(const std::string &name,
                          const list<Subfield> &subfields) : Instruction(name),
                                                             m_subfields(subfields) {}

    public:
      std::string which() const noexcept override
      {
        return "repetition";
      }

      Instruction *clone(const std::string &name) const override
      {
        list<Subfield> cloned;

        std::transform(
            m_subfields.begin(),
            m_subfields.end(),
            std::back_inserter(cloned),
            [](const Subfield &s)
            { return Subfield(s->clone(s->name())); });

        return new RepeatedInstruction(name, cloned);
      }

      value *extract(position it, const_position end) override
      {
        RepeatedValue::GroupList l{};

        while (it != end && it->type != Token::checksum)
        {
          subextract(it, end, l);
        }

        return new values::RepeatedValue(name(), l);
      }

    protected:
      void subextract(position it, const_position end, RepeatedValue::GroupList &l)
      {
        RepeatedValue::Group *g = new RepeatedValue::Group;
        NullValue *v = nullptr;
        for (auto &&i : m_subfields)
        {
          v = i->extract(it, end);
          g->push_back(v);
        }
        l.push_back(g);
      }
    };
  } // namespace formats

} // namespace wellnmea
