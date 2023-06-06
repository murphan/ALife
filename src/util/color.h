//
// Created by Emmet on 5/9/2023.
//

#ifndef ALIFE_COLOR_H
#define ALIFE_COLOR_H

#include <cmath>
#include "types.h"

namespace Color {
	struct HSV {
		f32 h, s, v;
	};

	constexpr auto fractionalPart(f32 f) {
		return f < 0.0_f32 ? f + (f32)(1_i64 - (i64)f) : f - (f32)(i64)f;
	}

	constexpr auto clamp(f32 value) -> f32 {
		if (value < 0.0_f32) return 0.0_f32;
		else if (value > 1.0_f32) return 1.0_f32;
		return value;
	}

	constexpr auto channelFloats2Int(f32 r, f32 g, f32 b) -> u32 {
		return ((u32)(clamp(r) * 255.0_f32) << 16) | ((u32)(clamp(g) * 255.0_f32) << 8) | (u32)(clamp(b) * 255.0_f32);
	}

	constexpr auto hsv2rgb(HSV hsv) -> u32 {
		auto [h, s, v] = hsv;

		if(s <= 0.0_f32)
			return channelFloats2Int(v, v, v);

		auto hh = fractionalPart(h) * 6.0_f32;
		auto i = (i32)hh;
		//auto ff = hh - (f32)i;
		auto ff = fractionalPart(hh);
		auto p = v * (1.0_f32 - s);
		auto q = v * (1.0_f32 - (s * ff));
		auto t = v * (1.0_f32 - (s * (1.0_f32 - ff)));

		switch(i) {
			case 0:
				return channelFloats2Int(v, t, p);
			case 1:
				return channelFloats2Int(q, v, p);
			case 2:
				return channelFloats2Int(p, v, t);
			case 3:
				return channelFloats2Int(p, q, v);
			case 4:
				return channelFloats2Int(t, p, v);
			case 5:
			default:
				return channelFloats2Int(v, p, q);
		}
	}

	constexpr auto rgb2hsv(u32 rgb) -> HSV {
		auto r = (i32)((rgb >> 16) & 0xff);
		auto g = (i32)((rgb >> 8) & 0xff);
		auto b = (i32)(rgb & 0xff);

		auto min = r < g ? r : g;
		min = min < b ? min : b;

		auto max = r > g ? r : g;
		max = max > b ? max : b;

		HSV out = {
			.h=0.0_f32,
			.s=0.0_f32,
			.v=(f32)max / 255.0_f32,
		};

		auto delta = max - min;
		if (delta == 0) {
			out.h = 0.0_f32;
			out.s = 0.0_f32;
			return out;
		}

		if (max > 0) {
			out.s = (f32)delta / (f32)max;
		} else {
			out.h = 0.0_f32;
			out.s = 0.0_f32;
			return out;
		}

		if (r == max) {
			out.h = (f32)(g - b) / (f32)delta;
		} else if (g == max) {
			out.h = 2.0_f32 + (f32)(b - r) / (f32)delta;
		} else { /* if (b == max) */
			out.h = 4.0_f32 + (f32)(r - g) / (f32)delta;
		}

		out.h = fractionalPart(out.h / 6.0_f32);

		return out;
	}
}

#endif //ALIFE_COLOR_H
