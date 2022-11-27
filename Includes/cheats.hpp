#ifndef CHEATS_H
#define CHEATS_H

#include <CTRPluginFramework.hpp>
#include "Helpers.hpp"
#include "Unicode.h"

namespace CTRPluginFramework
{
    using StringVector = std::vector<std::string>;

    void Cheat1(MenuEntry *entry);
    void Test1(MenuEntry *entry);
    void Pipes(MenuEntry *entry);
    void Search(MenuEntry *entry);
    void YokaiEditor(MenuEntry *entry);
    void Cube(MenuEntry *entry);
    void BadApple(MenuEntry *entry);
    void JPNotify(MenuEntry *entry);
    void ChangeBackGround(MenuEntry *entry);

    std::vector<u64> getFrame(int &frame);

    void japKey(std::string &out, std::vector<u8> &sjis);
    bool TouchRect(u32 x, u32 y, u32 w, u32 h);
    bool TouchCircle(u32 x, u32 y, u8 size);
}
#endif