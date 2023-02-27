#pragma once

#include <string>
#include <stdexcept>

namespace wellnmea
{
  /* -------------------------------------------------------------------------- */
  /*                              Class definition                              */
  /* -------------------------------------------------------------------------- */
  class field
  {
  private:
    /* -------------------------------------------------------------------------- */
    /// @brief Field internal name
    std::string m_name;
    /* -------------------------------------------------------------------------- */
    /// @brief Field position in sentence
    int m_pos;
    /* -------------------------------------------------------------------------- */
    /// @brief Required field value marker
    bool m_required = false;

  public:
    /* -------------------------------------------------------------------------- */
    explicit field(
        const std::string &name,
        unsigned int position) noexcept(false) : m_name(name),
                                          m_pos(position)
    {
      if (m_name.empty())
      {
        throw std::invalid_argument("Invalid field name provided to the field constructor");
      }
      if (m_pos < 1)
      {
        throw std::invalid_argument("Invalid field position provided to the field constructor");
      }
    };
    /* -------------------------------------------------------------------------- */
    ~field();
    /* -------------------------------------------------------------------------- */
  public:
    /* --------------------------------- Methods -------------------------------- */
    /// @brief Returns the name of expected field
    /// @return Name of field
    std::string fieldName() const noexcept;
    /* -------------------------------------------------------------------------- */
    int position() const noexcept;
    /* -------------------------------------------------------------------------- */
    bool required() const noexcept;
    /* -------------------------------------------------------------------------- */
    void markRequired() noexcept;
    /* -------------------------------------------------------------------------- */
  };
  /* -------------------------------------------------------------------------- */
  /*                               Implementation                               */
  /* -------------------------------------------------------------------------- */
  field::~field()
  {
  }
  /* -------------------------------------------------------------------------- */
  std::string field::fieldName() const noexcept
  {
    return m_name;
  }
  /* -------------------------------------------------------------------------- */
  int field::position() const noexcept
  {
    return m_pos;
  }
  /* -------------------------------------------------------------------------- */
  bool field::required() const noexcept
  {
    return m_required;
  }
  /* -------------------------------------------------------------------------- */
  void field::markRequired() noexcept
  {
    m_required = true;
  }
} // namespace wellnmea
