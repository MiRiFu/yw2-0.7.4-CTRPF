#include "cheats.hpp"
#include "osd_plus.hpp"
#include "osdjp.hpp"
#include "AliceCodes.hpp"
#include "../libctrpf/include/CTRPluginFrameworkImpl/System/ProcessImpl.hpp"
#include "../libctrpf/include/CTRPluginFrameworkImpl/Menu/KeyboardImpl.hpp"

namespace CTRPluginFramework
{
  bool TouchRect(u32 x, u32 y, u32 w, u32 h)
  {
    if (Touch::IsDown())
    {
      UIntVector pos = Touch::GetPosition();
      if (pos.x >= x && pos.y >= y && pos.x <= (x + w) && pos.y <= (y + h))
        return true;
    }
    return false;
  }

  bool TouchCircle(u32 x, u32 y, u8 size)
  {
    u32 rectLength = (size * 2) / 1.41421356237;
    u32 miniRadius = rectLength / 2;

    u32 rectX = x - miniRadius;
    u32 rectY = y - miniRadius;
    if (TouchRect(rectX, rectY, rectLength, rectLength))
      return true;

    UIntVector pos = Touch::GetPosition();
    for (int r = miniRadius; r < size; r++)
    {
      for (int angle = 0; angle < 360; angle++)
      {
        if (pos.x == x + cos(DegreeToRadian(angle)) * r && pos.y == y + sin(DegreeToRadian(angle)) * r)
          return true;
      }
    }
    return false;
  }

  void Test1(MenuEntry *entry)
  {
  }

  void JPNotify(MenuEntry *entry)
  {
    StringVector words = {"コ", "ン", "ニ", "チ", "ワ"};
    if (Controller::IsKeyDown(Key::Start) && Controller::IsKeyDown(Key::Y))
      words = {"オ", "チ", "ン", "チ", "ン"};
    if (Controller::IsKeyPressed(Key::Start))
    {
      std::string buff;
      for (int j = 0; j < 5; j++)
      {
        buff += words[Utils::Random(0, 4)];
      }
      u8 red = Utils::Random(0, 255), green = Utils::Random(0, 255), blue = Utils::Random(0, 255);
      u16 sum = std::max({red, green, blue}) + std::min({red, green, blue});
      OSDJPNotify(buff, Color(red, green, blue), Color(sum - red, sum - green, sum - blue));
    }
  }

  void Cheat1(MenuEntry *entry)
  {
    u8 size, mapX, mapY;
    u32 offset = 0x08001380;
    float posX, posY, diffX, diffY, touchX, touchY, timer;
    Process::Read8(0x08852342, size);
    Process::ReadFloat(0x08852E10, posX);
    Process::ReadFloat(0x08852E18, posY);
    // offset = 0x08001380;
    // Process::Read32(offset + 0x0000000, offset);
    // Process::Read8(offset + 0x000B2CA, mapX);
    // Process::Read8(offset + 0x000B2CE, mapY);
    mapX = 155;
    mapY = 120;
    auto position = Touch::GetPosition();
    if (0 < position.x && position.x < 300 && 20 < position.y && position.y < 200)
    {
      touchX = position.x;
      touchY = position.y;
      diffX = (touchX - mapX) * (size + 1) / 50;
      diffY = (touchY - mapY) * (size + 1) / 50;
      Process::WriteFloat(0x08852E10, posX + diffX);
      Process::WriteFloat(0x08852E18, posY + diffY);
    }

    const Screen &screen = OSD::GetTopScreen();
    screen.Draw(Utils::Format("%x", size), 0, 0);
    screen.Draw(Utils::Format("%f", diffX), 30, 0);
    screen.Draw(Utils::Format("%f", diffX), 120, 0);
    screen.Draw(Utils::Format("%f", posX), 0, 10);
    screen.Draw(Utils::Format("%f", posY), 0, 20);
    screen.Draw(Utils::Format("%x", mapX), 0, 30);
    screen.Draw(Utils::Format("%x", mapY), 10, 30);
    screen.Draw(Utils::Format("%x", offset + 0x000B2CA), 0, 40);
  }

  u32 xPos = 150, yPos = 120;
  u8 direct = 3, temp_direct, buff_direct, i = 0, len, j;
  Color screenColor[400][240];
  Color colorList[6] = {Color::Red, Color::Blue, Color::Cyan, Color::Green, Color::Yellow, Color::Orange};
  bool isReset = false;
  void Pipes(MenuEntry *entry)
  {
    const Screen &screen = OSD::GetTopScreen();
    if (isReset)
    {
      switch (direct)
      {
      case 0:
        xPos = Utils::Random(0, 398) + 1;
        yPos = 1;
        break;
      case 1:
        xPos = 1;
        yPos = Utils::Random(0, 338) + 1;
        break;
      case 2:
        xPos = 399;
        yPos = Utils::Random(0, 238) + 1;
        break;
      default:
        xPos = Utils::Random(0, 398) + 1;
        yPos = 239;
        break;
      }
      isReset = false;
      j = 0;
      if (i < 5)
      {
        i++;
      }
      else
      {
        i = 0;
      }
    }
    if (len > 0)
    {
      if (xPos == 0 || xPos == 400 || yPos == 0 || yPos == 240)
      {
        isReset = true;
      }
      else
      {
        // screen.DrawRect(xPos, yPos, 3, 3, Color::SkyBlue);
        for (int k = -1; k < 2; k++)
        {
          screenColor[xPos + k][yPos + k] = colorList[i];
        }
        switch (direct)
        {
        case 0:
          if (j == 0)
          {
            if (buff_direct == 1)
            {
              screenColor[xPos + 2][yPos] = Color::White;
            }
            else
            {
              screenColor[xPos - 2][yPos] = Color::White;
            }
            yPos++;
            break;
          }
          screenColor[xPos + 2][yPos] = Color::White;
          screenColor[xPos - 2][yPos] = Color::White;
          yPos++;
          break;
        case 1:
          if (j == 0)
          {
            if (buff_direct == 0)
            {
              screenColor[xPos][yPos + 2] = Color::White;
            }
            else
            {
              screenColor[xPos][yPos - 2] = Color::White;
            }
            xPos++;
            break;
          }
          screenColor[xPos][yPos + 2] = Color::White;
          screenColor[xPos][yPos - 2] = Color::White;
          xPos++;
          break;
        case 2:
          if (j == 0)
          {
            if (buff_direct == 0)
            {
              screenColor[xPos][yPos + 2] = Color::White;
            }
            else
            {
              screenColor[xPos][yPos - 2] = Color::White;
            }
            xPos--;
            break;
          }
          screenColor[xPos][yPos + 2] = Color::White;
          screenColor[xPos][yPos - 2] = Color::White;
          xPos--;
          break;
        default:
          if (j == 0)
          {
            if (buff_direct == 1)
            {
              screenColor[xPos + 2][yPos] = Color::White;
            }
            else
            {
              screenColor[xPos - 2][yPos] = Color::White;
            }
            yPos--;
            break;
          }
          screenColor[xPos + 2][yPos] = Color::White;
          screenColor[xPos - 2][yPos] = Color::White;
          yPos--;
          break;
        }
        len--;
      }
    }
    else
    {
      len = Utils::Random(0, 30);
      j = 0;
      while (1)
      {
        temp_direct = Utils::Random(0, 3);
        if ((temp_direct == 0 && direct != 3) || (temp_direct == 1 && direct != 2) || (temp_direct == 2 && direct != 1) || (temp_direct == 3 && direct != 0))
        {
          break;
        }
      }
      buff_direct = direct;
      direct = temp_direct;
    }
    for (u32 i = 0; i < 400; i++)
    {
      for (u32 j = 0; j < 240; j++)
      {
        if ((screenColor[i][j] != Color::White) && (screenColor[i][j] != 255))
        {
          screen.DrawPixel(i, j, screenColor[i][j]);
        }
      }
    }
    if (Controller::IsKeyPressed(Key::Start))
    {
      isReset = true;
      for (int i = 0; i < 400; i++)
      {
        for (int j = 0; j < 240; j++)
        {
          screenColor[i][j] = Color::White;
        }
      }
    }
  }

  std::vector<std::vector<std::string>> MenuEntryNameList = {{"pipes", "ぱいぷす", "パイプス", "Pipes"}, {"yokaieditor", "ようかいえでぃたー", "ヨウカイエディター", "YokaiEditor"}, {"cube", "きゅーぶ", "キューブ", "Cube"}, {"bad apple!!", "ばっどあっぷる！！", "バッドアップル！！", "Bad Apple!!"}, {"jpnotify", "じぇーぴーにほんごのてぃふぁい", "ジェーピーニホンゴノティファイ", "JPNotify"}};
  FuncPointer MenuEntryFuncList[] = {Pipes, YokaiEditor, Cube, BadApple, JPNotify};
  void Search(MenuEntry *entry)
  {
    std::string input;
    std::vector<u8> entryNums;
    std::vector<u8> sjis;
    japKey(input, sjis);
    if (input.empty())
      return;
    std::transform(input.cbegin(), input.cend(), input.begin(), tolower);
    for (int i = 0; i < MenuEntryNameList.size(); i++)
    {
      for (int j = 0; j < MenuEntryNameList[i].size(); j++)
      {
        if (MenuEntryNameList[i][j].find(input) != std::string::npos)
        {
          entryNums.push_back(i);
          break;
        }
      }
    }
    std::vector<std::string> options;
    for (int i = 0; i < entryNums.size(); i++)
    {
      options.push_back(MenuEntryNameList[entryNums[i]][3]);
    }
    Keyboard key("which do you want:", options);
    int choice = key.Open();
    switch (choice)
    {
    case -1:
      break;
    default:
      entry->Name() = MenuEntryNameList[entryNums[choice]][3] + "(search)";
      entry->SetGameFunc(MenuEntryFuncList[entryNums[choice]]);
      break;
    }
  }

  bool isOpened, afterKeyboard;
  u8 afterKeyboardSec;
  void YokaiEditor(MenuEntry *entry)
  {
    u8 level, leaderIndex, nickname[32];
    u16 hp, power, magic, protect, speed;
    u32 yokaiID, offset;
    std::string nickname_str;
    Process::Read8(0x8A4BD20, leaderIndex);
    offset = leaderIndex * 0xC4;
    const Screen &topScr = OSD::GetTopScreen();
    const Screen &btmScr = OSD::GetBottomScreen();
    if ((Controller::IsKeyPressed(Key::Start)) || (afterKeyboardSec > 2))
    {
      if ((isOpened) && (afterKeyboardSec < 2))
      {
        Process::Play();
        isOpened = false;
      }
      else
      {
        Process::Read8(0x870DC03 + offset, level);
        for (int i = 0; i < 32; i++)
        {
          Process::Read8(0x870DBBC + offset + i, nickname[i]);
        }
        Process::Read16(0x870DBEC + offset, hp);
        Process::Read16(0x870DC16 + offset, power);
        Process::Read16(0x870DC18 + offset, magic);
        Process::Read16(0x870DC1A + offset, protect);
        Process::Read16(0x870DC1C + offset, speed);
        Process::Read32(0x870DBB8 + offset, yokaiID);
        topScr.DrawRect(30, 20, 340, 200, Color::Black);
        btmScr.DrawRect(20, 20, 280, 200, Color::Black);
        topScr.DrawRect(32, 22, 336, 196, Color::White, false);
        btmScr.DrawRect(22, 22, 276, 196, Color::White, false);
        btmScr.DrawRect(275, 30, 15, 15, Color::Red, false);
        DrawLine(btmScr, 278, 33, 287, 43, Color::Red);
        DrawLine(btmScr, 287, 33, 278, 43, Color::Red);
        for (int i = 0; i < 32;)
        {
          std::string buff_str;
          u16 buff;

          if (nickname[i] == 0)
          {
            break;
          }
          else if ((nickname[i] < 0x80) || (nickname[i] > 0xA0))
          {
            buff = Convert::sjisToUtf16(nickname[i]);
            Utils::ConvertUTF16ToUTF8(buff_str, &buff);
            nickname_str += buff_str.substr(0, 1);
            i++;
          }
          else
          {
            buff = Convert::sjisToUtf16(nickname[i] * 0x100 + nickname[i + 1]);
            Utils::ConvertUTF16ToUTF8(buff_str, &buff);
            nickname_str += buff_str.substr(0, 3);
            i += 2;
          }
        }

        DrawSysfontPlus(topScr, "Yokai Editor", 37, 25, 0, 0, Color::White, Color::Black, Color::Red, true);
        DrawLine(topScr, 37, 42, 180, 42, Color::White);
        DrawSysfontPlus(topScr, Utils::Format("妖怪: (ID: %X)", yokaiID), 36, 55, 0, 0, Color::White, Color::Black, Color::Red, true);
        DrawSysfontPlus(topScr, "ニックネーム: " + nickname_str, 36, 75, 0, 0, Color::White, Color::Black, Color::Red, true);
        DrawSysfontPlus(topScr, Utils::Format("レベル: %d", level), 36, 95, 0, 0, Color::White, Color::Black, Color::Red, true);
        DrawSysfontPlus(topScr, Utils::Format("HP: %d", hp), 36, 115, 0, 0, Color::White, Color::Black, Color::Red, true);
        DrawSysfontPlus(topScr, Utils::Format("ちから: %d", power), 36, 135, 0, 0, Color::White, Color::Black, Color::Red, true);
        DrawSysfontPlus(topScr, Utils::Format("ようりょく: %d", magic), 36, 155, 0, 0, Color::White, Color::Black, Color::Red, true);
        DrawSysfontPlus(topScr, Utils::Format("まもり: %d", protect), 36, 175, 0, 0, Color::White, Color::Black, Color::Red, true);
        DrawSysfontPlus(topScr, Utils::Format("すばやさ: %d", speed), 36, 195, 0, 0, Color::White, Color::Black, Color::Red, true);
        DrawSysfontPlus(btmScr, "妖怪: ", 35, 30, 0, 0, Color::White, Color::Black, Color::Red, true);
        DrawSysfontPlus(btmScr, "ニックネーム: ", 35, 50, 0, 0, Color::White, Color::Black, Color::Red, true);
        btmScr.DrawRect(130, 48, 150, 19, Color::White, true);
        DrawSysfontPlus(btmScr, nickname_str, 280, 50, 0, 0, Color::Black, Color::Red, Color::Red, false, true);
        DrawSysfontPlus(btmScr, "妖怪ID: ", 35, 70, 0, 0, Color::White, Color::Black, Color::Red, true);
        btmScr.DrawRect(130, 68, 150, 19, Color::White, true);
        DrawSysfontPlus(btmScr, Utils::Format("%X", yokaiID), 280, 70, 0, 0, Color::Black, Color::Black, Color::Red, false, true);
        DrawSysfontPlus(btmScr, "レベル: ", 35, 90, 0, 0, Color::White, Color::Black, Color::Red, false);
        btmScr.DrawRect(120, 90, 50, 16, Color::White, true);
        DrawSysfontPlus(btmScr, Utils::Format("%d", level), 170, 90, 0, 0, Color::Black, Color::Black, Color::Red, false, true);
        btmScr.DrawRect(26, 120, 150, 93, Color::White, false);
        DrawSysfontPlus(btmScr, "ステータス", 40, 112, 0, 0, Color::White, Color::Black, Color::Red, true);
        DrawSysfontPlus(btmScr, "HP: ", 36, 127, 0, 0, Color::White, Color::Black, Color::Red, false);
        btmScr.DrawRect(120, 127, 50, 16, Color::White, true);
        DrawSysfontPlus(btmScr, Utils::Format("%d", hp), 170, 127, 0, 0, Color::Black, Color::Black, Color::Red, false, true);
        DrawSysfontPlus(btmScr, "ちから:", 36, 144, 0, 0, Color::White, Color::Black, Color::Red, false);
        btmScr.DrawRect(120, 144, 50, 16, Color::White, true);
        DrawSysfontPlus(btmScr, Utils::Format("%d", power), 170, 144, 0, 0, Color::Black, Color::Black, Color::Red, false, true);
        DrawSysfontPlus(btmScr, "ようりょく: ", 36, 161, 0, 0, Color::White, Color::Black, Color::Red, false);
        btmScr.DrawRect(120, 161, 50, 16, Color::White, true);
        DrawSysfontPlus(btmScr, Utils::Format("%d", magic), 170, 161, 0, 0, Color::Black, Color::Black, Color::Red, false, true);
        DrawSysfontPlus(btmScr, "まもり: ", 36, 178, 0, 0, Color::White, Color::Black, Color::Red, false);
        btmScr.DrawRect(120, 178, 50, 16, Color::White, true);
        DrawSysfontPlus(btmScr, Utils::Format("%d", protect), 170, 178, 0, 0, Color::Black, Color::White, Color::Red, false, true);
        DrawSysfontPlus(btmScr, "すばやさ: ", 36, 195, 0, 0, Color::White, Color::Black, Color::Red, true);
        btmScr.DrawRect(120, 195, 50, 16, Color::White, true);
        DrawSysfontPlus(btmScr, Utils::Format("%d", speed), 170, 195, 0, 0, Color::Black, Color::Black, Color::Red, false, true);
        DrawCircle(btmScr, 210, 190, 0, 25, 0, 360, Color::Lime);
        DrawCircle(btmScr, 265, 190, 0, 25, 0, 360, Color::Orange);
        DrawPlus(btmScr, "Save", 197, 187, 0, 0, Color::White, Color::Lime, Color::Red, 8);
        DrawPlus(btmScr, "Restore", 242, 187, 0, 0, Color::White, Color::Orange, Color::Red, 8);

        isOpened = true;
        afterKeyboard = false;
        ProcessImpl::Pause(false);
      }
    }
    auto position = Touch::GetPosition();
    if (Controller::IsKeyPressed(Touchpad) && isOpened)
    {
      if (TouchRect(275, 30, 15, 15))
      {
        Process::Play();
        isOpened = false;
      }
      else if (TouchRect(130, 48, 150, 19))
      {
        Keyboard select("select keyboard:", {"HEX", "キーボード"});

        int answer = select.Open();
        std::string out;
        std::vector<u8> sjis;
        switch (answer)
        {
        case -1:
          break;
        case 0:
          for (int i = 0; i < 32; i++)
          {
            u8 buff1;
            for (int j = 0; j < 32; j++)
            {
              Process::Read8(0x870DBBC + offset + j, nickname[j]);
            }
            Keyboard key(Utils::Format("ニックネーム:\n%dバイト目を打ってください\n%02X %02X %02X %02X %02X %02X %02X %02X\n%02X %02X %02X %02X %02X %02X %02X %02X\n%02X %02X %02X %02X %02X %02X %02X %02X\n%02X %02X %02X %02X %02X %02X %02X %02X", i + 1, nickname[0], nickname[1], nickname[2], nickname[3], nickname[4], nickname[5], nickname[6], nickname[7], nickname[8], nickname[9], nickname[10], nickname[11], nickname[12], nickname[13], nickname[14], nickname[15], nickname[16], nickname[17], nickname[18], nickname[19], nickname[20], nickname[21], nickname[22], nickname[23], nickname[24], nickname[25], nickname[26], nickname[27], nickname[28], nickname[29], nickname[30], nickname[31]));
            key.IsHexadecimal(true);
            key.SetMaxLength(2);
            if (key.Open(buff1) != -1)
            {
              Process::Write8(0x870DBBC + offset + i, buff1);
            }
            else
            {
              break;
            }
          }
          break;
        case 1:
          japKey(out, sjis);
          for (int j = 0; j < 32; j++)
          {
            Process::Write8(0x870DBBC + offset + j, 0x00);
          }
          for (int j = 0; j < (sjis.size() > 33 ? 32 : sjis.size()); j++)
          {
            Process::Write8(0x870DBBC + offset + j, sjis[j]);
          }
          break;
        default:
          break;
        }
        Process::Play();
        isOpened = false;
        afterKeyboard = true;
      }
      else if (TouchRect(130, 68, 150, 19))
      {
        Sleep(Milliseconds(200));
        u32 buff;
        Keyboard key("妖怪ID:");
        key.IsHexadecimal(true);
        key.SetMaxLength(8);
        if (key.Open(buff) != -1)
        {
          Process::Write32(0x870DBB8 + offset, buff);
        }
        Process::Play();
        isOpened = false;
        afterKeyboard = true;
      }
      else if (TouchRect(120, 90, 50, 20))
      {
        Sleep(Milliseconds(200));
        u8 buff;
        Keyboard key("レベル:\n255以下にしてください");
        key.SetMaxLength(3);
        key.IsHexadecimal(false);
        if (key.Open(buff) != -1)
        {
          Process::Write8(0x870DC03 + offset, buff);
        }
        Process::Play();
        isOpened = false;
        afterKeyboard = true;
      }
      else if (TouchRect(120, 127, 50, 15))
      {
        Sleep(Milliseconds(200));
        u16 buff;
        Keyboard key("HP:");
        key.SetMaxLength(5);
        key.IsHexadecimal(false);
        if (key.Open(buff) != -1)
        {
          Process::Write16(0x870DBEC + offset, buff);
        }
        Process::Play();
        isOpened = false;
        afterKeyboard = true;
      }
      else if (TouchRect(120, 144, 50, 15))
      {
        Sleep(Milliseconds(200));
        u16 buff;
        Keyboard key("ちから:");
        key.SetMaxLength(5);
        key.IsHexadecimal(false);
        if (key.Open(buff) != -1)
        {
          Process::Write16(0x870DC16 + offset, buff);
        }
        Process::Play();
        isOpened = false;
        afterKeyboard = true;
      }
      else if (TouchRect(120, 161, 50, 15))
      {
        Sleep(Milliseconds(200));
        u16 buff;
        Keyboard key("ようりょく:");
        key.SetMaxLength(5);
        key.IsHexadecimal(false);
        if (key.Open(buff) != -1)
        {
          Process::Write16(0x870DC18 + offset, buff);
        }
        Process::Play();
        isOpened = false;
        afterKeyboard = true;
      }
      else if (TouchRect(120, 178, 50, 15))
      {
        Sleep(Milliseconds(200));
        u16 buff;
        Keyboard key("まもり:");
        key.SetMaxLength(5);
        key.IsHexadecimal(false);
        if (key.Open(buff) != -1)
        {
          Process::Write16(0x870DC1A + offset, buff);
        }
        Process::Play();
        isOpened = false;
        afterKeyboard = true;
      }
      else if (TouchRect(120, 195, 50, 16))
      {
        Sleep(Milliseconds(200));
        u16 buff;
        Keyboard key("すばやさ:");
        key.SetMaxLength(5);
        key.IsHexadecimal(false);
        if (key.Open(buff) != -1)
        {
          Process::Write16(0x870DC1C + offset, buff);
        }
        Process::Play();
        isOpened = false;
        afterKeyboard = true;
      }
      else if (TouchCircle(210, 190, 25))
      {
        Sleep(Milliseconds(200));
        Keyboard select("select mode:\n保存するときの名前入力してね", {"ニックネーム", "ステータス", "妖怪ID"});
        int answer = select.Open();
        std::string out;
        if (answer != -1)
        {
          KeyboardImpl a("");
          a.SetLayout(Layout::QWERTY);
          a.Run();
          out = a.GetInput();
          if (out.empty())
            answer = -1;
        }
        Process::Play();
        isOpened = false;
        afterKeyboard = true;
        Directory::Create("YokaiEditor");
        File file;
        switch (answer)
        {
        case -1:
          break;
        case 0:
          File::Create("YokaiEditor/nickname_" + out + ".bin");
          File::Open(file, "YokaiEditor/nickname_" + out + ".bin");
          file.Dump(0x870DBBC + offset, 32);
          file.Flush();
          file.Close();
          break;
        case 1:
          File::Create("YokaiEditor/status_" + out + ".bin");
          File::Open(file, "YokaiEditor/status_" + out + ".bin");
          file.Write((void *)&level, 1);
          file.Write((void *)&hp, 2);
          file.Write((void *)&power, 2);
          file.Write((void *)&magic, 2);
          file.Write((void *)&protect, 2);
          file.Write((void *)&speed, 2);
          file.Close();
          break;
        case 2:
          File::Create("YokaiEditor/yokaiID_" + out + ".bin");
          File::Open(file, "YokaiEditor/yokaiID_" + out + ".bin");
          file.Dump(0x870DBB8 + offset, 4);
          file.Close();
          break;
        default:
          break;
        }
      }
      else if (TouchCircle(265, 190, 25))
      {
        Sleep(Milliseconds(200));
        Keyboard select("select mode:", {"ニックネーム", "ステータス", "妖怪ID", "削除"});
        int answer = select.Open();
        Process::Play();
        isOpened = false;
        afterKeyboard = true;
        Directory dir("YokaiEditor");
        File file;
        StringVector files_name;
        switch (answer)
        {
        case -1:
          break;
        case 0:
        {
          dir.ListFiles(files_name, "nickname_");
          for (int i = 0; i < files_name.size(); i++)
          {
            files_name[i].replace(files_name[i].find("nickname_"), 9, "").replace(files_name[i].find(".bin"), 4, "");
          }
          if (files_name.size() == 0)
          {
            files_name.push_back("ok");
            Keyboard file_select("no files found", files_name);
            file_select.Open();
            break;
          }
          Keyboard file_select("select:", files_name);
          File::Open(file, "YokaiEditor/nickname_" + files_name[file_select.Open()] + ".bin");
          file.Inject(0x870DBBC + offset, 32);
          file.Close();
          break;
        }
        case 1:
        {
          dir.ListFiles(files_name, "status_");
          for (int i = 0; i < files_name.size(); i++)
          {
            files_name[i].replace(files_name[i].find("status_"), 7, "").replace(files_name[i].find(".bin"), 4, "");
          }
          if (files_name.size() == 0)
          {
            files_name.push_back("ok");
            Keyboard file_select("no files found", files_name);
            file_select.Open();
            break;
          }
          Keyboard file_select("select:", files_name);
          File::Open(file, "YokaiEditor/status_" + files_name[file_select.Open()] + ".bin");
          u8 buff_8;
          u16 buff_16;
          file.Read((void *)&buff_8, 1);
          Process::Write8(0x870DC03 + offset, buff_8);
          file.Read((void *)&buff_16, 2);
          Process::Write16(0x870DBEC + offset, buff_16);
          file.Read((void *)&buff_16, 2);
          Process::Write16(0x870DC16 + offset, buff_16);
          file.Read((void *)&buff_16, 2);
          Process::Write16(0x870DC18 + offset, buff_16);
          file.Read((void *)&buff_16, 2);
          Process::Write16(0x870DC1A + offset, buff_16);
          file.Read((void *)&buff_16, 2);
          Process::Write16(0x870DC1C + offset, buff_16);
          file.Close();
          break;
        }
        case 2:
        {
          dir.ListFiles(files_name, "yokaiID_");
          for (int i = 0; i < files_name.size(); i++)
          {
            files_name[i].replace(files_name[i].find("yokaiID_"), 8, "").replace(files_name[i].find(".bin"), 4, "");
          }
          if (files_name.size() == 0)
          {
            files_name.push_back("ok");
            Keyboard file_select("no files found", files_name);
            file_select.Open();
            break;
          }
          Keyboard file_select("select:", files_name);
          File::Open(file, "YokaiEditor/yokaiID_" + files_name[file_select.Open()] + ".bin");
          file.Inject(0x870DBB8 + offset, 4);
          file.Close();
          break;
        }
        case 3:
        {
          Sleep(Milliseconds(200));
          dir.ListFiles(files_name);
          if (files_name.size() == 0)
          {
            files_name.push_back("ok");
            Keyboard file_select("no files found", files_name);
            file_select.Open();
            break;
          }
          Keyboard file_select("select:", files_name);
          int i = file_select.Open();
          if (i != -1)
            File::Remove("YokaiEditor/" + files_name[i]);
          break;
        }
        default:
          break;
        }
      }
    }
    if (afterKeyboard)
    {
      afterKeyboardSec++;
    }
    else
    {
      afterKeyboardSec = 0;
    }
  }

  float AA, BB, CC;

  float cubeWidth = 20;
  int width = 400, height = 240;
  float zBuffer[400 * 240];
  char buffer[400 * 240];
  int distanceFromCam = 100;
  float horizontalOffset;
  float K1 = 40;

  float incrementSpeed = 0.6;

  float x, y, z;
  float ooz;
  int xp, yp;
  int idx;

  float calculateX(int i, int j, int k)
  {
    return j * sin(AA) * sin(BB) * cos(CC) - k * cos(AA) * sin(BB) * cos(CC) +
           j * cos(AA) * sin(CC) + k * sin(AA) * sin(CC) + i * cos(BB) * cos(CC);
  }

  float calculateY(int i, int j, int k)
  {
    return j * cos(AA) * cos(CC) + k * sin(AA) * cos(CC) -
           j * sin(AA) * sin(BB) * sin(CC) + k * cos(AA) * sin(BB) * sin(CC) -
           i * cos(BB) * sin(CC);
  }

  float calculateZ(int i, int j, int k)
  {
    return k * cos(AA) * cos(BB) - j * sin(AA) * cos(BB) + i * sin(BB);
  }

  void calculateForSurface(float cubeX, float cubeY, float cubeZ, int ch)
  {
    x = calculateX(cubeX, cubeY, cubeZ);
    y = calculateY(cubeX, cubeY, cubeZ);
    z = calculateZ(cubeX, cubeY, cubeZ) + distanceFromCam;

    ooz = 1 / z;

    xp = (int)(width / 2 + horizontalOffset + K1 * ooz * x * 2);
    yp = (int)(height / 2 + K1 * ooz * y);

    idx = xp + yp * width;
    if (idx >= 0 && idx < width * height)
    {
      if (ooz > zBuffer[idx])
      {
        zBuffer[idx] = ooz;
        buffer[idx] = ch;
      }
    }
  }
  void rotateCube(void)
  {
    memset(buffer, 10, sizeof(buffer));
    memset(zBuffer, 0, width * height * 4);
    horizontalOffset = 1 * cubeWidth;
    // second cube
    for (float cubeX = -cubeWidth; cubeX < cubeWidth; cubeX += incrementSpeed)
    {
      for (float cubeY = -cubeWidth; cubeY < cubeWidth; cubeY += incrementSpeed)
      {
        calculateForSurface(cubeX, cubeY, -cubeWidth, '@');
        calculateForSurface(cubeWidth, cubeY, cubeX, '$');
        calculateForSurface(-cubeWidth, cubeY, -cubeX, '~');
        calculateForSurface(-cubeX, cubeY, cubeWidth, '#');
        calculateForSurface(cubeX, -cubeWidth, -cubeY, ';');
        calculateForSurface(cubeX, cubeWidth, cubeY, '+');
      }
    }
    for (int i = 0; i < 400; i++)
    {
      for (int j = 0; j < 240; j++)
      {
        screenColor[i][j] = Color::Black;
      }
    }
    for (int k = 0; k < width * height; k++)
    {
      if (buffer[k] == 64)
      {
        screenColor[k % width][k / width] = Color::Red;
      }
      if (buffer[k] == 36)
      {
        screenColor[k % width][k / width] = Color::Blue;
      }
      if (buffer[k] == 126)
      {
        screenColor[k % width][k / width] = Color::Orange;
      }
      if (buffer[k] == 35)
      {
        screenColor[k % width][k / width] = Color::Green;
      }
      if (buffer[k] == 59)
      {
        screenColor[k % width][k / width] = Color::White;
      }
      if (buffer[k] == 43)
      {
        screenColor[k % width][k / width] = Color::Yellow;
      }
    }
  }

  u32 slow = 0;
  void Cube(MenuEntry *entry)
  {
    const Screen &screen = OSD::GetTopScreen();
    for (int i = 0; i < 400; i++)
    {
      for (int j = 0; j < 240; j++)
      {
        if (screenColor[i][j] != Color::Black)
        {
          screen.DrawPixel(i, j, screenColor[i][j]);
        }
      }
    }
    slow++;
    if (slow % 10 == 0)
    {
      rotateCube();
    }
    if (Controller::IsKeyDown(Key::CPadLeft))
    {
      BB -= 0.05;
    }
    if (Controller::IsKeyDown(Key::CPadRight))
    {
      BB += 0.05;
    }
    if (Controller::IsKeyDown(Key::CPadUp))
    {
      AA += 0.05;
    }
    if (Controller::IsKeyDown(Key::CPadDown))
    {
      AA -= 0.05;
    }
    if (Controller::IsKeyDown(Key::CStickUp))
    {
      CC -= 0.05;
    }
    if (Controller::IsKeyDown(Key::CStickDown))
    {
      CC += 0.05;
    }
    if (Controller::IsKeyDown(Key::CStickLeft))
    {
      distanceFromCam -= 5;
    }
    if (Controller::IsKeyDown(Key::CStickRight))
    {
      distanceFromCam += 5;
    }
  }

  int frame_num = 0;
  void BadApple(MenuEntry *entry)
  {
    ProcessImpl::Pause(false);

    if (entry->WasJustActivated())
      frame_num = 0;
    std::vector<u64> str_frame = getFrame(frame_num);

    const Screen &screen = OSD::GetTopScreen();
    for (int i = 0; i < str_frame.size(); i++)
    {
      u8 index = 0;
      for (u64 j = 1; j < 0x1000000000000000; j *= 2)
      {
        if (str_frame[i] & j)
          screen.DrawRect(380 - index * 6, 10 + i * 10, 6, 10, Color::White);
        else
          screen.DrawRect(380 - index * 6, 10 + i * 10, 6, 10, Color::Black);
        index++;
      }
    }
    slow++;
    if (slow % 0x7 == 0)
    {
      frame_num++;
    }
    OSD::SwapBuffers();
  }

  void ChangeBackGround(MenuEntry *entry)
  {
    StringVector files_name;
    Directory("BMP").ListFiles(files_name);
    std::string file_name;
    switch (Keyboard("which Background?:", {"Top", "Bottom"}).Open())
    {
    case 0:
    {
      u8 answer = Keyboard("select BMP:", files_name).Open();
      if (answer != -1)
        file_name = files_name[answer];
      else
        break;
      AliceCodes::SetTopScreenBackground("BMP/" + file_name);
      break;
    }
    case 1:
    {
      u8 answer = Keyboard("select BMP:", files_name).Open();
      if (answer != -1)
        file_name = files_name[answer];
      else
        break;
      AliceCodes::SetBottomScreenBackground("BMP/" + file_name);
      break;
    }
    }
  }
}