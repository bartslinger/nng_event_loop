#ifndef TIMER_HPP
#define TIMER_HPP

#include <functional>

#include "event_loop.hpp"
#include "event_source.hpp"

class Timer : public EventSource {
public:
	Timer(EventLoop* loop);
	~Timer();

	void start_periodic(int interval_ms);
	void set_timeout_callback(std::function<void(void)> callback);

protected:
	void pollin_event();

private:
	std::function<void(void)> _callback = nullptr;
};

#endif // TIMER_HPP
