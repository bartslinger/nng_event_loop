#include <iostream>
#include "event_source.hpp"

EventSource::EventSource()
{

}

void EventSource::pollin_event()
{
	std::cerr << "POLLIN event, no handler defined" << std::endl;
}

int EventSource::fd()
{
	return _fd;
}

EventSource::~EventSource()
{

}
