#ifndef REQUESTER_HPP
#define REQUESTER_HPP

#include <iostream>
#include <functional>

#include <nng/nng.h>
#include <nng/protocol/reqrep0/req.h>

#include "nng_event_loop/event_loop.hpp"
#include "nng_event_loop/event_source.hpp"

class Requester : public EventSource {
public:
	Requester(EventLoop* loop, std::string url);
	~Requester();

	void request(std::string &request);
	void set_receive_callback(std::function<void(std::string)> callback);

protected:
	void pollin_event();

private:
	nng_dialer _dialer;
	nng_socket _socket;
	std::string _url;

	std::function<void(std::string)> _callback;

	bool _request_busy;

};

#endif // REQUESTER_HPP
