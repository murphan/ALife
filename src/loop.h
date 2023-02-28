//
// Created by Emmet on 2/27/2023.
//

#ifndef ALIFE_LOOP_H
#define ALIFE_LOOP_H

#include <chrono>

#include "util.h"

class Loop {
private:
	using clock = std::chrono::steady_clock;
	using resolution = clock::duration;
	using seconds = std::chrono::seconds;

	i32 fps;

public:
	/**
	 * first construct the loop object
	 */
	explicit Loop(i32 fps);

	auto setFps(i32 fps) -> void;

	/**
	 * then call this to enter the loop
	 */
	template<Util::Function<void, bool> StepFunction>
	auto enter(StepFunction step) -> void {
		auto lastTime = clock::now();

		do {
			auto shouldClose = step();
			if (shouldClose) break;

			auto frameTime = (u64)(((f64)1 / (f64)fps) * resolution(seconds(1)).count());

			lastTime += resolution(frameTime);
			auto nextTime = lastTime;

			auto now = clock::now();
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
