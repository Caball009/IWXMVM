#include "StdInclude.hpp"
#include "FileOpen.h"

#include "Utilities/HookManager.hpp"
#include "Mod.hpp"

#include "UI/UIManager.hpp"
#include "../IW3Interface.hpp"

namespace IWXMVM::IW3::Hooks::FileOpen
{
	void CheckFilePath(const char** filePathPtr)
	{
		const std::string_view sv(*filePathPtr, std::find(*filePathPtr, *filePathPtr + 256, '\0'));

		if (sv.ends_with(Mod::GetGameInterface()->GetDemoExtension())) 
		{
			if (sv.find(RESERVED_DEMO_NAME) != std::string::npos)
			{
				*filePathPtr = Mod::GetGameInterface()->GetLatestDemoPath().c_str();
			}
			else if (std::filesystem::exists(sv)) 
			{
				Mod::GetGameInterface()->SetLatestDemoPath(std::string{ sv });
			} 
		}
	}

	std::uintptr_t fopen_Trampoline;
	void __declspec(naked) fopen_Hook()
	{
		static const char** filePathPtr{};

		_asm
		{
			pushad
			lea eax, [esp + 0x4 + 0x20]
			mov filePathPtr, eax
		}

		CheckFilePath(filePathPtr);

		_asm
		{
			popad
			jmp fopen_Trampoline
		}
	}

	std::uintptr_t CL_Vid_Restart_f_Trampoline;
	void __declspec(naked) CL_Vid_Restart_f_Hook()
	{
		_asm pushad

		UI::UIManager::RestartImGui();

		_asm
		{
			popad
			jmp CL_Vid_Restart_f_Trampoline
		}
	}

	bool CheckAutomaticModLoading()
	{
		return reinterpret_cast<IW3Interface*>(Mod::GetGameInterface())->GetAutomaticModLoading();
	}

	std::uintptr_t CL_SystemInfoChanged_CoD4X_Trampoline;
	_declspec(naked) void CL_SystemInfoChanged_CoD4X_Hook()
	{
		_asm pushad

		if (CheckAutomaticModLoading()) {
			_asm
			{
				popad
				jmp CL_SystemInfoChanged_CoD4X_Trampoline
			}
		} 
		else 
		{
			_asm
			{
				popad
				ret 
			}
		}
	}

	std::uintptr_t CL_SystemInfoChanged_Trampoline;
	_declspec(naked) void CL_SystemInfoChanged_Hook()
	{
		_asm pushad

		if (CheckAutomaticModLoading()) 
		{
			_asm
			{
				popad
				mov dword ptr ds : [0x934E74], 0 // clc_demoplaying
				call CL_SystemInfoChanged_Trampoline
				mov dword ptr ds : [0x934E74], 1
				ret
			}
		}
		else 
		{
			_asm
			{
				popad
				jmp CL_SystemInfoChanged_Trampoline
			}
		}
	}

	HMODULE GetCoD4xModuleHandle()
	{
		std::string moduleName = "cod4x_021.dll";
		MODULEINFO moduleData{};

		for (std::size_t i = 0; i < 1000; ++i) {
			if (!::GetModuleInformation(::GetCurrentProcess(), ::GetModuleHandle(moduleName.c_str()), &moduleData, sizeof(moduleData)) || moduleData.lpBaseOfDll) {
				break;
			}

			moduleName = "cod4x_" + std::format("{:03}", i) + ".dll";
		}

		return static_cast<HMODULE>(moduleData.lpBaseOfDll);
	}

	void Install()
	{
		if (*reinterpret_cast<std::uint8_t*>(0x671E8F) != 0xE9) // is not hooked (by CoD4X)
		{
			HookManager::CreateHook(0x671E8F, (std::uintptr_t)fopen_Hook, &fopen_Trampoline);
			HookManager::CreateHook(0x46A180, (std::uintptr_t)CL_Vid_Restart_f_Hook, &CL_Vid_Restart_f_Trampoline);
			HookManager::CreateHook(0x473AB0, (std::uintptr_t)CL_SystemInfoChanged_Hook, &CL_SystemInfoChanged_Trampoline);
		} 
		else 
		{
			const std::uintptr_t address = 0x671E8F + *reinterpret_cast<std::uintptr_t*>(0x671E8F + 1) + 5;

			if (*reinterpret_cast<std::uint8_t*>(address) == 0xFF) 
			{
				HookManager::CreateHook(address, (std::uintptr_t)fopen_Hook, &fopen_Trampoline);
			}

			const std::uintptr_t baseAddress = reinterpret_cast<std::uintptr_t>(GetCoD4xModuleHandle());

			if (baseAddress != 0)
			{
				// TODO: use signature: 55 89 E5 53 B8 ?? ?? 00 00 E8 ?? ?? ?? ?? 29 C4 C7 04 24 01 00 00 00 E8
				//						?? ?? ?? ?? ?? ?? ?? 00 00 E8 ?? ?? ?? ?? 29 C4 C7 04 24 01 00 00 00 E8
				HookManager::CreateHook(baseAddress + 0x1DA49, (std::uintptr_t)CL_SystemInfoChanged_CoD4X_Hook, &CL_SystemInfoChanged_CoD4X_Trampoline);
			}
		}
	}
}