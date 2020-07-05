#include "Name.hpp"
#include <unordered_set>
#include <otm/Hash.hpp>
#include "Json.hpp"
#include "Core.hpp"
#include "Templates/Sync.hpp"

namespace oeng
{
	struct NameHasher
	{
		[[nodiscard]] constexpr size_t operator()(std::string_view s) const noexcept
		{
			return HashRange(s.begin(), s.end(), tolower);
		}
	};

	struct NameEqual
	{
		[[nodiscard]] constexpr bool operator()(std::string_view s1, std::string_view s2) const noexcept
		{
			if (s1.size() != s2.size()) return false;
			for (size_t i = 0; i < s1.size(); ++i)
				if (tolower(s1[i]) != tolower(s2[i]))
					return false;
			return true;
		}
	};

	static auto GetSet()
	{
		constexpr bool thread_safe = OE_NAME_THREADSAFE;
		assert(thread_safe || IsGameThread());
		
		using NameSet = std::unordered_set<std::string, NameHasher, NameEqual>;
		static CondMonitor<NameSet, thread_safe> set{std::string{}};
		return set.Lock();
	}
	
	Name::Name() noexcept
	{
		static const auto def = &*GetSet()->find({});
		sp = def;
	}

	Name::Name(std::string s)
		:sp{&*GetSet()->insert(std::move(s)).first}
	{
	}

	void to_json(Json& json, const Name& name)
	{
		json = *name;
	}

	void from_json(const Json& json, Name& name)
	{
		name = Name{json.get<std::string>()};
	}
}
