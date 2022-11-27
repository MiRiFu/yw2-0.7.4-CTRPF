#ifndef CTRPLUGINFRAMEWORK_ALICECODES_HPP
#define CTRPLUGINFRAMEWORK_ALICECODES_HPP

#include <CTRPluginFramework.hpp>
#include "types.h"

namespace CTRPluginFramework
{
	class AliceCodes
	{
	public:
		static Result SetTopScreenBackground(const std::string &filename, bool just_size = true);
		static Result SetBottomScreenBackground(const std::string &filename, bool just_size = true);
	};
}

#endif