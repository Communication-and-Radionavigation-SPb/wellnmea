#pragma once

#include <wellnmea/formats/instruction.hpp>
#include <wellnmea/values/checksum.hpp>
#include <wellnmea/util/token_t_utils.hpp>

namespace wellnmea
{
  namespace formats
  {
    class ChecksumInstruction : public Instruction
    {
      bool m_required;

    public:
      ChecksumInstruction(
          const std::string &name,
          bool required = false) : Instruction(name), m_required(required) {}

    private:
      bool validate(position it, const_position end) const
      {
        if (it != end && !it->slice.empty())
        {
          return true;
        }
        return m_required && (throw extraction_error("Checksum marked as required but "
                                                     "provided message do not contains that field."),
                              false);
      }

    public:
      Instruction *applyParams(props params) override
      {
        // ! Do nothing if params are empty
        if (params.empty())
          return this;

        auto required_pos_param = params[0] == "true" ? true : false;

        m_required = required_pos_param;

        return this;
      }

      std::string which() const noexcept override
      {
        return "checksum";
      }

      Instruction *clone(const std::string &name) const override
      {
        return new ChecksumInstruction(name, m_required);
      }

      value *extract(position it, const_position end) override
      {
        using wellnmea::values::ChecksumValue;
        using wellnmea::values::NullChecksumValue;

        if (!validate(it, end))
        {
          return new NullChecksumValue(name());
        }

        if (!util::token::isChecksum(it->type))
          throw extraction_error("Failed to extract checksum value near `" + it->slice + "`");
        int checksum = 0;
        string significant = it->slice.substr(1, 2);
        try
        {
          checksum = (int)std::stol(significant, NULL, 16);
        }
        catch (const std::invalid_argument &e)
        {
          throw extraction_error("Failed to extract checksum "
                                 "value near `" +
                                 it->slice +
                                 "`.");
        }
        ++it;
        return new ChecksumValue(name(), checksum);
      }
    };
  } // namespace formats

} // namespace wellnmea
