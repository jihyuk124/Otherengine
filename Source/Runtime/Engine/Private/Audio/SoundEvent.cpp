#include "Audio/SoundEvent.hpp"
#include "fmod_studio.hpp"

namespace oeng::engine
{
	void SoundEvent::Restart() const
	{
		instance_->start();
	}

	void SoundEvent::Stop(bool allow_fadeout) const noexcept
	{
		instance_->stop(allow_fadeout ? FMOD_STUDIO_STOP_ALLOWFADEOUT : FMOD_STUDIO_STOP_IMMEDIATE);
	}

	void SoundEvent::SetPaused(bool paused) const noexcept
	{
		instance_->setPaused(paused);
	}

	void SoundEvent::SetVolume(float volume) const noexcept
	{
		instance_->setVolume(volume);
	}

	void SoundEvent::SetPitch(float pitch) const noexcept
	{
		instance_->setPitch(pitch);
	}

	void SoundEvent::SetParam(const char* name, float value) const
	{
		instance_->setParameterByName(name, value);
	}

	bool SoundEvent::IsPaused() const noexcept
	{
		bool paused;
		instance_->getPaused(&paused);
		return paused;
	}

	float SoundEvent::GetVolume() const noexcept
	{
		float volume;
		instance_->getVolume(&volume);
		return volume;
	}

	float SoundEvent::GetPitch() const noexcept
	{
		float pitch;
		instance_->getPitch(&pitch);
		return pitch;
	}

	float SoundEvent::GetParam(const char* name) const noexcept
	{
		float value;
		instance_->getParameterByName(name, &value);
		return value;
	}
}