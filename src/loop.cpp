//
// Created by Emmet on 2/27/2023.
//

module;

#include <chrono>

export module Loop;

import Types;
import Util;

export class Fps {
private:
	i32 value;

public:
	explicit Fps(i32 value) : value(value) {}

	static auto unlimited() -> Fps {
		return Fps(-1);
	}

	[[nodiscard]] auto isUnlimited() const -> bool {
		return value == -1;
	}

	friend class Loop;
};

export class Loop {
public:
	using clock = std::chrono::steady_clock;
	using resolution = clock::duration;
	using seconds = std::chrono::seconds;
	using timePoint = std::chrono::time_point<clock>;

	/**
	 * first construct the loop object
	 */
	explicit Loop() = default;

	/**
	 * then call this to enter the loop
	 * lambda returns the current fps the loop should run at
	 */
	template<Util::Function<Fps, timePoint> StepFunction>
	[[noreturn]] auto enter(StepFunction stepFunction) -> void {
		auto lastTime = clock::now();

		i32 fps;
		bool unlimited;

		do {
			auto now = clock::now();

			Fps fpsReturn = stepFunction(now);

			if (fpsReturn.isUnlimited()) {
				unlimited = true;
			} else {
				fps = fpsReturn.value;
				unlimited = false;
			}

			if (unlimited) continue;

			auto frameTime = (u64)((1.0_f64 / (f64)fps) * resolution(seconds(1)).count());

			auto nextTime = (lastTime += resolution(frameTime));
			now = clock::now();
			auto sleepDuration = nextTime - now;

			if (sleepDuration.count() < 0) {
				lastTime = now;
			} else {
				std::this_thread::sleep_for(sleepDuration);
			}
		} while (true);
	}
};
