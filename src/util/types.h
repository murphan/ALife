//
// Created by Emmet on 11/9/2022.
//

#ifndef ALIFE_TYPES_H
#define ALIFE_TYPES_H

#include <cstdint>
#include <string>

using u8 = std::uint8_t;
using u16 = std::uint16_t;
using u32 = std::uint32_t;
using u64 = std::uint64_t;
using usize = std::size_t;

using i8 = std::int8_t;
using i16 = std::int16_t;
using i32 = std::int32_t;
using i64 = std::int64_t;

using f32 = float;
using f64 = double;

constexpr auto operator "" _u8(unsigned long long int i) -> u8 { return static_cast<u8>(i); }
constexpr auto operator "" _u16(unsigned long long int i) -> u16 { return static_cast<u16>(i); }
constexpr auto operator "" _u32(unsigned long long int i) -> u32 { return static_cast<u32>(i); }
constexpr auto operator "" _u64(unsigned long long int i) -> u64 { return static_cast<u64>(i); }
constexpr auto operator "" _usize(unsigned long long int i) -> usize { return static_cast<usize>(i); }

constexpr auto operator "" _i8(unsigned long long int i) -> i8 { return static_cast<i8>(i); }
constexpr auto operator "" _i16(unsigned long long int i) -> i16 { return static_cast<i16>(i); }
constexpr auto operator "" _i32(unsigned long long int i) -> i32 { return static_cast<i32>(i); }
constexpr auto operator "" _i64(unsigned long long int i) -> i64 { return static_cast<i64>(i); }

constexpr auto operator "" _f32(long double i) -> f32 { return static_cast<f32>(i); }
constexpr auto operator "" _f64(long double i) -> f64 { return static_cast<f64>(i); }

#endif //ALIFE_TYPES_H
