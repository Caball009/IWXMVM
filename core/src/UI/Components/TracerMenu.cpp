#include "StdInclude.hpp"
#include "TracerMenu.hpp"

#include "Mod.hpp"

// TODO: there are reads/writes from different threads, so these variables should be atomic...
bool resetTracers = false;
float beginColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
float endColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

namespace IWXMVM::UI
{
	void TracerMenu::Initialize()
	{}

	void TracerMenu::Render()
	{
		const std::optional<Dvar> cg_firstPersonTracerChance = Mod::GetGameInterface()->GetDvar("cg_firstPersonTracerChance");
		const std::optional<Dvar> cg_tracerchance = Mod::GetGameInterface()->GetDvar("cg_tracerchance");
		const std::optional<Dvar> cg_tracerlength = Mod::GetGameInterface()->GetDvar("cg_tracerlength");
		const std::optional<Dvar> cg_tracerScale = Mod::GetGameInterface()->GetDvar("cg_tracerScale");
		const std::optional<Dvar> cg_tracerScaleDistRange = Mod::GetGameInterface()->GetDvar("cg_tracerScaleDistRange");
		const std::optional<Dvar> cg_tracerScaleMinDist = Mod::GetGameInterface()->GetDvar("cg_tracerScaleMinDist");
		const std::optional<Dvar> cg_tracerScrewDist = Mod::GetGameInterface()->GetDvar("cg_tracerScrewDist");
		const std::optional<Dvar> cg_tracerScrewRadius = Mod::GetGameInterface()->GetDvar("cg_tracerScrewRadius");
		const std::optional<Dvar> cg_tracerSpeed = Mod::GetGameInterface()->GetDvar("cg_tracerSpeed");
		const std::optional<Dvar> cg_tracerwidth = Mod::GetGameInterface()->GetDvar("cg_tracerwidth");

		if (!cg_firstPersonTracerChance.has_value() ||
			!cg_tracerchance.has_value() ||
			!cg_tracerlength.has_value() ||
			!cg_tracerScale.has_value() ||
			!cg_tracerScaleDistRange.has_value() ||
			!cg_tracerScaleMinDist.has_value() ||
			!cg_tracerScrewDist.has_value() ||
			!cg_tracerScrewRadius.has_value() ||
			!cg_tracerSpeed.has_value() ||
			!cg_tracerwidth.has_value()
			) 
		{
			return;
		}

		ImGui::Begin("Tracer settings", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
		ImGui::SliderFloat("Chance self", &cg_firstPersonTracerChance.value().value->floating_point, 0.0f, 1.0f, "%.3f", ImGuiSliderFlags_NoInput);
		ImGui::SliderFloat("Chance players", &cg_tracerchance.value().value->floating_point, 0.0f, 1.0f, "%.3f", ImGuiSliderFlags_NoInput);
		ImGui::SliderFloat("Length", &cg_tracerlength.value().value->floating_point, 0.0f, 10'000.0f, "%.1f", ImGuiSliderFlags_Logarithmic | ImGuiSliderFlags_NoInput);
		ImGui::SliderFloat("Scale", &cg_tracerScale.value().value->floating_point, 1.0f, 10'000.0f, "%.1f", ImGuiSliderFlags_Logarithmic | ImGuiSliderFlags_NoInput);
		ImGui::SliderFloat("ScaleDistRange", &cg_tracerScaleDistRange.value().value->floating_point, 0.0f, 100'000.0f, "%.1f", ImGuiSliderFlags_Logarithmic | ImGuiSliderFlags_NoInput);
		ImGui::SliderFloat("ScaleMinDist", &cg_tracerScaleMinDist.value().value->floating_point, 0.0f, 10'000.0f, "%.1f", ImGuiSliderFlags_Logarithmic | ImGuiSliderFlags_NoInput);
		ImGui::SliderFloat("ScrewDist", &cg_tracerScrewDist.value().value->floating_point, 0.0f, 1000.0f, "%.1f", ImGuiSliderFlags_Logarithmic | ImGuiSliderFlags_NoInput);
		ImGui::SliderFloat("ScrewRadius", &cg_tracerScrewRadius.value().value->floating_point, 0.0f, 100.0f, "%.1f", ImGuiSliderFlags_Logarithmic | ImGuiSliderFlags_NoInput);
		ImGui::SliderFloat("Speed", &cg_tracerSpeed.value().value->floating_point, 0.0f, 100'000.0f, "%.1f", ImGuiSliderFlags_Logarithmic | ImGuiSliderFlags_NoInput);
		ImGui::SliderFloat("Width", &cg_tracerwidth.value().value->floating_point, 0.0f, 1000.0f, "%.1f", ImGuiSliderFlags_Logarithmic | ImGuiSliderFlags_NoInput);
		ImGui::ColorEdit4("Begin color", beginColor);
		ImGui::ColorEdit4("End color", endColor);
		ImGui::Checkbox("Remove current tracers", &resetTracers);

		bool resetDefaultValues = false;
		ImGui::Checkbox("Set default settings", &resetDefaultValues);
		if (resetDefaultValues) 
		{
			cg_firstPersonTracerChance.value().value->floating_point = 0.5f;
			cg_tracerchance.value().value->floating_point = 0.2f;
			cg_tracerlength.value().value->floating_point = 160.0f;
			cg_tracerScale.value().value->floating_point = 1.0f;
			cg_tracerScaleDistRange.value().value->floating_point = 25'000.0f;
			cg_tracerScaleMinDist.value().value->floating_point = 5000.0f;
			cg_tracerScrewDist.value().value->floating_point = 100.0f;
			cg_tracerScrewRadius.value().value->floating_point = 0.5f;
			cg_tracerSpeed.value().value->floating_point = 7500.0f;
			cg_tracerwidth.value().value->floating_point = 4.0f;

			beginColor[0] = 1.0f;
			beginColor[1] = 1.0f;
			beginColor[2] = 1.0f;
			beginColor[3] = 1.0f;
			endColor[0] = 1.0f;
			endColor[1] = 1.0f;
			endColor[2] = 1.0f;
			endColor[3] = 1.0f;
		}

		ImGui::End();
	}

	void TracerMenu::Release()
	{}
}