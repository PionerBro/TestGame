#pragma once
#include <functional>

class Timer {
public:
	Timer::Timer();

	void Timer::update(const double delta);
	void Timer::start(const double duration);
	void Timer::stop();
	void Timer::set_callback(std::function<void()> callback);
private:
	std::function<void()> m_callback;
	double m_timeLeft;
	bool m_isRunning;
};
