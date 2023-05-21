//
// Created by Emmet on 5/7/2023.
//

module;

#include <mutex>

export module PriorityMutex;

import Util;

export class PriorityMutex {
private:
	std::mutex mutex;
	std::mutex lowPriorityMutex;
	std::mutex nextAccessMutex;

public:
	explicit PriorityMutex() : mutex(), lowPriorityMutex(), nextAccessMutex() {};

	template<Util::Function<void> Block>
	auto lowPriorityLock(Block block) -> void {
		lowPriorityMutex.lock();
		nextAccessMutex.lock();
		mutex.lock();
		nextAccessMutex.unlock();

		block();

		mutex.unlock();
		lowPriorityMutex.unlock();
	}

	template<Util::Function<void> Block>
	auto highPriorityLock(Block block) -> void {
		nextAccessMutex.lock();
		mutex.lock();
		nextAccessMutex.unlock();

		block();

		mutex.unlock();
	}
};
