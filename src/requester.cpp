#include "requester.hpp"

Requester::Requester(EventLoop *loop, std::string url) :
	_callback(nullptr),
	_request_busy(false)
{
	loop->add_event_source(this);

	_url = url;

	int rv;
	// start the Requester
	if ((rv = nng_req0_open(&_socket)) != 0) {
		std::cerr << "[requester] nng_req0_open: " << nng_strerror(rv) << std::endl;
	}

	if ((rv = nng_setopt_bool(_socket, NNG_OPT_TCP_KEEPALIVE, true)) != 0) {
		std::cerr << "[requester] nng_setopt keepalive: " << nng_strerror(rv) << std::endl;
	}

	if ((rv = nng_dialer_create(&_dialer, _socket, url.c_str())) != 0) {
		std::cerr << "[requester] dialer: " << nng_strerror(rv) << std::endl;
	}

	// Set re-connect time to 500ms, helps when starting subscriber first
	if ((rv = nng_dialer_setopt_ms(_dialer, NNG_OPT_RECONNMINT, 500)) != 0) {
		std::cerr << "[requester] nng_dialer_setopt_ms: " << nng_strerror(rv) << std::endl;
	}

	if ((rv = nng_dialer_setopt_ms(_dialer, NNG_OPT_RECONNMAXT, 0)) != 0) {
		std::cerr << "[requester] nng_dialer_setopt_ms: " << nng_strerror(rv) << std::endl;
	}

	if ((rv = nng_dialer_start(_dialer, NNG_FLAG_NONBLOCK)) != 0) {
		std::cerr << "[requester] nng_dialer_start: " << nng_strerror(rv) << std::endl;
	}

	//if ((rv = nng_dial(_socket, url.c_str(), NULL, NNG_FLAG_NONBLOCK)) != 0) {
	//	std::cerr << "[requester] nng_dial: " << nng_strerror(rv) << std::endl;
	//}

	if ((rv = nng_getopt_int(_socket, NNG_OPT_RECVFD, &_fd))) {
		std::cerr << "[requester] nng_getopt: " << nng_strerror(rv) << std::endl;
	}
}

Requester::~Requester()
{

}

void Requester::request(std::string &request)
{
	if (_request_busy) {
		std::cerr << "Another request still in progress" << std::endl;
		return;
	}
	nng_msg *msg;
	nng_msg_alloc(&msg, 0);
	nng_msg_append(msg, request.c_str(), request.length());
	nng_sendmsg(_socket, msg, NNG_FLAG_NONBLOCK);

	_request_busy = true;

}

void Requester::set_receive_callback(std::function<void (std::string)> callback)
{
	_callback = callback;
}

void Requester::pollin_event()
{
	// handle the reply
	nng_msg *msg;
	nng_recvmsg(_socket, &msg, 0);
	std::string message((char*)nng_msg_body(msg), (char*)nng_msg_body(msg) + nng_msg_len(msg));

	// free the message
	nng_msg_free(msg);

	if (_callback == nullptr) {
		std::cerr << "No reply handler defined by requester" << std::endl;
	} else {
		_callback(message);
	}
	_request_busy = false;
}
