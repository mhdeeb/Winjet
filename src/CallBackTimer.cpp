#include "CallBackTimer.h"

CallBackTimer::~CallBackTimer() {
	if (_execute.load(std::memory_order_acquire)) {
		stop();
	}
}

void CallBackTimer::stop() {
	_execute.store(false, std::memory_order_release);
	if (_thd.joinable())
		_thd.join();
}

bool CallBackTimer::isRunning() const {
	return _execute.load(std::memory_order_acquire);
}

TimerQueue::~TimerQueue() {
	stopAll();
}

bool TimerQueue::isRunning(unsigned int id) const {
	return _timers.at(id).isRunning();
}

void TimerQueue::stop(unsigned int id) {
	_timers[id].stop();
	_timers.erase(id);
}

void TimerQueue::stopAll() {
	for (auto& [id, timer] : _timers)
		timer.stop();
	_timers.clear();
}