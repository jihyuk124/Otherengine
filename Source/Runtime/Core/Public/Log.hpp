#pragma once
#include "Templates/Time.hpp"

/**
 * Try to execute the expression; if an exception thrown, swallow the exception if NDEBUG is defined; otherwise, abort the app.
 */
#define ASSERT_TRY(expr) do { try { expr; } catch (...) { assert(!"Critical exception"); } } while (false)

#ifndef DOXYGEN_SHOULD_SKIP_THIS
namespace spdlog
{
	class logger;
}
#endif

namespace oeng::core
{
	class CoreSystem;
}

namespace oeng::core::log
{
#ifdef OE_LOG_THREADSAFE
	constexpr auto kLogThreadSafe = true;
#else
	constexpr auto kLogThreadSafe = false;
#endif

	enum class Level
	{
		/**
		 * Prints a message to console and log file (does not print if NDEBUG is defined).
		 */
		kDebug,

		/**
		 * Prints a message to a log file (does not print to console if NDEBUG is defined).
		 */
		kLog,

		/**
		 * Prints a message to console and log file.
		 */
		kDisplay,

		/**
		 * Prints a warning to console and log file.
		 */
		kWarn,
		
		/**
		 * Prints an error to console and log file.
		 */
		kErr,

		/**
		 * Prints a critical error to console and log file.
		 */
		kCritical
	};

	struct LogCategory
	{
		std::u8string_view name;
		Level min_level = Level::kDebug;
	};

	class CORE_API Logger
	{
	public:
		void Log(Level level, std::u8string_view message) const;
		void LogDelay(unsigned id, Duration delay, Level level, std::u8string_view msg);
		
	private:
		friend CoreSystem;
		Logger();
		
		std::shared_ptr<spdlog::logger> console_;
		std::shared_ptr<spdlog::logger> file_;

		// Should NOT use memory pool
		CondMonitor<std::unordered_map<unsigned, TimePoint>, kLogThreadSafe> delayed_;
	};
	
	CORE_API void Log(Level level, std::u8string_view message);

	template <class... Args>
	void Log(Level level, std::u8string_view fmt, const Args&... args)
	{
		Log(level, Format(fmt, args...));
	}
	
	namespace detail
	{
		class CORE_API LogDelay
		{
		public:
			LogDelay();
			
			void operator()(Duration delay, Level level, std::u8string_view msg) const;

			template <class Rep, class Period>
			void operator()(time::duration<Rep, Period> delay, Level level, std::u8string_view msg) const
			{
				operator()(time::duration_cast<Duration>(delay), level, msg);
			}

			template <class Rep, class Period, class... Args>
			void operator()(time::duration<Rep, Period> delay, Level level, std::u8string_view fmt, const Args&... args) const
			{
				operator()(time::duration_cast<Duration>(delay), level, Format(fmt, args...));
			}

		private:
			unsigned id_;
		};
	}
}

/**
 * Log with delay (anti-spam)
 * @param delay time::duration
 * @param level log::level
 * @param format message format
 */
#define OE_DLOG(delay, level, format, ...) [&] \
{ \
	static const ::oeng::log::detail::LogDelay log_delay; \
	log_delay(delay, level, format, ##__VA_ARGS__); \
}()
