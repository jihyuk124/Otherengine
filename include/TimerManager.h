#pragma once
#include <functional>
#include <unordered_map>
#include "TimeUtil.h"

namespace Game
{
	struct FTimerHandle;
}

template <>
struct std::hash<Game::FTimerHandle>
{
	size_t operator()(const Game::FTimerHandle& key) const noexcept;
};

namespace Game
{
	struct FTimerHandle
	{
		bool operator==(const FTimerHandle&) const noexcept = default;
		bool operator!=(const FTimerHandle&) const noexcept = default;
		
	private:
		friend class CTimerManager;
		friend std::hash<FTimerHandle>;
		
		static FTimerHandle Create() noexcept;
		
		int key = -1;
	};

	class CTimerManager
	{
	public:
		explicit CTimerManager(class CWorld& world);
		~CTimerManager();
		
		void Update();

		/**
		 * \param fn Return false to stop looping
		 */
		template <class R, class P>
		FTimerHandle SetTimer(duration<R, P> delay, bool loop, std::function<bool()>&& fn)
		{
			return SetTimer(duration_cast<Duration>(delay), loop, std::move(fn));
		}
		
		/**
		 * \param fn Return false to stop looping
		 */
		FTimerHandle SetTimer(Duration delay, bool loop, std::function<bool()>&& fn);

	private:
		struct FTimer;
		
		CWorld& world_;
		std::unordered_map<FTimerHandle, FTimer> timers_;
		std::unordered_map<FTimerHandle, FTimer> pending_timers_;
	};
}