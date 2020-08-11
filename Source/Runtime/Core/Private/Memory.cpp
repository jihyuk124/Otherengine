#include "Memory.hpp"
#include <sstream>
#include <thread>

namespace logcat
{
	const LogCategory kMemory{u8"Memory"sv};
}

namespace oeng::core
{
	struct MemPoolStorage
	{
		DELETE_CPMV(MemPoolStorage);
		MemPoolStorage() {}
		~MemPoolStorage() {}
		union { omem::MemoryPoolManager pool; };
		bool is_initialized = false;
	};

	static thread_local MemPoolStorage storage;
	
	void InitMemPool()
	{
		assert(!storage.is_initialized);
		new (&storage.pool) omem::MemoryPoolManager;
		storage.is_initialized = true;
	}

	omem::MemoryPoolManager& GetMemPool() noexcept
	{
		assert(storage.is_initialized);
		return storage.pool;
	}

	static std::u8string ThreadId()
	{
		std::stringstream ss;
		ss << std::this_thread::get_id();
		return AsString8(ss.str());
	}

	static void LogInfo()
	{
#ifndef NDEBUG
		using namespace otm;
		const auto& pools = storage.pool.Pools();
		
		omem::PoolInfo max;
		std::vector<omem::PoolInfo> infos;
		infos.reserve(pools.size());
		
		for (const auto& [size, pool] : pools)
		{
			auto compare = [](const omem::PoolInfo& a, const omem::PoolInfo& b) noexcept
			{
				return a.size < b.size;
			};
			const auto& info = pool.GetInfo();
			const auto pos = std::upper_bound(infos.begin(), infos.end(), info, compare);
			infos.insert(pos, info);

			max.size = Max(max.size, info.size);
			max.count = Max(max.count, info.count);
			max.cur = Max(max.cur, info.cur);
			max.peak = Max(max.peak, info.peak);
			max.fault = Max(max.fault, info.fault);
		}
		
		omem::PoolInfo align;
		align.size = IntLog(max.size, 10) + 1;
		align.count = IntLog(max.count, 10) + 1;
		align.peak = IntLog(max.peak, 10) + 1;
		align.fault = IntLog(max.fault, 10) + 1;
		
		OE_LOG(kMemory, kDebug, fmt::format(u8"[Mem] Memory pool info for thread {}:"sv, ThreadId()));
		for (const auto& info : infos)
		{
			OE_LOG(kMemory, kDebug, fmt::format(u8"[Mem] {:>{}}-byte blocks, total: {:>{}}, peak: {:>{}}, fault: {:>{}}, in use: {}"sv,
				info.size, align.size, info.count, align.count, info.peak, align.peak, info.fault, align.fault, info.cur));
		}
		if (max.cur > 0) OE_LOG(kMemory, kWarn, u8"[Mem] Memory leak detected"sv);
#endif
	}

	void CleanUpMemPool() noexcept
	{
		assert(storage.is_initialized);
		storage.is_initialized = false;
		EXPECT_NO_THROW(LogInfo());
		storage.pool.~MemoryPoolManager();
	}
}
