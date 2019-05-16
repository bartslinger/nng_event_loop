#ifndef LISTENER_HPP
#define LISTENER_HPP

#include <iostream>
#include <functional>

#include <nng/nng.h>
#include <nng/protocol/reqrep0/rep.h>

#include "event_loop.hpp"
#include "event_source.hpp"

class Listener : public EventSource {
public:
	Listener(EventLoop* loop);
	~Listener();

	int listen(std::string url);
	void set_receive_callback(std::function<void(std::string, std::string&)> callback);

protected:

	void pollin_event();

private:
	nng_socket _socket;
	std::string _url;

	std::function<void(std::string, std::string&)> _callback;
};

#endif // LISTENER_HPP
