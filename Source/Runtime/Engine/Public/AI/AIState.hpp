#pragma once
#include "Name.hpp"

ENGINE_BEGIN

namespace ai_state
{
	class Base
	{
	public:
		explicit Base(class AIComponent& owner) noexcept: owner_{owner} {}
		explicit Base(class AIComponent* owner) noexcept: owner_{*owner} {}
		virtual ~Base() = default;
		virtual void OnEnter([[maybe_unused]] Base& prev) {}
		virtual void Update([[maybe_unused]] Float delta_seconds) {}
		virtual void OnExit([[maybe_unused]] Base& next) {}
		[[nodiscard]] virtual Name GetName() const = 0;
		[[nodiscard]] AIComponent& GetOwner() const noexcept { return owner_; }

	private:
		AIComponent& owner_;
	};
}

ENGINE_END
