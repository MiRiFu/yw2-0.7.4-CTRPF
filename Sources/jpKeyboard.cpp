#include "cheats.hpp"
#include <math.h>
#include "../libctrpf/include/CTRPluginFrameworkImpl/Menu/KeyboardImpl.hpp"

namespace CTRPluginFramework
{
  u16 U16_ChrArray[50 + 1];
  bool KatakanaMode;
  bool KeyboardOpened;

  u8 selectedIndex;

  std::vector<u16> InputChrs;
  std::string InputStr;

  const std::string Hiragana =
      "わらやまはなたさかあ"
      "をりゆみひにちしきい"
      "んるよむふぬつすくう"
      "、れ！めへねてせけえ"
      "。ろ？もほのとそこお";

  const std::string Katakana =
      "ワラヤマハナタサカア"
      "ヲリユミヒニチシキイ"
      "ンルヨムフヌツスクウ"
      "、レ！メヘネテセケエ"
      "。ロ？モホノトソコオ";

  void MakeU16Array()
  {
    Process::WriteString((u32)U16_ChrArray, KatakanaMode ? Katakana : Hiragana, StringFormat::Utf16);
  }

  void Komoji(u16 &moji, std::vector<u8> &sjis)
  {
    // ひらがな・カタカナじゃなければ帰る
    if (!(moji >= 0x3042 && moji <= 0x3093) && !(moji >= 0x30A2 && moji <= 0x30F3))
      return;

    const std::string str1 = "あいうえおアイウエオわワカつツやゆよヤユヨ";
    const std::string str2 = "ぁぃぅぇぉァィゥェォゎヮヵっッゃゅょャュョ";

    // UTF16文字配列
    u16 u16array1[100] = {0}; // 変換前
    u16 u16array2[100] = {0}; // 変換後

    Process::WriteString((u32)u16array1, str1, StringFormat::Utf16);
    Process::WriteString((u32)u16array2, str2, StringFormat::Utf16);

    for (int i = 0; i < 22; i++)
    {
      if (moji == u16array1[i])
      {
        moji = u16array2[i];
        sjis.pop_back();
        sjis.pop_back();
        u16 buff_utf16;
        buff_utf16 = Convert::utf16ToSjis(u16array2[i]);
        sjis.push_back(buff_utf16 / 0x100);
        sjis.push_back(buff_utf16 & 0xFF);
      }
      else if (moji == u16array2[i])
      {
        moji = u16array1[i];
        sjis.pop_back();
        sjis.pop_back();
        u16 buff_utf16;
        buff_utf16 = Convert::utf16ToSjis(u16array1[i]);
        sjis.push_back(buff_utf16 / 0x100);
        sjis.push_back(buff_utf16 & 0xFF);
      }
    }
  }

  void Dakuten(bool handakuten, u16 &moji, std::vector<u8> &sjis)
  {
    // ひらがな・カタカナじゃなければ帰る
    if (!(moji >= 0x3042 && moji <= 0x3093) && !(moji >= 0x30A2 && moji <= 0x30F3))
      return;

    // 濁点用
    const std::string str1 = "かきくけこさしすせそたちつてとはひふへほ";
    const std::string str2 = "がぎぐげござじずぜぞだぢづでどばびぶべぼ";

    // 半濁点用
    const std::string str3 = "はひふへほ";
    const std::string str4 = "ぱぴぷぺぽ";

    // UTF16文字配列
    u16 u16array1[100] = {0}; // 変換前
    u16 u16array2[100] = {0}; // 変換後

    // moji = カタカナ
    bool isKatakana = moji >= 0x30A2 && moji <= 0x30F3;

    if (handakuten) // 半濁点モード
    {
      Process::WriteString((u32)u16array1, str3, StringFormat::Utf16);
      Process::WriteString((u32)u16array2, str4, StringFormat::Utf16);
    }
    else // 濁点モード
    {
      Process::WriteString((u32)u16array1, str1, StringFormat::Utf16);
      Process::WriteString((u32)u16array2, str2, StringFormat::Utf16);
    }

    // 変換
    for (int i = 0; i < (handakuten ? 5 : 20); i++)
    {
      if (isKatakana) // カタカナだったら変換用配列をカタカナにする
      {
        u16array1[i] += 0x60;
        u16array2[i] += 0x60;
      }

      // ヒットしたら書き換えて終了
      if (u16array1[i] == moji)
      {
        moji = u16array2[i];
        sjis.pop_back();
        sjis.pop_back();
        u16 buff_utf16;
        buff_utf16 = Convert::utf16ToSjis(u16array2[i]);
        sjis.push_back(buff_utf16 / 0x100);
        sjis.push_back(buff_utf16 & 0xFF);
        break;
      }
      else if (u16array2[i] == moji)
      {
        moji = u16array1[i];
        sjis.pop_back();
        sjis.pop_back();
        u16 buff_utf16;
        buff_utf16 = Convert::utf16ToSjis(u16array1[i]);
        sjis.push_back(buff_utf16 / 0x100);
        sjis.push_back(buff_utf16 & 0xFF);
        break;
      }
    }
  }

  void DrawKeyboard(const Screen &scr, std::string &out, std::vector<u8> &sjis)
  {
    MakeU16Array();

    // 背景
    scr.DrawRect(20, 20, 280, 200, Color::Black);
    scr.DrawRect(22, 22, 276, 196, Color::White, false);
    // scr.DrawRect(40, 68, 240, 110, Color(222, 170, 99));

    // 入力
    if (Controller::IsKeyPressed(Touchpad))
    {
      UIntVector pos = Touch::GetPosition();
      if (pos.x >= 23 && pos.y >= 69 && pos.x <= 262 && pos.y <= 178)
      {
        selectedIndex = 0;
        int wx = pos.x - 23;
        int wy = pos.y - 69;
        wx /= 24;
        wy /= 22;
        scr.DrawRect(23 + wx * 24, 68 + wy * 22, 24, 22, Color::White);

        if (InputChrs.size() < 31)
        {
          InputChrs.push_back(U16_ChrArray[wy * 10 + wx]);
          u16 buff_utf16;
          buff_utf16 = Convert::utf16ToSjis(U16_ChrArray[wy * 10 + wx]);
          sjis.push_back(buff_utf16 / 0x100);
          sjis.push_back(buff_utf16 & 0xFF);
        }
      }
    }

    // キー描画
    std::string key;
    for (u8 x = 0; x < 10; x++)
      for (u8 y = 0; y < 5; y++)
      {
        u32 sx = 23 + x * 24;
        u32 sy = 68 + y * 22;

        // 文字
        key.clear();
        Process::ReadString((u32)&U16_ChrArray[y * 10 + x], key, 2, StringFormat::Utf16);
        scr.DrawSysfont(key, sx + 5, sy + 3, Color::White);
      }

    // 入力中の文字列描画
    scr.DrawRect(55, 56, 212, 1, Color::White);
    if (!InputChrs.empty())
    {
      InputStr.clear();
      u16 str[100] = {0};
      for (int i = 0; i < InputChrs.size(); i++)
      {
        str[i] = InputChrs[i];
      }
      Process::ReadString((u32)str, InputStr, InputChrs.size() * 2, StringFormat::Utf16);
      u8 i = 0;
      u8 j = 0;
      u16 width = OSD::GetTextWidth(true, InputStr);
      while (1)
      {
        if (width <= 208)
          break;
        if (InputChrs[j] < 0x1000)
        {
          i++;
        }
        else
        {
          i += 3;
        }
        j++;
        width = OSD::GetTextWidth(true, InputStr.substr(i, InputStr.length() - i));
      }
      scr.DrawRect(58 + width - selectedIndex * 13, 37, selectedIndex * 13, 17, Color::Blue);
      scr.DrawSysfont(InputStr.substr(i, InputStr.length() - i), 58, 38);
    }

    // 右側のオプション
    const std::string opt[5] = {"削除", "小字", "ー", "”", "゜"};
    for (int i = 0; i < 5; i++)
    {
      if (Controller::IsKeyPressed(Touchpad) && TouchRect(268, 72 + i * 22, 24, 16))
      {
        selectedIndex = 0;
        switch (i)
        {
        case 0:
          if (!InputChrs.empty())
          {
            sjis.pop_back();
            if (InputChrs.back() > 0x100)
            {
              sjis.pop_back();
            }
            InputChrs.pop_back();
          }
          break;
        case 1:
          Komoji(InputChrs[InputChrs.size() - 1], sjis);
          break;
        case 2:
          if (InputChrs.size() < 31)
          {
            InputChrs.push_back(0x30FC);
            sjis.push_back(0x81);
            sjis.push_back(0x5B);
          }
          break;
        case 3:
          Dakuten(false, InputChrs[InputChrs.size() - 1], sjis);
          break;
        case 4:
          Dakuten(true, InputChrs[InputChrs.size() - 1], sjis);
          break;
        }
        scr.DrawRect(263, 68 + i * 22, 34, 22, Color::White);
      }
      scr.DrawSysfont(opt[i], 268, 72 + i * 22);
    }

    //選択
    scr.DrawSysfont("<", 35, 35);
    scr.DrawSysfont(">", 277, 35);
    if (Controller::IsKeyPressed(Touchpad) && TouchRect(32, 32, 24, 22))
    {
      scr.DrawRect(32, 35, 17, 17, Color::White);
      std::vector<u16> lastN(InputChrs.end() - selectedIndex - 1, InputChrs.end());
      if (lastN[0] > 0x1000)
      {
        selectedIndex++;
      }
    }
    if (Controller::IsKeyPressed(Touchpad) && TouchRect(274, 32, 24, 22))
    {
      scr.DrawRect(274, 35, 17, 17, Color::White);
      if (selectedIndex != 0)
      {
        selectedIndex--;
      }
    }

    if (Controller::IsKeyPressed(Key::Y) && selectedIndex != 0)
    {
      std::string kanji = Convert::hiraganaToKanji(InputStr.substr(InputStr.length() - selectedIndex * 3, selectedIndex * 3));
      if (!kanji.empty())
      {
        for (int j = 0; j < selectedIndex; j++)
        {
          InputChrs.pop_back();
          sjis.pop_back();
          sjis.pop_back();
        }
        if (InputChrs.size() < 31)
        {
          u8 k = 0, i = 0;
          u16 buff_utf16[100] = {0};
          Process::WriteString((u32)buff_utf16, kanji, StringFormat::Utf16);
          i = 0;
          selectedIndex = 0;
          while (1)
          {
            if (buff_utf16[i] == 0)
              break;
            if (buff_utf16[i] < 0x1000)
              selectedIndex = 0;
            else
              selectedIndex++;
            i++;
          }
          for (int j = 0; j < i; j++)
          {
            u16 buff_sjis;
            if (buff_utf16[j] > 0x1000)
            {
              buff_sjis = Convert::strToSjis(kanji.substr(k, 3));
              k += 3;
              sjis.push_back(buff_sjis / 0x100);
              sjis.push_back(buff_sjis & 0xFF);
            }
            else
            {
              buff_sjis = Convert::strToSjis(kanji.substr(k, 1));
              sjis.push_back(buff_sjis & 0xFF);
              k++;
            }
            InputChrs.push_back(buff_utf16[j]);
          }
        }
      }
    }

    // とじる
    scr.DrawRect(28, 191, 68, 22, Color::Gray);
    scr.DrawRect(28, 191, 68, 22, Color::White, false);
    scr.DrawSysfont("とじる", 42, 194);
    if (Controller::IsKeyPressed(B) || TouchRect(28, 191, 68, 22))
    {
      sjis.clear();
      KeyboardOpened = false;
    }

    // モード変換
    scr.DrawRect(126, 191, 68, 22, Color::Gray);
    scr.DrawRect(126, 191, 68, 22, Color::White, false);
    scr.DrawSysfont("レイアウト", 126, 194);
    if (Controller::IsKeyPressed(Touchpad) && TouchRect(126, 191, 68, 22))
    {
      if (KatakanaMode)
      {
        std::string input;
        KeyboardImpl a("");
        a.SetLayout(Layout::QWERTY);
        a.Run();
        input = a.GetInput();
        for (int i = 0; i < input.length(); i++)
        {
          if (InputChrs.size() > 31)
          {
            break;
          }
          u16 buff;
          Process::WriteString((u32)&buff, input.substr(i, 1), StringFormat::Utf16);
          InputChrs.push_back(buff);
          buff = Convert::strToSjis(input.substr(i, 1));
          sjis.push_back(buff & 0xFF);
          selectedIndex = 0;
        }
      }
      KatakanaMode = KatakanaMode ? false : true;
    }

    // 決定
    scr.DrawRect(224, 191, 68, 22, Color::Gray);
    scr.DrawRect(224, 191, 68, 22, Color::White, false);
    scr.DrawSysfont("決定", 246, 194);
    if (TouchRect(224, 191, 68, 22))
    {
      if (!InputStr.empty())
      {
        out = InputStr;
        KeyboardOpened = false;
      }
    }
  }

  void japKey(std::string &out, std::vector<u8> &sjis)
  {
    if (!Process::IsPaused())
      return;

    InputChrs.clear();
    InputStr.clear();
    sjis.clear();

    const Screen &scr = OSD::GetBottomScreen();

    KeyboardOpened = true;
    KatakanaMode = false;
    while (KeyboardOpened)
    {
      Controller::Update();
      DrawKeyboard(scr, out, sjis);
      OSD::SwapBuffers();
    }
  }
}