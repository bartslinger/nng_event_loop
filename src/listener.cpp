#include <string.h>
#include "listener.hpp"

Listener::Listener(EventLoop* loop) :
	_callback(nullptr)
{
	loop->add_event_source(this);
}

Listener::~Listener()
{

}

int Listener::listen(std::string url)
{
	_url = url;
	int rv;
	// Configure reply topic for utmsp request
	if ((rv = nng_rep0_open(&_socket)) != 0) {
		std::cerr << "[listener] nng_rep0_open: " << nng_strerror(rv) << std::endl;
		return 1;
	}

	if ((rv = nng_listen(_socket, _url.c_str(), NULL, NNG_FLAG_NONBLOCK))) {
		std::cerr << "[listener] nng_listen: " << nng_strerror(rv) << std::endl;
		return 1;
	}

	if ((rv = nng_getopt_int(_socket, NNG_OPT_RECVFD, &_fd))) {
		std::cerr << "[listener] nng_getopt: " << nng_strerror(rv) << std::endl;
		return 1;
	}

	return 0;
}

void Listener::set_receive_callback(std::function<void (std::string, std::string &)> callback)
{
	_callback = callback;
}

void Listener::pollin_event()
{
	nng_msg *msg;
	nng_recvmsg(_socket, &msg, 0);
	std::string message((char*)nng_msg_body(msg), (char*)nng_msg_body(msg) + nng_msg_len(msg));

	// free the message
	nng_msg_free(msg);

	std::string reply = "NOTOK";

	if (_callback == nullptr) {
		std::cerr << "No callback defined!" << std::endl;
	} else {
		_callback(message, reply);
	}

	// send response
	nng_msg_alloc(&msg, reply.length());
	memcpy(nng_msg_body(msg), reply.c_str(), reply.length());
	nng_sendmsg(_socket, msg, 0);
}
