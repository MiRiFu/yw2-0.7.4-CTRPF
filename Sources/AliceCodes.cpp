#include "AliceCodes.hpp"
#include <CTRPluginFramework.hpp>
#include <CTRPluginFrameworkImpl.hpp>

namespace CTRPluginFramework
{
  Result AliceCodes::SetTopScreenBackground(const std::string &filename)
  {
    BMPImage *image = new BMPImage(filename);

    if (!image->IsLoaded() || image->Width() != 340 || image->Height() != 200)
    {
      delete image;
      return -1;
    }

    Preferences::topBackgroundImage = image;
    Window::UpdateBackgrounds();
    return 1;
  }

  Result AliceCodes::SetBottomScreenBackground(const std::string &filename)
  {
    BMPImage *image = new BMPImage(filename);

    if (!image->IsLoaded() || image->Width() != 280 || image->Height() != 200)
    {
      delete image;
      return -1;
    }

    Preferences::bottomBackgroundImage = image;
    Window::UpdateBackgrounds();
    return 1;
  }
}