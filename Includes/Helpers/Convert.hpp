#ifndef CONVERT_HPP
#define CONVERT_HPP

#include "CTRPluginFramework.hpp"

namespace CTRPluginFramework
{
  class Convert
  {
  public:
    static u16 sjisToUtf16(u16 sjis);
    static u16 utf16ToSjis(u16 utf16);
    static u16 strToSjis(std::string moji);
    static std::string hiraganaToKanji(std::string hiragana);
  };
}

#endif