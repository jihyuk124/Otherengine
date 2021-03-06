#pragma once
#include "Core.hpp"
#include "Name.hpp"
#include "Templates/Time.hpp"
#include <fmt/compile.h>

/**
 * Namespace for log categories.
 * You should declare/define log categories in logcat namespace.
 */
namespace logcat
{
/**
 * Log category. You can add custom log category with it:
 *
 * In .hpp:
 * @code{cpp}
 * namespace logcat
 * {
 * extern const LogCategory kMyCategory;
 * }
 * @endcode
 * 
 * In .cpp:
 * @code{cpp}
 * namespace logcat
 * {
 * const LogCategory kMyCategory{u8"MyCategory"sv};
 * }
 * @endcode
 *
 * Or simply define it as static in .cpp:
 * @code{cpp}
 * namespace logcat
 * {
 * static const LogCategory kMyCategory{u8"MyCategory"sv};
 * }
 * @endcode
 *
 * @note You should declare/define log categories in logcat namespace.
 */
struct LogCategory
{
    oeng::Name name;
};

using namespace std::string_view_literals;
}

namespace oeng
{
inline namespace core
{
enum class LogLevel
{
    /**
     * Prints a debug message to log file if NDEBUG is not defined.
     */
    kDebug,

    /**
     * Prints a message to log file (and console if NDEBUG is not defined).
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

CORE_API void Log(const logcat::LogCategory& category, LogLevel level, std::u8string_view message);
CORE_API void FlushLog();

template <class Str, class... Args>
void Log(const logcat::LogCategory& category, LogLevel level, const Str& format, const Args&... args)
{
    Log(category, level, std::u8string_view{fmt::format(format, args...)});
}

class CORE_API DelayedLog
{
public:
    DelayedLog();

    void operator()(Duration delay, const logcat::LogCategory& category, LogLevel level,
                    std::u8string_view message) const;

    template <class Rep, class Period, class Str, class... Args>
    void operator()(time::duration<Rep, Period> delay, const logcat::LogCategory& category, LogLevel level,
                    const Str& format, const Args&... args) const
    {
        operator()(time::duration_cast<Duration>(delay), category, level,
                   std::u8string_view{fmt::format(format, args...)});
    }

private:
    size_t id_;
};
}
}

/**
 * Log with delay (anti-spam)
 * @param delay Minimum interval
 * @param category Log category
 * @param level Log level
 * @param format Message format
 */
#define OE_DLOG(delay, category, level, format, ...) [&] \
{ \
	static const ::oeng::DelayedLog log_delay; \
	log_delay(delay, ::logcat::category, ::oeng::LogLevel::level, format, ##__VA_ARGS__); \
}()

/**
 * Log macro that allows you to easily specify categories and levels.
 * @param category Log category. `logcat::` must be omitted.
 * @param level Log level. `LogLevel::` must be omitted.
 * @param format Log format string.
 * @param ... Format arguments.
 */
#define OE_LOG(category, level, format, ...) ::oeng::Log(::logcat::category, ::oeng::LogLevel::level, format, ##__VA_ARGS__)
