#include <iostream>

#include "nng_event_loop/event_loop.hpp"
#include "nng_event_loop/timer.hpp"
#include "nng_event_loop/subscriber.hpp"
#include "nng_event_loop/publisher.hpp"
#include "nng_event_loop/listener.hpp"

using namespace std::placeholders;

class TestNode : public EventLoop {
public:
	TestNode() :
		timer(this),
		sub(this),
		pub("ipc:///tmp/testsocket"),
		listener(this)
	{

	}

	~TestNode()
	{

	}

	void on_timeout() {
		std::cout << "timeout callback" << std::endl;
		std::string message = "Publishing on timer";
		pub.publish(message);
	}

	void sub_callback(std::string message) {
		std::cout << "received: " << message << std::endl;
	}

	void listener_callback(std::string message, std::string &reply) {
		std::cout << "Listener received: " << message << std::endl;
		reply = "request was received!";
	}

	int init()
	{
		std::cout << "Node init" << std::endl;

		// bind timer to callback
		timer.set_timeout_callback(std::bind(&TestNode::on_timeout, this));
		timer.start_periodic(1000);

		sub.set_receive_callback(std::bind(&TestNode::sub_callback, this, _1));
		sub.subscribe("ipc:///tmp/testsocket");

		listener.set_receive_callback(std::bind(&TestNode::listener_callback, this, _1, _2));
		listener.listen("ipc:///tmp/listenersocket");

		return 0;
	}

private:
	Timer timer;
	Subscriber sub;
	Publisher pub;
	Listener listener;
};

int main(int argc, char* argv[])
{
	std::cout << "Start test program" << std::endl;

	TestNode node;

	node.init();
	node.run();


	return 0;
}
