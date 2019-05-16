#ifndef PUBLISHER_HPP
#define PUBLISHER_HPP

#include <iostream>

#include <nng/nng.h>
#include <nng/protocol/pubsub0/pub.h>

class Publisher {
public:
	Publisher(std::string url);
	~Publisher();

	void publish(std::string &message);

private:
	nng_socket _socket;
	std::string _url;
};

#endif // PUBLISHER_HPP
