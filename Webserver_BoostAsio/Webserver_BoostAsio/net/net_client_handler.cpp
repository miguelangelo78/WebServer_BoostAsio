#include "stdafx.h" // May need to delete this if not using precompiled headers
#include "net_handler.h"

// Client events:

// Client Connection:
void ClientConnection::OnAccept(const std::string & host, uint16_t port) {
	TLOCK;
	std::cout << "[" << __FUNCTION__ << "] " << host << ":" << port << std::endl;
	TULOCK;

	// Start the next receive
	Recv();
}

void ClientConnection::OnConnect(const std::string & host, uint16_t port) {
	TLOCK;
	std::cout << "[" << __FUNCTION__ << "] " << host << ":" << port << std::endl;
	TULOCK;

	// Start the next receive
	Recv();

	std::string str = "GET / HTTP/1.0\r\n\r\n";

	std::vector< uint8_t > request;
	std::copy(str.begin(), str.end(), std::back_inserter(request));
	Send(request);
}

void ClientConnection::OnDisconnect() {

}

void ClientConnection::OnSend(const std::vector< uint8_t > & buffer) {
	TLOCK;
	std::cout << "[" << __FUNCTION__ << "] " << buffer.size() << " bytes" << std::endl;
	for (size_t x = 0; x < buffer.size(); ++x)
	{
		std::cout << buffer[x];
	}
	std::cout << std::endl;
	TULOCK;
}

void ClientConnection::OnRecv(std::vector< uint8_t > & buffer) {
	TLOCK;
	std::cout << "[" << __FUNCTION__ << "] " << buffer.size() << " bytes" << std::endl;
	for (size_t x = 0; x < buffer.size(); ++x)
	{
		std::cout << buffer[x];
	}
	std::cout << std::endl;
	TULOCK;

	// Start the next receive
	Sleep(1000);
	Recv();
}

void ClientConnection::OnTimer(const boost::posix_time::time_duration & delta) {
	TLOCK;

	TULOCK;
}

void ClientConnection::OnError(const boost::system::error_code & error) {
	TLOCK;
	std::cout << "[" << __FUNCTION__ << "] " << error << std::endl;
	TULOCK;
}

// Client Handler:
void NetClient::client_work() {
	while (1) {
		updateHive();

		if (_kbhit()) break;
	}
}
