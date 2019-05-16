#include <unistd.h>
#include <string.h>
#include <iostream>
#include "udp_source.hpp"

UdpSource::UdpSource(EventLoop *loop) :
	_callback(nullptr)
{
	loop->add_event_source(this);
	_fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
}

UdpSource::~UdpSource()
{
	close(_fd);
}

int UdpSource::connect(std::string remote_ip, unsigned int remote_port, unsigned int local_port)
{
	// Listen to UAV
	memset(&_loc_addr, 0, sizeof(_loc_addr));
	_loc_addr.sin_family = AF_INET;
	_loc_addr.sin_addr.s_addr = INADDR_ANY;
	_loc_addr.sin_port = htons(local_port);

	if (bind(_fd, (struct sockaddr *)&_loc_addr, sizeof(struct sockaddr)) == -1)
	{
		std::cerr << "UDP bind failed" << std::endl;
		close(_fd);
		exit(EXIT_FAILURE);
	}

	// Configure socket for sending to UAV
	memset(&_uav_addr, 0, sizeof(_uav_addr));
	_uav_addr.sin_family = AF_INET;
	_uav_addr.sin_addr.s_addr = inet_addr(remote_ip.c_str());
	_uav_addr.sin_port = htons(remote_port);
}

void UdpSource::set_receive_callback(std::function<void (std::string)> callback)
{
	_callback = callback;
}

ssize_t UdpSource::send_packet(std::string packet)
{
	ssize_t bytes_sent = sendto(_fd, packet.c_str(), packet.length(), 0, (struct sockaddr*)&_uav_addr, sizeof(struct sockaddr_in));
	return bytes_sent;
}

void UdpSource::pollin_event()
{
	ssize_t recv_size = recv(_fd, (void *)buf, BUFFER_LENGTH, 0);
	if (recv_size > 0) {
		// std::cout << "received " << std::to_string(recv_size) << " bytes" << std::endl;
		std::string packet((char*)buf, recv_size);

		if (_callback == nullptr) {
			std::cout << "Received bytes but no callback defined" << std::endl;
		} else {
			_callback(packet);
		}

	}
}
