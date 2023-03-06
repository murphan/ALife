//
// Created by Emmet on 2/27/2023.
//

#ifndef ALIFE_LOOP_H
#define ALIFE_LOOP_H

#include <chrono>

#include "util.h"

class Fps {
private:
	i32 value;

public:
	explicit Fps(i32 value);

	static auto unlimited() -> Fps;
	auto isUnlimited() const -> bool;

	friend class Loop;
};

class Loop {
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

#endif //ALIFE_LOOP_H
