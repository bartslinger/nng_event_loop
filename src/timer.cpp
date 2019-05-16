#include <unistd.h>
#include <sys/timerfd.h>
#include <iostream>
#include "timer.hpp"

Timer::Timer(EventLoop *loop)
{
	loop->add_event_source(this);

	_fd = timerfd_create(CLOCK_MONOTONIC, 0);

}

Timer::~Timer()
{
	close(_fd);
}

void Timer::set_timeout_callback(std::function<void ()> callback)
{
	_callback = callback;
}

void Timer::start_periodic(int interval_ms)
{
	struct timespec ts;
	ts.tv_sec = interval_ms / 1000;
	ts.tv_nsec = (interval_ms % 1000) * 1e6;
	struct itimerspec its;
	its.it_interval = ts;
	its.it_value = ts;

	if (timerfd_settime(_fd, 0, &its, NULL) == -1) {
		std::cerr << "ERROR setting periodic timer" << std::endl;
	}
}

void Timer::pollin_event()
{
	uint64_t num_timer_events;
	ssize_t recv_size = read(_fd, &num_timer_events, 8);
	(void) recv_size;
	if (_callback == nullptr) {
		std::cout << "Timer event, but no callback defined" << std::endl;
	} else {
		_callback();
	}
}
