//
// Created by Emmet on 11/9/2022.
//

module;

#include <cstdint>
#include <cstddef>

export module Types;

export using u8 = std::uint8_t;
export using u16 = std::uint16_t;
export using u32 = std::uint32_t;
export using u64 = std::uint64_t;
export using usize = std::size_t;

export using i8 = std::int8_t;
export using i16 = std::int16_t;
export using i32 = std::int32_t;
export using i64 = std::int64_t;

export using f32 = float;
export using f64 = double;

export constexpr auto operator "" _u8(unsigned long long int i) noexcept -> u8 { return static_cast<u8>(i); }
export constexpr auto operator "" _u16(unsigned long long int i) noexcept -> u16 { return static_cast<u16>(i); }
export constexpr auto operator "" _u32(unsigned long long int i) noexcept -> u32 { return static_cast<u32>(i); }
export constexpr auto operator "" _u64(unsigned long long int i) noexcept -> u64 { return static_cast<u64>(i); }
export constexpr auto operator "" _usize(unsigned long long int i) noexcept -> usize { return static_cast<usize>(i); }

export constexpr auto operator "" _i8(unsigned long long int i) noexcept -> i8 { return static_cast<i8>(i); }
export constexpr auto operator "" _i16(unsigned long long int i) noexcept -> i16 { return static_cast<i16>(i); }
export constexpr auto operator "" _i32(unsigned long long int i) noexcept -> i32 { return static_cast<i32>(i); }
export constexpr auto operator "" _i64(unsigned long long int i) noexcept -> i64 { return static_cast<i64>(i); }

export constexpr auto operator "" _f32(long double i) noexcept -> f32 { return static_cast<f32>(i); }
export constexpr auto operator "" _f64(long double i) noexcept -> f64 { return static_cast<f64>(i); }
