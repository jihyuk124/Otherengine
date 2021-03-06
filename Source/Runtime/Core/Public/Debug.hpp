#pragma once
#include "Log.hpp"
#include "Platform.hpp"

namespace logcat
{
extern CORE_API const LogCategory kDebug;
}

#ifdef NDEBUG

#define DEBUG_BREAK() (void)0

#define ENSURE(expr) !!(expr)
#define ENSURE_MSG(expr, fmt, ...) !!(expr)

#define EXPECT(expr) (void)0
#define EXPECT_MSG(expr, fmt, ...) (void)0

#else

#ifdef _WIN32

/**
 * Trigger breakpoint only if debugger is attached.
 */
#define DEBUG_BREAK() (void)(::oeng::IsDebugging() && (::__debugbreak(), true))

#else

#include <csignal>
#define DEBUG_BREAK() (void)(::oeng::IsDebugging() && (::std::raise(SIGTRAP), true))

#endif

/**
 * Can be used to test for *non-fatal* errors at runtime.
 *
 * Rather than crashing, an error will be logged. 
 * This is useful when you want runtime code verification but you're handling the error case anyway.
 *
 * Can be nested within conditionals! Example:
 * 
 * @code{cpp}
 * if (ENSURE(InObject != nullptr))
 * {
 *     InObject->Modify();
 * }
 * @endcode
 * 
 * This code is safe to execute as the pointer dereference is wrapped in a non-nullptr conditional
 * block, but you still want to find out if this ever happens so you can avoid side effects.
 *
 * Resolves to just evaluate the expression when NDEBUG is defined.
 */
#define ENSURE(expr) (!!(expr) || (OE_ELOG(kDebug, u8"Ensure failed: " u8 ## #expr u8", file " U8_FILE u8", line " U8_LINE), false))

/**
 * Same as ENSURE(), but it can log additional information that can be helpful.
 * 
 * Supports formatting. Example:
 * 
 * @code{cpp}
 * if (ENSURE_MSG(count == 0, u8"count must be 0, but it was {}", count))
 * {
 *     ...
 * }
 * @endcode
 */
#define ENSURE_MSG(expr, fmt, ...) (!!(expr) || (OE_ELOG(kDebug, u8"Ensure failed: " u8 ## #expr u8", " fmt u8", file " U8_FILE u8", line " U8_LINE, ##__VA_ARGS__), DEBUG_BREAK(), false))

/**
 * Same as ENSURE(), but it **CAN'T** be nested within conditionals.
 * Resolves to empty expression when NDEBUG is defined.
 */
#define EXPECT(expr) (void)(!!(expr) || (OE_ELOG(kDebug, u8"Expect failed: " u8 ## #expr u8", file " U8_FILE u8", line " U8_LINE), DEBUG_BREAK(), false))

/**
 * Same as EXPECT(), but it can log additional information that can be helpful.
 * Supports formatting. See ENSURE_MSG()
 */
#define EXPECT_MSG(expr, fmt, ...) (void)(!!(expr) || (OE_ELOG(kDebug, u8"Expect failed: " u8 ## #expr u8", " fmt u8", file " U8_FILE u8", line " U8_LINE, ##__VA_ARGS__), DEBUG_BREAK(), false))

#endif

#define EXPECT_NO_ENTRY() EXPECT(!u8"Enclosing block should never be called")

/**
 * Try to execute the expression, and if an exception thrown, log the error and swallow the exception.
 */
#define EXPECT_NO_THROW(expr) do { try { expr; } catch (const std::exception& e) { EXPECT_MSG(!u8"No exceptions should be thrown", u8"{}: {}"sv, u8 ## #expr ## sv, What(e)); } } while (false)

/**
 * Log error with anti-spam and debug break.
 * @param fmt Formatted message to log
 */
#define OE_ELOG(category, format, ...) (OE_DLOG(1s, category, kErr, format, ##__VA_ARGS__), DEBUG_BREAK())
