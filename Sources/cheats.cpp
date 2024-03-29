#include "cheats.hpp"
#include "KaniCodes.hpp"
#include "JPKeyboard.hpp"
#include "Convert.hpp"

namespace CTRPluginFramework
{
  void Test(MenuEntry *entry)
  {
  }

  void YokaiEditor(MenuEntry *entry)
  {
    static bool isOpened = false;
    if (!System::IsCitra() && *(u32 *)(*(u32 *)0x8594AC + 8) != 0x804)
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
    StringVector poses = {"きめポーズ1", "きめポーズ2", "きめポーズ3", "きめポーズ4", "おまかせ", "", "", "", "", "", "", "", "", "", "", ""};

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
            yokaiName = ReadSJIS(*(u32 *)(*(u32 *)(ModelAddress + 0x04)));
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
      Process::Pause();
    }
    if (Controller::IsKeyPressed(Key::Start))
    {
      if (Process::IsPaused())
        ForcePlay();
      isOpened = !isOpened;
    }
    auto position = Touch::GetPosition();
    if (Controller::IsKeyPressed(Touchpad) && isOpened)
    {
      if (TouchRect(275, 30, 15, 15))
      {
        if (Process::IsPaused())
          ForcePlay();
        isOpened = !isOpened;
      }
      else if (TouchRect(130, 48, 150, 19))
      {
        Keyboard select("select keyboard:", {"HEX", "キーボード"});

        int answer = select.Open();
        std::string out;
        switch (answer)
        {
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
            if (0 <= key.Open(buff1))
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
        {
          if (JPKeyboard("妖怪の名前を入れてください").Open(out))
          {
            std::vector<u8> sjis;
            std::vector<u16> buff = Convert::strToSjis(out);
            for (size_t i = 0; i < buff.size(); i++)
            {
              sjis.push_back(buff[i] / 0x100);
              sjis.push_back(buff[i] & 0xFF);
            }
            for (size_t j = 0; j < 24; j++)
            {
              Process::Write8(0x870DBBC + offset + j, 0x00);
            }
            for (size_t j = 0; j < (sjis.size() > 23 ? 23 : sjis.size()); j++)
            {
              Process::Write8(0x870DBBC + offset + j, sjis[j]);
            }
          }
          break;
        }
        }
        ForcePlay();
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
          if (JPKeyboard("妖怪の名前を入れてください").Open(out))
          {
            StringVector yokaiNames;
            std::vector<u32> yokaiIDs;
            for (size_t i = 0; i < 706; i++)
            {
              u32 ModelAddress = 0;
              for (size_t y = 0; y < 602; y++)
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
                if (ReadSJIS(*(u32 *)(*(u32 *)(ModelAddress + 0x04))).find(out) != std::string::npos)
                {
                  yokaiNames.push_back(ReadSJIS(*(u32 *)(*(u32 *)(ModelAddress + 0x04))));
                  yokaiIDs.push_back((*(u32 *)(0x08576864 + (i * 0x84))));
                }
              }
            }
            ans = Keyboard("select", yokaiNames).Open();
            if (0 <= ans)
            {
              Process::Write32(0x870DBB8 + offset, yokaiIDs[ans]);
            }
          }
          break;
        }
        case 1:
        {
          u32 buff;
          Keyboard key("妖怪ID:");
          key.IsHexadecimal(true);
          key.SetMaxLength(8);
          if (0 <= key.Open(buff))
          {
            Process::Write32(0x870DBB8 + offset, buff);
          }
        }
        break;
        }
        ForcePlay();
      }
      else if (TouchRect(120, 90, 50, 20))
      {
        Sleep(Milliseconds(200));
        if (0 <= Keyboard("レベル:").Open(level))
          Process::Write8(0x870DC03 + offset, level);
        ForcePlay();
      }
      else if (TouchRect(120, 127, 50, 15))
      {
        Sleep(Milliseconds(200));
        if (0 <= Keyboard("HP:").Open(hp))
          Process::Write16(0x870DBEC + offset, hp);
        ForcePlay();
      }
      else if (TouchRect(120, 144, 50, 15))
      {
        Sleep(Milliseconds(200));
        if (0 <= Keyboard("ちから:").Open(power))
          Process::Write16(0x870DC16 + offset, power);
        ForcePlay();
      }
      else if (TouchRect(120, 161, 50, 15))
      {
        Sleep(Milliseconds(200));
        if (0 <= Keyboard("ようりょく:").Open(magic))
          Process::Write16(0x870DC18 + offset, magic);
        ForcePlay();
      }
      else if (TouchRect(120, 178, 50, 15))
      {
        Sleep(Milliseconds(200));
        if (0 <= Keyboard("まもり:").Open(protect))
          Process::Write16(0x870DC1A + offset, protect);
        ForcePlay();
      }
      else if (TouchRect(120, 195, 50, 16))
      {
        Sleep(Milliseconds(200));
        if (0 <= Keyboard("すばやさ:").Open(speed))
        {
          Process::Write16(0x870DC1C + offset, speed);
        }
        ForcePlay();
      }
      else if (TouchCircle(210, 190, 25))
      {
        Sleep(Milliseconds(200));
        Keyboard select("select mode:\n保存するときの名前入力してね", {"ニックネーム", "ステータス", "妖怪ID"});
        int answer = select.Open();
        std::string out;
        if (answer != -1)
        {
          Keyboard("").Open(out);
          if (out.empty())
            answer = -1;
        }
        ForcePlay();
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
        ForcePlay();
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
          for (size_t i = 0; i < files_name.size(); i++)
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
          for (size_t i = 0; i < files_name.size(); i++)
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
          for (size_t i = 0; i < files_name.size(); i++)
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
          ForcePlay();
          if (j != -1)
            Process::Write8(0x870DC08 + offset, i * 0x10 + j);
        }
      }
      else if (TouchRect(230, 95, 50, 25))
      {
        Sleep(Milliseconds(100));
        s8 i = Keyboard(poses).Open();
        ForcePlay();
        if (i != -1)
          Process::Write8(0x870DC04 + offset, i);
      }
      Sleep(Milliseconds(100));
    }
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