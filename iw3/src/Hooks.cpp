#include "StdInclude.hpp"
#include "Hooks.hpp"

#include "Hooks/Commands.hpp"
#include "Hooks/Playback.hpp"
#include "Hooks/Camera.hpp"
#include "Hooks/FileOpen.h"


namespace IWXMVM::IW3::Hooks
{
	void Install()
	{
		Hooks::FileOpen::Install();
		Hooks::Playback::Install();
		Hooks::Commands::Install();
		Hooks::Camera::Install();
	}
}
