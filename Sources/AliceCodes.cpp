#include "AliceCodes.hpp"
#include <CTRPluginFramework.hpp>
#include <CTRPluginFrameworkImpl.hpp>

namespace CTRPluginFramework
{
  Result AliceCodes::SetTopScreenBackground(const std::string &filename, bool just_size)
  {
    BMPImage *image = new BMPImage(filename);

    if (!image->IsLoaded())
    {
      delete image;
      return -1;
    }

    if (just_size && (image->Width() != 340 || image->Height() != 200))
    {
      delete image;
      MessageBox(Color::Red << "An error has occurred.\nfile size is wrong")();
      return -1;
    }

    Preferences::topBackgroundImage = image;
    Window::UpdateBackgrounds();
    return 0;
  }

  Result AliceCodes::SetBottomScreenBackground(const std::string &filename, bool just_size)
  {
    BMPImage *image = new BMPImage(filename);

    if (!image->IsLoaded())
    {
      delete image;
      return -1;
    }

    if (just_size && (image->Width() != 280 || image->Height() != 200))
    {
      delete image;
      MessageBox(Color::Red << "An error has occurred.\nfile size is wrong")();
      return -1;
    }

    Preferences::bottomBackgroundImage = image;
    Window::UpdateBackgrounds();
    return 0;
  }
}