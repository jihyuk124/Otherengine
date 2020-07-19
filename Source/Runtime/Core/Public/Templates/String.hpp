#pragma once
#include <string>
#include "Memory.hpp"

namespace oeng::core
{
	using namespace std::literals;
	
	using String = std::basic_string<char, std::char_traits<char>, PoolAllocator<char>>;
	using String8 = std::basic_string<char8_t, std::char_traits<char8_t>, PoolAllocator<char8_t>>;
	using String16 = std::basic_string<char16_t, std::char_traits<char16_t>, PoolAllocator<char16_t>>;

	/**
	 * Compares two strings case-insensitively
	 * @return True if two strings are equivalent
	 */
	template <class T>
	[[nodiscard]] constexpr bool StrEqCi(std::basic_string_view<T> a, std::basic_string_view<T> b) noexcept
	{
		if (a.size() != b.size()) return false;
		for (size_t i = 0; i < a.size(); ++i)
			if (std::tolower(a[i]) != std::tolower(b[i]))
				return false;
		return true;
	}

	template <class T, class Tr, class Al1, class Al2>
	[[nodiscard]] constexpr bool StrEqCi(const std::basic_string<T, Tr, Al1>& a, const std::basic_string<T, Tr, Al2>& b) noexcept
	{
		return StrEqCi(std::basic_string_view<T>{a}, std::basic_string_view<T>{b});
	}

	template <class T>
	[[nodiscard]] constexpr bool StrEqCi(const T* a, const T* b) noexcept
	{
		return StrEqCi(std::basic_string_view<T>{a}, std::basic_string_view<T>{b});
	}

	template <template <class> class Tr, template <class> class Al>
	decltype(auto) AsString(std::basic_string<char8_t, Tr<char8_t>, Al<char8_t>>& s) noexcept
	{
		return reinterpret_cast<std::basic_string<char, Tr<char>, Al<char>>&>(s);
	}

	template <template <class> class Tr, template <class> class Al>
	decltype(auto) AsString(std::basic_string<char8_t, Tr<char8_t>, Al<char8_t>>&& s) noexcept
	{
		return reinterpret_cast<std::basic_string<char, Tr<char>, Al<char>>&&>(s);
	}

	template <template <class> class Tr, template <class> class Al>
	decltype(auto) AsString(const std::basic_string<char8_t, Tr<char8_t>, Al<char8_t>>& s) noexcept
	{
		return reinterpret_cast<const std::basic_string<char, Tr<char>, Al<char>>&>(s);
	}

	template <template <class> class Tr, template <class> class Al>
	decltype(auto) AsString8(std::basic_string<char, Tr<char>, Al<char>>& s) noexcept
	{
		return reinterpret_cast<std::basic_string<char8_t, Tr<char8_t>, Al<char8_t>>&>(s);
	}

	template <template <class> class Tr, template <class> class Al>
	decltype(auto) AsString8(std::basic_string<char, Tr<char>, Al<char>>&& s) noexcept
	{
		return reinterpret_cast<std::basic_string<char8_t, Tr<char8_t>, Al<char8_t>>&&>(s);
	}

	template <template <class> class Tr, template <class> class Al>
	decltype(auto) AsString8(const std::basic_string<char, Tr<char>, Al<char>>& s) noexcept
	{
		return reinterpret_cast<const std::basic_string<char8_t, Tr<char8_t>, Al<char8_t>>&>(s);
	}

	inline auto AsString(const std::basic_string_view<char8_t>& s) noexcept
	{
		return reinterpret_cast<const std::basic_string_view<char>&>(s);
	}

	inline auto AsString8(const std::basic_string_view<char>& s) noexcept
	{
		return reinterpret_cast<const std::basic_string_view<char8_t>&>(s);
	}

	inline auto AsString(const char8_t* s) noexcept
	{
		return reinterpret_cast<const char*>(s);
	}

	inline auto AsString(char8_t* s) noexcept
	{
		return reinterpret_cast<char*>(s);
	}

	inline auto AsString8(const char* s) noexcept
	{
		return reinterpret_cast<const char8_t*>(s);
	}

	inline auto AsString8(char* s) noexcept
	{
		return reinterpret_cast<char8_t*>(s);
	}

	template <size_t N>
	auto& AsString(char8_t(&s)[N]) noexcept
	{
		return reinterpret_cast<char(&)[N]>(s);
	}

	template <size_t N>
	auto& AsString(const char8_t(&s)[N]) noexcept
	{
		return reinterpret_cast<const char(&)[N]>(s);
	}

	template <size_t N>
	auto& AsString8(char(&s)[N]) noexcept
	{
		return reinterpret_cast<char8_t(&)[N]>(s);
	}

	template <size_t N>
	auto& AsString8(const char(&s)[N]) noexcept
	{
		return reinterpret_cast<const char8_t(&)[N]>(s);
	}

	CORE_API std::u8string ToUtf8(std::u16string_view utf16);
	CORE_API std::u16string ToUtf16(std::u8string_view utf8);
}
