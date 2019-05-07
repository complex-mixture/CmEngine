#pragma once
#include <stdint.h>
#include <string>

namespace TemplateImpl
{
	constexpr inline bool IsIdentifierChar(char _char, bool _isFront) noexcept
	{
		return
			(_isFront || _char >= '0' && _char <= '9')
			|| _char >= 'a' && _char <= 'z'
			|| _char >= 'A' && _char <= 'Z'
			|| _char == '_';
	}

	constexpr inline std::string_view RemoveSuffixOfFunsig(std::string_view _funsig)
	{
		for (int i = 0; i != _funsig.size(); ++i)
		{
			if (_funsig[_funsig.size() - i - 1] == '>')
			{
				_funsig.remove_suffix(i + 1);
				break;
			}
		}
		return _funsig;
	}

	constexpr inline std::string_view ParsingFunsigForLastName(std::string_view _funsig)
	{
		_funsig = RemoveSuffixOfFunsig(_funsig);

		for (size_t i = _funsig.size() - 1; i >= 0; --i)
		{
			if (!IsIdentifierChar(_funsig[i], false))
			{
				_funsig.remove_prefix(i);
				break;
			}
		}

		if (_funsig.size() != 0 && IsIdentifierChar(_funsig.front(), true))
			return _funsig;
		else return {};
	}

	constexpr inline std::string_view ParsingFunsigForLastIdentify(std::string_view _funsig)
	{
		_funsig = RemoveSuffixOfFunsig(_funsig);

		for (size_t i = _funsig.size() - 1; i >= 0; --i)
		{
			if (_funsig[i] == '<' || _funsig[i] == ',')
			{
				_funsig.remove_prefix(i);
				break;
			}
		}

		if (_funsig.size() != 0)
			return _funsig;
		else return {};
	}

	constexpr inline std::string_view ParsingFunsigForTemplateArguments(std::string_view _funsig)
	{
		_funsig = RemoveSuffixOfFunsig(_funsig);

		for (size_t i = _funsig.size() - 1; i >= 0; --i)
		{
			if (_funsig[i] == '<')
			{
				_funsig.remove_prefix(i);
				break;
			}
		}

		if (_funsig.size() != 0)
			return _funsig;
		else return {};
	}

	template<typename _type>
	constexpr inline std::string_view GetTypeName()
	{
		return ParsingFunsigForLastName(__FUNCSIG__);
	}

	template<typename _enumType, _enumType _value>
	constexpr inline std::string_view GetEnumName()
	{
		return ParsingFunsigForLastName(__FUNCSIG__);
	}

	template<typename _type>
	constexpr inline std::string_view GetIdentify()
	{
		return ParsingFunsigForLastIdentify(__FUNCSIG__);
	}

	template<typename... _argts>
	constexpr inline std::string_view GetTemplateArguments()
	{
		return ParsingFunsigForTemplateArguments(__FUNCSIG__);
	}

	template<typename _enumType>
	constexpr inline auto EnumToInt(_enumType _enumValue)->std::underlying_type_t<_enumType>
	{
		static_assert(std::is_enum_v<_enumType>, "_enumType is not a enum type");
		using intType = std::underlying_type_t<_enumType>;
		return static_cast<intType>(_enumValue);
	}

}
