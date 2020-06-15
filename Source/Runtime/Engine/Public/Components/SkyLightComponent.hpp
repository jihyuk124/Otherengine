#pragma once
#include "Components/SceneComponent.hpp"
#include "Interfaces/Light.hpp"

namespace oeng
{
	class OEAPI SkyLightComponent : public SceneComponent, public ISkyLight
	{
	public:
		explicit SkyLightComponent(AActor& owner, int update_order = 100)
			:SceneComponent{owner, update_order}, color_{0.1, 0.1, 0.1}
		{
		}

		~SkyLightComponent();

		void Activate() const noexcept;

		void SetColor(const Vec3& color) noexcept { color_ = color; }
		[[nodiscard]] const Vec3& GetColor() const noexcept override { return color_; }

	private:
		Vec3 color_;
	};
}