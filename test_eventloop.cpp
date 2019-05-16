#include <iostream>

#include "nng_event_loop/event_loop.hpp"
#include "nng_event_loop/timer.hpp"
#include "nng_event_loop/subscriber.hpp"
#include "nng_event_loop/publisher.hpp"
#include "nng_event_loop/replier.hpp"
#include "nng_event_loop/requester.hpp"

using namespace std::placeholders;

class TestNode : public EventLoop {
public:
	TestNode() :
		timer(this),
		sub(this),
		pub("ipc:///tmp/testsocket"),
		replier(this),
		requester(this, "ipc:///tmp/repliersocket")
	{

	}

	~TestNode()
	{

	}

	void on_timeout() {
		std::cout << "timeout callback" << std::endl;
		std::string message = "Publishing on timer";
		pub.publish(message);

		std::string request = "Requestion on timer";
		requester.request(request);
	}

	void sub_callback(std::string message) {
		std::cout << "Subscriber received: " << message << std::endl;
	}

	void replier_callback(std::string message, std::string &reply) {
		std::cout << "Replier received: " << message << std::endl;
		reply = "request was received!";
	}

	void requester_callback(std::string reply) {
		std::cout << "Requester received: " << reply << std::endl;
	}

	int init()
	{
		std::cout << "Node init" << std::endl;

		// bind timer to callback
		timer.set_timeout_callback(std::bind(&TestNode::on_timeout, this));
		timer.start_periodic(1500);

		sub.set_receive_callback(std::bind(&TestNode::sub_callback, this, _1));
		sub.subscribe("ipc:///tmp/testsocket");

		replier.set_receive_callback(std::bind(&TestNode::replier_callback, this, _1, _2));
		replier.listen("ipc:///tmp/repliersocket");

		requester.set_receive_callback(std::bind(&TestNode::requester_callback, this, _1));

		return 0;
	}

private:
	Timer timer;
	Subscriber sub;
	Publisher pub;
	Replier replier;
	Requester requester;
};

int main(int argc, char* argv[])
{
	std::cout << "Start test program" << std::endl;

	TestNode node;

	node.init();
	node.run();


	return 0;
}
