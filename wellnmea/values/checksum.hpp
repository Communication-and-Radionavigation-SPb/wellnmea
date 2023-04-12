#pragma once
#include <string>
#include <optional>
#include <wellnmea/values/null_value.hpp>

using std::optional;
using std::string;

namespace wellnmea
{
  namespace values
  {
    class _ChecksumValue : public NullValue
    {

    public:
      _ChecksumValue(const string name) : NullValue(name) {}

      virtual ~_ChecksumValue() = default;

    public:
      virtual optional<int> value() const noexcept = 0;

      virtual void accept(visitor_base &v) const noexcept override
      {
        using value_visitor = visitor<_ChecksumValue>;
        value_visitor *ev = dynamic_cast<value_visitor *>(&v);

        if (ev != nullptr)
        {
          ev->visit(this);
        }
      }
    };

    class ChecksumValue : public _ChecksumValue
    {
    private:
      int _value;

    public:
      ChecksumValue(const string &name, int checksum) : _value(checksum), _ChecksumValue(name) {}

    public:
      optional<int> value() const noexcept override
      {
        return _value;
      }
    };

    class NullChecksumValue : public _ChecksumValue
    {
    public:
      NullChecksumValue(const string &name) : _ChecksumValue(name) {}

    public:
      optional<int> value() const noexcept override
      {
        return std::nullopt;
      }
    };
  } // namespace values

} // namespace wellnmea
