#pragma once
#include "StdInclude.hpp"
#include "GameInterface.hpp"

#include "Structures.hpp"
#include "Hooks.hpp"
#include "Events.hpp"
#include "DemoParser.hpp"
#include "Hooks/Camera.hpp"

namespace IWXMVM::IW3
{
	const auto RESERVED_DEMO_NAME = "RESERVED_IW3MVM_DEMO_NAME";

	class IW3Interface : public GameInterface
	{
	public:

		IW3Interface() : GameInterface(Types::Game::IW3) {}

		void InstallHooks() final
		{
			Hooks::Install();
		}

		void SetupEventListeners() final
		{
			Events::RegisterListener(EventType::OnDemoLoad, DemoParser::Run);

			Events::RegisterListener(EventType::OnCameraChanged, Hooks::Camera::OnCameraChanged);
			
			Events::RegisterListener(EventType::OnDemoLoad, []() { Structures::FindDvar("sv_cheats")->current.enabled = true; });
		}

		uintptr_t GetWndProc() final
		{
			return (uintptr_t)0x57BB20;
		}

		void SetMouseMode(Types::MouseMode mode) final
		{
			Structures::GetMouseVars()->mouseInitialized = (mode == Types::MouseMode::Capture) ? false : true;
		}

		Types::GameState GetGameState() final
		{
			if (!Structures::FindDvar("cl_ingame")->current.enabled)
				return Types::GameState::MainMenu;

			if (Structures::GetClientConnection()->demoplaying)
				return Types::GameState::InDemo;

			return Types::GameState::InGame;
		}

		// TODO: cache this
		Types::DemoInfo GetDemoInfo() final
		{
			const std::filesystem::path path = GetLatestDemoPath();

			Types::DemoInfo demoInfo;
			demoInfo.name = path.filename().string();

			demoInfo.path = path.parent_path().string();

			demoInfo.currentTick = Structures::GetClientActive()->serverTime - DemoParser::demoStartTick;
			demoInfo.endTick = DemoParser::demoEndTick - DemoParser::demoStartTick;

			return demoInfo;
		}

		std::string_view GetDemoExtension() final 
		{
			return { ".dm_1" };
		}

		bool loadModAutomatically{};

		bool& GetAutomaticModLoading()
		{
			return loadModAutomatically;
		}

		std::string latestDemoPath;

		void SetLatestDemoPath(std::string demoPath) final
		{
			assert(demoPath.ends_with(GetDemoExtension()) && std::filesystem::exists(demoPath));
			latestDemoPath = std::move(demoPath);
		}

		const std::string& GetLatestDemoPath() final
		{
			return latestDemoPath;
		}

		bool PlayDemo(const std::filesystem::path& demoPath, bool doDisconnect) final
		{
			if (GetGameState() == Types::GameState::MainMenu || !GetAutomaticModLoading())
			{
				SetLatestDemoPath(demoPath.string());

				Structures::Cbuf_AddText(
					std::format(R"(demo {0})", RESERVED_DEMO_NAME)
				);

				return true;
			} 
			else 
			{
				if (doDisconnect)
				{
					Structures::Cbuf_AddText("disconnect");
				}

				return false;
			}
		}


		bool isPlaybackPaused = false;

		void ToggleDemoPlaybackState() final
		{
			isPlaybackPaused = !isPlaybackPaused;
		}

		bool IsDemoPlaybackPaused() final
		{
			return isPlaybackPaused;
		}

		std::optional<Types::Dvar> GetDvar(const std::string name) final
		{
			const auto iw3Dvar = Structures::FindDvar(name);

			if (!iw3Dvar)
				return std::nullopt;

			Types::Dvar dvar;
			dvar.name = iw3Dvar->name;
			dvar.value = (Types::Dvar::Value*)&iw3Dvar->current;

			return dvar;
		}
	};
}
