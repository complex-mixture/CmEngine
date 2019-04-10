#pragma once
#include <stdint.h>

namespace TemplateImpl
{
	template<uint64_t _sizeofInt>
	struct int_type {};

	template<uint64_t _sizeofInt>
	struct uint_type {};

	template<> struct int_type<1> { using type = int8_t; };
	template<> struct int_type<2> { using type = int16_t; };
	template<> struct int_type<4> { using type = int32_t; };
	template<> struct int_type<8> { using type = int64_t; };
	
	template<> struct uint_type<1> { using type = uint8_t; };
	template<> struct uint_type<2> { using type = uint16_t; };
	template<> struct uint_type<4> { using type = uint32_t; };
	template<> struct uint_type<8> { using type = uint64_t; };
}

template<uint64_t _sizeofInt>
using int_type = typename TemplateImpl::int_type<_sizeofInt>::type;

template<uint64_t _sizeofInt>
using uint_type = typename TemplateImpl::uint_type<_sizeofInt>::type;
