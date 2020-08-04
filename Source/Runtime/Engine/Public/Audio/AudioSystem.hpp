#pragma once
#include "Core.hpp"
#include "EngineFwd.hpp"
#include "MathFwd.hpp"

namespace FMOD
{
	namespace Studio
	{
		class System;
	}
	class System;
}

namespace oeng::core
{
	class ENGINE_API AudioSystem
	{
	public:
		DELETE_CPMV(AudioSystem);
		
		AudioSystem();
		~AudioSystem();

		void Update(Float delta_seconds) const;

	private:
		FMOD::Studio::System* system_;
		FMOD::System* core_system_;
	};
}