#include "cheats.hpp"
#include "osdjp.hpp"
#include "AliceCodes.hpp"
#include "csvc.h"
#include "KaniCodes.hpp"
#include "memory.h"
#include "../libctrpf/include/CTRPluginFrameworkImpl/System/ProcessImpl.hpp"
#include "../libctrpf/include/CTRPluginFrameworkImpl/Menu/KeyboardImpl.hpp"

namespace CTRPluginFramework
{
  void Test1(MenuEntry *entry)
  {
  }

  bool flagShowScreenBuffer = false;
  std::vector<std::vector<Color>> screenColor(400, std::vector<Color>(240, Color::Black));

  bool ShowScreenBuffer(const Screen &screen)
  {
    if (!screen.IsTop)
      return false;
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
    if (!flagShowScreenBuffer)
      return true;
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
  Color colorList[6] = {Color::Red, Color::Blue, Color::Cyan, Color::Green, Color::Yellow, Color::Orange};
  bool isReset = false;
  void Pipes(MenuEntry *entry)
  {
    if (entry->WasJustActivated())
    {
      screenColor = std::vector<std::vector<Color>>(400, std::vector<Color>(240, Color::Black));
      flagShowScreenBuffer = true;
      OSD::Run(ShowScreenBuffer);
    }
    if (!entry->IsActivated())
    {
      flagShowScreenBuffer = false;
      OSD::Stop(ShowScreenBuffer);
    }
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
        i++;
      else
        i = 0;
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
              screenColor[xPos + 2][yPos] = Color::Black;
            else
              screenColor[xPos - 2][yPos] = Color::Black;
            yPos++;
            break;
          }
          screenColor[xPos + 2][yPos] = Color::Black;
          screenColor[xPos - 2][yPos] = Color::Black;
          yPos++;
          break;
        case 1:
          if (j == 0)
          {
            if (buff_direct == 0)
              screenColor[xPos][yPos + 2] = Color::Black;
            else
              screenColor[xPos][yPos - 2] = Color::Black;
            xPos++;
            break;
          }
          screenColor[xPos][yPos + 2] = Color::Black;
          screenColor[xPos][yPos - 2] = Color::Black;
          xPos++;
          break;
        case 2:
          if (j == 0)
          {
            if (buff_direct == 0)
              screenColor[xPos][yPos + 2] = Color::Black;
            else
              screenColor[xPos][yPos - 2] = Color::Black;
            xPos--;
            break;
          }
          screenColor[xPos][yPos + 2] = Color::Black;
          screenColor[xPos][yPos - 2] = Color::Black;
          xPos--;
          break;
        default:
          if (j == 0)
          {
            if (buff_direct == 1)
              screenColor[xPos + 2][yPos] = Color::Black;
            else
              screenColor[xPos - 2][yPos] = Color::Black;
            yPos--;
            break;
          }
          screenColor[xPos + 2][yPos] = Color::Black;
          screenColor[xPos - 2][yPos] = Color::Black;
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
          break;
      }
      buff_direct = direct;
      direct = temp_direct;
    }
    if (Controller::IsKeyPressed(Key::Start))
    {
      isReset = true;
      for (int i = 0; i < 400; i++)
      {
        for (int j = 0; j < 240; j++)
          screenColor[i][j] = Color::Black;
      }
    }
  }

  std::vector<std::vector<std::string>> MenuEntryNameList = {{"pipes", "ぱいぷす", "パイプス", "Pipes"}, {"yokaieditor", "ようかいえでぃたー", "ヨウカイエディター", "YokaiEditor"}, {"cube", "きゅーぶ", "キューブ", "Cube"}, {"bad apple!!", "ばっどあっぷる！！", "バッドアップル！！", "Bad Apple!!"}, {"jpnotify", "じぇーぴーにほんごのてぃふぁい", "ジェーピーニホンゴノティファイ", "JPNotify"}, {"changebackground", "ちぇんじばっくぐらうんど", "チェンジバックグラウンド", "ChangeBackGround"}, {"playmusic", "ぷれいみゅーじっく", "プレイミュージック", "PlayMusic"}, {"indicator", "いんでぃけーたー", "インディケーター", "Indicator"}};
  FuncPointer GameFuncList[] = {Pipes, YokaiEditor, Cube, BadApple, JPNotify, nullptr, nullptr, Indicator};
  FuncPointer MenuFuncList[] = {nullptr, nullptr, nullptr, nullptr, nullptr, ChangeBackGround, PlayMusic, nullptr};
  std::string NoteList[] = {"startで消えます", "designed with OSD Designer\nrespect for Tekito_256\n\n控えのメダルでSTARTボタンを押してください\n\n第一水準漢字しか対応してません(表示のみ)", "", nullptr, "startで表示\n(Y押しながら押すんじゃないぞ！)", "BMPフォルダに画像を入れてください", "", ""};
  void Search(MenuEntry *entry)
  {
    std::string input;
    std::vector<u8> entryNums;
    PluginMenu *menu = PluginMenu::GetRunningInstance();
    japKey(input, "エントリー名を入力してください");
    if (input.empty())
      return;
    std::transform(input.begin(), input.end(), input.begin(), [](unsigned char c)
                   { return std::tolower(c); });
    // std::transform(input.cbegin(), input.cend(), input.begin(), tolower);
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
    std::vector<MenuFolder *> folders = menu->GetFolderList();
    for (auto folder : folders)
    {
      if (folder->Name() == "Search")
      {
        folder->Clear();
        *folder += new MenuEntry("Search", nullptr, Search);
        for (int i = 0; i < entryNums.size(); i++)
        {
          *folder += new MenuEntry(MenuEntryNameList[entryNums[i]][3], GameFuncList[entryNums[i]], MenuFuncList[entryNums[i]], NoteList[entryNums[i]]);
        }
      }
    }
  }

  bool isOpened = false;
  void YokaiEditor(MenuEntry *entry)
  {
    if (((*(u32 *)0x858088) == 0) && (!System::IsCitra()))
    {
      if (Controller::IsKeyPressed(Key::Start))
        OSD::Notify("open with any medal selected");
      return;
    }
    u8 level, leaderIndex, nickname[24], character, pose;
    u16 hp, power, magic, protect, speed;
    u32 yokaiID, offset, kon1, kon2, attackID, magicID, specialAttackID;
    std::string nickname_str, yokaiName;
    StringVector characters = {"超まじめで", "まじめで", "すなおで", "気ままで", "ずぼらで", "超ずぼらで", "ビビリで", "", "", "", "", "", "", "", "", ""};
    StringVector characters2 = {"", "短気", "れいせい", "しんちょう", "やさしい", "いやらしい", "協力的", "荒くれ", "ずのう的", "動じない", "情け深い", "非道", "けんしん的", "", "", ""};
    StringVector poses = {"きめポーズ1", "きめポーズ2", "きめポーズ3", "きめポーズ4", "おまかせ", "", "", "", "", "", "", "", "", "", ""};

    Process::Read8((*(u32 *)0x858088) + 0x361, leaderIndex);
    offset = leaderIndex * 0xC4;
    Process::Write8(0x870DC05 + offset, 0xAF);
    const Screen &topScr = OSD::GetTopScreen();
    const Screen &btmScr = OSD::GetBottomScreen();
    if (isOpened)
    {
      Process::Read8(0x870DC03 + offset, level);
      Process::Read8(0x870DC04 + offset, pose);
      Process::Read8(0x870DC08 + offset, character);
      for (int i = 0; i < 24; i++)
      {
        Process::Read8(0x870DBBC + offset + i, nickname[i]);
      }
      Process::Read16(0x870DBEC + offset, hp);
      Process::Read16(0x870DC16 + offset, power);
      Process::Read16(0x870DC18 + offset, magic);
      Process::Read16(0x870DC1A + offset, protect);
      Process::Read16(0x870DC1C + offset, speed);
      Process::Read32(0x870DBB8 + offset, yokaiID);
      Process::Read32(0x870DBD4 + offset, kon1);
      Process::Read32(0x870DBD8 + offset, kon2);
      Process::Read32(0x870DC2C + offset, attackID);
      Process::Read32(0x870DC38 + offset, magicID);
      Process::Read32(0x870DC5C + offset, specialAttackID);
      topScr.DrawRect(30, 20, 340, 200, Color::Black);
      btmScr.DrawRect(20, 20, 280, 200, Color::Black);
      topScr.DrawRect(32, 22, 336, 196, Color::White, false);
      btmScr.DrawRect(22, 22, 276, 196, Color::White, false);
      btmScr.DrawRect(275, 30, 15, 15, Color::Red, false);
      DrawLine(btmScr, 278, 33, 287, 43, Color::Red);
      DrawLine(btmScr, 287, 33, 278, 43, Color::Red);
      for (int i = 0; i < 22;)
      {
        std::string buff_str;
        u16 buff;
        if (nickname[i] == 0)
          break;
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
      for (int i = 0; i < 705; i++)
      {
        if (*(u32 *)(0x08576864 + (i * 0x84)) == *(u32 *)(0x870DBB8 + offset))
        {
          u32 ModelAddress = 0;
          for (int y = 0; y < 602; y++)
          {
            if (*(u32 *)(0x08576868 + (i * 0x84)) == *(u32 *)(0x08570774 + (y * 0x28)))
            {
              ModelAddress = 0x08570774 + (y * 0x28);
              break;
            }
          }
          if (ModelAddress)
          {
            yokaiName = ProcessPlus::ReadSJIS(*(u32 *)(*(u32 *)(ModelAddress + 0x04)));
            break;
          }
        }
      }

      DrawSysfontPlus(topScr, "Yokai Editor", 37, 25, 0, 0, Color::White, Color::Black, Color::Red, true);
      DrawLine(topScr, 37, 42, 180, 42, Color::White);
      DrawSysfontPlus(topScr, "妖怪: " + yokaiName + Utils::Format(" (ID: %X)", yokaiID), 36, 55, 0, 0, Color::White, Color::Black, Color::Red, true);
      DrawSysfontPlus(topScr, "ニックネーム: " + nickname_str, 36, 75, 0, 0, Color::White, Color::Black, Color::Red, true);
      DrawSysfontPlus(topScr, Utils::Format("レベル: %d", level), 36, 95, 0, 0, Color::White, Color::Black, Color::Red, true);
      DrawSysfontPlus(topScr, Utils::Format("HP: %d", hp), 36, 115, 0, 0, Color::White, Color::Black, Color::Red, true);
      DrawSysfontPlus(topScr, Utils::Format("ちから: %d", power), 36, 135, 0, 0, Color::White, Color::Black, Color::Red, true);
      DrawSysfontPlus(topScr, Utils::Format("ようりょく: %d", magic), 36, 155, 0, 0, Color::White, Color::Black, Color::Red, true);
      DrawSysfontPlus(topScr, Utils::Format("まもり: %d", protect), 36, 175, 0, 0, Color::White, Color::Black, Color::Red, true);
      DrawSysfontPlus(topScr, Utils::Format("すばやさ: %d", speed), 36, 195, 0, 0, Color::White, Color::Black, Color::Red, true);
      DrawSysfontPlus(topScr, "性格: " + characters[character / 0x10] + " " + characters2[character % 0x10], 180, 95, 0, 0, Color::White, Color::Black, Color::Red, true);
      DrawSysfontPlus(topScr, "きめポーズ: " + poses[pose % 0x10], 180, 115, 0, 0, Color::White, Color::Black, Color::Red, true);
      DrawSysfontPlus(btmScr, "妖怪: " + yokaiName, 35, 30, 0, 0, Color::White, Color::Black, Color::Red, true);
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
      DrawCircle(btmScr, 210, 190, 0, 25, 0, 360, Color::Lime, 8);
      DrawCircle(btmScr, 265, 190, 0, 25, 0, 360, Color::Orange, 8);
      DrawPlus(btmScr, "Save", 197, 187, 0, 0, Color::White, Color::Lime, Color::Red, 8);
      DrawPlus(btmScr, "Restore", 242, 187, 0, 0, Color::White, Color::Orange, Color::Red, 8);
      DrawRectPlus(btmScr, 175, 95, 50, 25, Color::White, true, 0);
      DrawSysfontPlus(btmScr, "性格", 185, 100, 0, 0, Color::Black, Color::Black, Color::Red, false, false, 8);
      DrawRectPlus(btmScr, 230, 95, 50, 25, Color::White, true, 0);
      DrawSysfontPlus(btmScr, "ポーズ", 234, 100, 0, 0, Color::Black, Color::Black, Color::Red, false, false, 8);

      isOpened = true;
      ProcessImpl::Pause(false);
    }
    if (Controller::IsKeyPressed(Key::Start))
    {
      ProcessImpl::Play(true);
      isOpened = !isOpened;
    }
    auto position = Touch::GetPosition();
    if (Controller::IsKeyPressed(Touchpad) && isOpened)
    {
      if (TouchRect(275, 30, 15, 15))
      {
        ProcessImpl::Play(true);
        isOpened = !isOpened;
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
          for (int i = 0; i < 23; i++)
          {
            u8 buff1;
            for (int j = 0; j < 23; j++)
            {
              Process::Read8(0x870DBBC + offset + j, nickname[j]);
            }
            Keyboard key(Utils::Format("ニックネーム:\n%dバイト目を打ってください\n%02X %02X %02X %02X %02X %02X %02X %02X\n%02X %02X %02X %02X %02X %02X %02X %02X\n%02X %02X %02X %02X %02X %02X %02X", i + 1, nickname[0], nickname[1], nickname[2], nickname[3], nickname[4], nickname[5], nickname[6], nickname[7], nickname[8], nickname[9], nickname[10], nickname[11], nickname[12], nickname[13], nickname[14], nickname[15], nickname[16], nickname[17], nickname[18], nickname[19], nickname[20], nickname[21], nickname[22], nickname[23]));
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
          japKey(out, "妖怪の名前を入れてください", &sjis);
          for (int j = 0; j < 24; j++)
          {
            Process::Write8(0x870DBBC + offset + j, 0x00);
          }
          for (int j = 0; j < (sjis.size() > 23 ? 23 : sjis.size()); j++)
          {
            Process::Write8(0x870DBBC + offset + j, sjis[j]);
          }
          break;
        default:
          break;
        }
        ProcessImpl::Play(true);
        isOpened = false;
      }
      else if (TouchRect(130, 68, 150, 19))
      {
        Sleep(Milliseconds(200));
        s8 ans = Keyboard("select", {"検索", "ID打ち込み"}).Open();
        switch (ans)
        {
        case 0:
        {
          std::string out;
          japKey(out, "妖怪の名前を入れてください");
          StringVector yokaiNames;
          std::vector<u32> yokaiIDs;
          for (int i = 0; i < 706; i++)
          {
            u32 ModelAddress = 0;
            for (int y = 0; y < 602; y++)
            {
              if (*(u32 *)(0x08576868 + (i * 0x84)) == *(u32 *)(0x08570774 + (y * 0x28)))
              {
                ModelAddress = 0x08570774 + (y * 0x28);
                break;
              }
            }
            if (ModelAddress)
            {
              if (*(u32 *)(ModelAddress + 0x04) < 0x8000000)
                continue;
              if (ProcessPlus::ReadSJIS(*(u32 *)(*(u32 *)(ModelAddress + 0x04))).find(out) != std::string::npos)
              {
                yokaiNames.push_back(ProcessPlus::ReadSJIS(*(u32 *)(*(u32 *)(ModelAddress + 0x04))));
                yokaiIDs.push_back((*(u32 *)(0x08576864 + (i * 0x84))));
              }
            }
          }
          ans = Keyboard("select", yokaiNames).Open();
          if (ans != -1)
          {
            Process::Write32(0x870DBB8 + offset, yokaiIDs[ans]);
          }
          break;
        }
        case 1:
        {
          u32 buff;
          Keyboard key("妖怪ID:");
          key.IsHexadecimal(true);
          key.SetMaxLength(8);
          if (key.Open(buff) != -1)
          {
            Process::Write32(0x870DBB8 + offset, buff);
          }
        }
        break;
        }
        ProcessImpl::Play(true);
      }
      else if (TouchRect(120, 90, 50, 20))
      {
        Sleep(Milliseconds(200));
        if (Keyboard("レベル:").Open(level) != -1)
          Process::Write8(0x870DC03 + offset, level);
        ProcessImpl::Play(true);
      }
      else if (TouchRect(120, 127, 50, 15))
      {
        Sleep(Milliseconds(200));
        if (Keyboard("HP:").Open(hp) != -1)
          Process::Write16(0x870DBEC + offset, hp);
        ProcessImpl::Play(true);
      }
      else if (TouchRect(120, 144, 50, 15))
      {
        Sleep(Milliseconds(200));
        if (Keyboard("ちから:").Open(power) != -1)
          Process::Write16(0x870DC16 + offset, power);
        ProcessImpl::Play(true);
      }
      else if (TouchRect(120, 161, 50, 15))
      {
        Sleep(Milliseconds(200));
        if (Keyboard("ようりょく:").Open(magic) != -1)
          Process::Write16(0x870DC18 + offset, magic);
        ProcessImpl::Play(true);
      }
      else if (TouchRect(120, 178, 50, 15))
      {
        Sleep(Milliseconds(200));
        if (Keyboard("まもり:").Open(protect) != -1)
          Process::Write16(0x870DC1A + offset, protect);
        ProcessImpl::Play(true);
      }
      else if (TouchRect(120, 195, 50, 16))
      {
        Sleep(Milliseconds(200));
        if (Keyboard("すばやさ:").Open(speed) != -1)
        {
          Process::Write16(0x870DC1C + offset, speed);
        }
        ProcessImpl::Play(true);
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
        ProcessImpl::Play(true);
        Directory::Create("YokaiEditor");
        File file;
        switch (answer)
        {
        case -1:
          break;
        case 0:
          File::Create("YokaiEditor/nickname_" + out + ".bin");
          File::Open(file, "YokaiEditor/nickname_" + out + ".bin");
          file.Dump(0x870DBBC + offset, 24);
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
        Sleep(Milliseconds(100));
        Keyboard select("select mode:", {"ニックネーム", "ステータス", "妖怪ID", "削除"});
        int answer = select.Open();
        ProcessImpl::Play(true);
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
          file.Inject(0x870DBBC + offset, 24);
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
          answer = file_select.Open();
          if (answer != -1)
          {
            File::Open(file, "YokaiEditor/status_" + files_name[answer] + ".bin");
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
          }
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
          answer = file_select.Open();
          if (answer != -1)
          {
            File::Open(file, "YokaiEditor/yokaiID_" + files_name[answer] + ".bin");
            file.Inject(0x870DBB8 + offset, 4);
            file.Close();
          }
          break;
        }
        case 3:
        {
          Sleep(Milliseconds(100));
          dir.ListFiles(files_name);
          if (files_name.size() == 0)
          {
            files_name.push_back("ok");
            Keyboard file_select("no files found", files_name);
            file_select.Open();
            break;
          }
          Keyboard file_select("select:", files_name);
          answer = file_select.Open();
          if (answer != -1)
            File::Remove("YokaiEditor/" + files_name[answer]);
          break;
        }
        default:
          break;
        }
      }
      else if (TouchRect(175, 95, 50, 25))
      {
        Sleep(Milliseconds(100));
        s8 i = Keyboard(characters).Open();
        if (i != -1)
        {
          Sleep(Milliseconds(200));
          s8 j = Keyboard(characters2).Open();
          ProcessImpl::Play(true);
          if (j != -1)
            Process::Write8(0x870DC08 + offset, i * 0x10 + j);
        }
      }
      else if (TouchRect(230, 95, 50, 25))
      {
        Sleep(Milliseconds(100));
        s8 i = Keyboard(poses).Open();
        ProcessImpl::Play(true);
        if (i != -1)
          Process::Write8(0x870DC04 + offset, i);
      }
      Sleep(Milliseconds(100));
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
    screenColor = std::vector<std::vector<Color>>(400, std::vector<Color>(240, Color::Black));
    for (int k = 0; k < width * height; k++)
    {
      switch (buffer[k])
      {
      case 64:
        screenColor[k % width][k / width] = Color::Red;
        break;
      case 36:
        screenColor[k % width][k / width] = Color::Blue;
        break;
      case 126:
        screenColor[k % width][k / width] = Color::Orange;
        break;
      case 35:
        screenColor[k % width][k / width] = Color::Green;
        break;
      case 59:
        screenColor[k % width][k / width] = Color::White;
        break;
      case 43:
        screenColor[k % width][k / width] = Color::Yellow;
        break;
      }
    }
  }

  void Cube(MenuEntry *entry)
  {
    if (entry->WasJustActivated())
    {
      flagShowScreenBuffer = true;
      OSD::Run(ShowScreenBuffer);
    }
    if (!entry->IsActivated())
    {
      flagShowScreenBuffer = false;
      OSD::Stop(ShowScreenBuffer);
    }
    rotateCube();
    if (Controller::IsKeyDown(Key::CPadLeft))
      BB -= 0.05;
    if (Controller::IsKeyDown(Key::CPadRight))
      BB += 0.05;
    if (Controller::IsKeyDown(Key::CPadUp))
      AA += 0.05;
    if (Controller::IsKeyDown(Key::CPadDown))
      AA -= 0.05;
    if (Controller::IsKeyDown(Key::CStickUp))
      CC -= 0.05;
    if (Controller::IsKeyDown(Key::CStickDown))
      CC += 0.05;
    if (Controller::IsKeyDown(Key::CStickLeft))
      distanceFromCam -= 5;
    if (Controller::IsKeyDown(Key::CStickRight))
      distanceFromCam += 5;
  }

  int frame_num = 0;
  void BadApple(MenuEntry *entry)
  {
    if (!entry->IsActivated())
    {
      ProcessImpl::Play(true);
      return;
    }
    ProcessImpl::Pause(false);
    if (entry->WasJustActivated())
      frame_num = 0;

    std::vector<u64> frame = getFrame(frame_num);

    const Screen &screen = OSD::GetTopScreen();
    for (int i = 0; i < frame.size(); i++)
    {
      u8 index = 0;
      for (u64 j = 1; j < 0x1000000000000000; j *= 2)
      {
        if (frame[i] & j)
          screen.DrawRect(380 - index * 6, 10 + i * 10, 6, 10, Color::White);
        else
          screen.DrawRect(380 - index * 6, 10 + i * 10, 6, 10, Color::Black);
        index++;
      }
    }
    Sleep(Milliseconds(100));
    frame_num++;
    OSD::SwapBuffers();
  }

  void ChangeBackGround(MenuEntry *entry)
  {
    StringVector files_name = {};
    Directory("BMP", true).ListFiles(files_name);
    if (!(files_name.size()))
    {
      MessageBox("no files found")();
      Sleep(Milliseconds(500));
      return;
    }
    Keyboard key("select BMP:", files_name);
    s8 i = key.Open();
    if (i != -1)
    {
      switch (Keyboard("which", {"Top", "Bottom"}).Open())
      {
      case 0:
        AliceCodes::SetTopScreenBackground("BMP/" + files_name[i], false);
        Sleep(Milliseconds(500));
        break;
      case 1:
        AliceCodes::SetBottomScreenBackground("BMP/" + files_name[i], false);
        Sleep(Milliseconds(500));
        break;
      }
    }
  }

  void PlayMusic(MenuEntry *entry)
  {
    StringVector files_name;
    Directory("MUSIC", true).ListFiles(files_name, ".bcwav");
    if (!files_name.size())
    {
      MessageBox("no files found")();
      return;
    }
    s8 i = Keyboard("select file:", files_name).Open();
    if (i != -1)
      Sound("MUSIC/" + files_name[i]).Play();
  }

  void Indicator(MenuEntry *entry)
  {
    u16 max_health, health;
    const Screen &topScr = OSD::GetTopScreen();
    Process::Read16(0x087FB9C2, max_health);
    Process::Read16(0x087FB9C4, health);
    if (health)
    {
      DrawCircle(topScr, 333, 30, 0, 20, int(270 - (360 / (max_health * 1.0 / health * 1.0))), 270, Color::Red, 8);
      topScr.DrawSysfont(Utils::Format("%d/%d", health, max_health), 300, 0);
    }
    Process::Read16(0x087FAFF2, max_health);
    Process::Read16(0x087FAFF4, health);
    if (health)
    {
      DrawCircle(topScr, 200, 30, 0, 20, int(270 - (360 / (max_health * 1.0 / health * 1.0))), 270, Color::Red, 8);
      topScr.DrawSysfont(Utils::Format("%d/%d", health, max_health), 170, 0);
    }
    Process::Read16(0x087FB4DA, max_health);
    Process::Read16(0x087FB4DC, health);
    if (health)
    {
      DrawCircle(topScr, 66, 30, 0, 20, int(270 - (360 / (max_health * 1.0 / health * 1.0))), 270, Color::Red, 8);
      topScr.DrawSysfont(Utils::Format("%d/%d", health, max_health), 30, 0);
    }
  }
}