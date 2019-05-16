#include <iostream>

#include "nng_event_loop/event_loop.hpp"
#include "nng_event_loop/timer.hpp"

class TestNode : public EventLoop {
public:
	TestNode() :
		timer(this)
	{

	}

	~TestNode()
	{

	}

	void on_timeout() {
		std::cout << "timeout callback" << std::endl;
	}

	int init()
	{
		std::cout << "Node init" << std::endl;

		// bind timer to callback
		timer.set_timeout_callback(std::bind(&TestNode::on_timeout, this));
		timer.start_periodic(1000);

		return 0;
	}

private:
	Timer timer;
};

int main(int argc, char* argv[])
{
	std::cout << "Start test program" << std::endl;

	TestNode node;

	node.init();
	node.run();


	return 0;
}
