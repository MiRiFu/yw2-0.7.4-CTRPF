#ifndef CHEATS_H
#define CHEATS_H

#include <CTRPluginFramework.hpp>
#include "Helpers.hpp"
#include "Unicode.h"

namespace CTRPluginFramework
{
  using StringVector = std::vector<std::string>;

  void Test(MenuEntry *entry);
  void YokaiEditor(MenuEntry *entry);
  void Indicator(MenuEntry *entry);

}
#endif