#pragma once
#include <thread>
#include <functional>

class CallBackTimer {
private:
	std::atomic<bool> _execute = false;
	std::jthread _thd;
public:
	~CallBackTimer() {
		if (_execute.load(std::memory_order_acquire))
			stop();
	}

	void stop() {
		_execute.store(false, std::memory_order_release);
		if (_thd.joinable())
			_thd.join();
	}

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
};