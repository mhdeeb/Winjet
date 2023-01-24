#pragma once

#include <thread>
#include <functional>
#include <map>

class CallBackTimer {
private:
	std::atomic<bool> _execute = false;
	std::jthread _thd;
public:
	~CallBackTimer();

	void stop();

	template<typename _FUNC, typename ... _ARGS>
	void start(int interval, _FUNC func, _ARGS...args) {
		if (_execute.load(std::memory_order_acquire))
			stop();
		_execute.store(true, std::memory_order_release);
		_thd = std::jthread([this, interval, func, args...]() {
			while (_execute.load(std::memory_order_acquire)) {
				func(args...);
				std::this_thread::sleep_for(std::chrono::milliseconds(interval));
			}
			});
	}

	bool isRunning() const;
};

class TimerQueue {
private:
	std::map<unsigned int, CallBackTimer> _timers;
public:
	~TimerQueue();

	template<typename _FUNC, typename ... _ARGS>
	void add(unsigned int id, int interval, _FUNC func, _ARGS...args) {
		_timers[id].start(interval, func, args...);
	}

	bool isRunning(unsigned int id) const;

	void stop(unsigned int id);

	void stopAll();
};