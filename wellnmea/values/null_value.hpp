#pragma once

#include <string>
#include <map>
#include <variant>
#include <optional>

namespace wellnmea
{
  namespace values
  {
    class NullValue
    {
    public:
      using SerializedValue = std::variant<double, long, std::string>;
      using SerializedProperty = std::map<std::string, SerializedValue>;
      using SerializedResult = std::optional<SerializedProperty>;

    private:
      std::string m_name;

    public:
      /**
       * @brief Construct a new BaseValue object
       *
       * @param a_name Name of parameter
       */
      NullValue(const std::string &a_name) : m_name(a_name){};
      virtual ~NullValue() = default;

    public:
      /**
       * @brief Name of parameter
       *
       * @return std::string_view
       */
      std::string_view name() const noexcept
      {
        return m_name;
      }

      virtual SerializedResult serialise() const noexcept = 0;

      template <class ConsiderAs>
      ConsiderAs *as()
      {
        return dynamic_cast<ConsiderAs *>(this);
      }
    };

  } // namespace params

} // namespace wellnmea
