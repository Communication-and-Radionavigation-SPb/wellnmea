#pragma once

#include <cctype>
#include <string>
#include <stack>
#include <list>
#include <utility>
#include <algorithm>

namespace wellnmea
{
  namespace util
  {
    using namespace std;
    using Enclosures = list<pair<char, char>>;

    inline string to_lower(string &source) noexcept
    {
      string clone(source);
      transform(source.begin(), source.end(), clone.begin(), [](unsigned char c)
                { return tolower(c); });
      return clone;
    }

    inline int checksum0183(const std::string &source)
    {
      if (source.empty())
        return 0;

      int crc = 0;
      int length = source.length() - 3;

      for (int i = 1; i < length; i++)
      {
        crc ^= source[i];
      }

      return crc;
    }

    inline string::const_iterator
    punct_position(list<char> enclosures,
                   string::const_iterator start, string::const_iterator end) noexcept
    {
      for (auto it = start; it < end; it++)
      {
        auto o = find_if(enclosures.begin(), enclosures.end(), [it](const char &p)
                         { return p == *it; });
        if (o != enclosures.end())
        {
          return it;
        }
      }
      return end;
    }

    inline string::const_iterator
    border_sign(const char &symbol,
                Enclosures enclosures,
                string::const_iterator start,
                string::const_iterator end) noexcept
    {
      stack<Enclosures::iterator> st;
      for (auto it = start; it != end; it++)
      {
        auto o = find_if(enclosures.begin(), enclosures.end(), [it](const pair<char, char> &p)
                         { return p.first == *it; });
        if (o != enclosures.end())
        {
          st.push(o);
        }
        else if (!st.empty() && st.top()->second == *it)
        {
          st.pop();
        }
        else if (st.empty() && *it == symbol)
          return it;
      }
      return end;
    }

    inline string::const_iterator
    extract_between(
        const char &opening,
        const char &closing,
        string::const_iterator start,
        string::const_iterator end) noexcept
    {
      int st = 0;
      for (auto it = start; it != end; it++)
      {
        if (*it == opening)
        {
          st++;
        }
        else if (*it == closing)
        {
          if (--st == 0)
          {
            return it;
          }
        }
      }
      return end;
    }
  } // namespace util

} // namespace wellnmea
