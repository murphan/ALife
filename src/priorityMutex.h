//
// Created by Emmet on 5/7/2023.
//

#ifndef ALIFE_PRIORITYMUTEX_H
#define ALIFE_PRIORITYMUTEX_H

#include <mutex>
#include "util.h"

class PriorityMutex {
private:
	std::mutex mutex;
	std::mutex lowPriorityMutex;
	std::mutex nextAccessMutex;

public:
	explicit PriorityMutex();

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

#endif //ALIFE_PRIORITYMUTEX_H
