#pragma once
#include "UI/UIComponent.hpp"

namespace IWXMVM::UI
{
	class TracerMenu : public UIComponent
	{
	public:
		TracerMenu()
		{
			Initialize();
		}
		~TracerMenu()
		{
			Release();
		}

		void Render() final;
		void Release() final;

	private:
		void Initialize() final;
	};
}