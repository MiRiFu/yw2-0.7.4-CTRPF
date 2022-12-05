#ifndef KANICODES_HPP
#define KANICODES_HPP

#include <CTRPluginFramework.hpp>
#include <math.h>
#include "types.h"

namespace CTRPluginFramework
{
  //osd_plus

  void DrawLine(const Screen &scr, int srcX, int srcY, int dstX, int dstY, const Color &color);
  void DrawPlus(const Screen &scr, const std::string &str, u32 posX, u32 posY, u32 borderWidth, u32 padding, const Color &foreground, const Color &background, const Color &border, int fontAlign);
  void DrawSysfontPlus(const Screen &scr, const std::string &str, u32 posX, u32 posY, u32 borderWidth, u32 padding, const Color &foreground, const Color &background, const Color &border, bool fillBackground, bool rightAligned = false, int fontAlign = 0);
  float DegreeToRadian(float degree);
  void DrawCircle(const Screen &scr, u32 x, u32 y, u32 radiusStart, u32 radiusEnd, int start, int end, const Color &color);

  class Convert
  {
  public:
    static u16 sjisToUtf16(u16 sjis);
    static u16 utf16ToSjis(u16 utf16);
    static u16 strToSjis(std::string moji);
    static std::string hiraganaToKanji(std::string hiragana);
  };

  bool TouchCircle(u32 x, u32 y, u8 size);
  bool TouchRect(u32 x, u32 y, u32 w, u32 h);
}

#endif