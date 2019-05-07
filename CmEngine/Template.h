#pragma once
#include "Template.inl"

namespace Tpl
{
	template<typename _type>
	constexpr auto TypeName = TemplateImpl::GetTypeName<_type>();

	template<typename _enumType, _enumType _enumVal>
	constexpr auto EnumName = TemplateImpl::GetEnumName<_enumType, _enumVal>();

	template<typename _type>
	constexpr auto IdentifyName = TemplateImpl::GetIdentify<_type>();

	template<typename... _argts>
	constexpr auto ArgumentsName = TemplateImpl::GetTemplateArguments<_argts...>();

	template<typename... _types>
	struct TypeSequence {};

	template<typename _type>
	struct TAsSingle
	{
	public:
		using Type = _type;

		TAsSingle() = default;
		TAsSingle(const TAsSingle&) = delete;

		static Type & Get()
		{
			static Type single;
			return single;
		}
	};
}
