#include "stdafx.h" // May need to delete this if not using precompiled headers
#include "net_handler.h"

boost::mutex global_lock;

std::string buffer_to_str(std::vector<uint8_t> buff) {
	std::string str(buff.begin(), buff.end());
	return str;
}

std::vector<uint8_t> str_to_buffer(std::string str) {
	std::vector<uint8_t> buffer;
	std::copy(str.begin(), str.end(), std::back_inserter(buffer));
	return buffer;
}

// Server events:

// Server Connection:
void ServerConnection::OnAccept(const std::string & host, uint16_t port) {
	TLOCK;
	std::cout << "[" << __FUNCTION__ << "] " << host << ":" << port << std::endl;
	TULOCK;

	// Start the next receive
	Recv();
}

void ServerConnection::OnConnect(const std::string & host, uint16_t port) {
	TLOCK;
	std::cout << "[" << __FUNCTION__ << "] " << host << ":" << port << std::endl;
	TULOCK;

	// Start the next receive
	Recv();
}

void ServerConnection::OnSend(const std::vector< uint8_t > & buffer) {
	TLOCK;
	std::cout << "Sent: " << buffer_to_str(buffer) << std::endl;
	TULOCK;
}

void ServerConnection::OnRecv(std::vector<uint8_t> & buffer) {
	TLOCK;
	TULOCK;

	std::cout << "Request: " << buffer_to_str(buffer)<<std::endl;

	std::vector<uint8_t> buff = str_to_buffer("HTTP/1.1 302 Found\r\n"
		"Cache-Control: private\r\n"
		"Content - Type : text / html; charset = UTF - 8\r\n"
		"Location: http://www.google.co.uk/?gfe_rd=cr&ei=9oQoVZKUAYLj8wez24GQCA\r\n"
		"Content - Length : 261\r\n"
		"Date : Sat, 11 Apr 2015 02 : 20 : 38 GMT\r\n"
		"Server : GFE / 2.0\r\n"
		"Alternate - Protocol : 80 : quic, p = 0.5\r\n\r\n"
		"<HTML><HEAD><meta http - equiv = \"content-type\" content = \"text/html;charset=utf-8\">\r\n"
		"<TITLE>302 Moved< / TITLE>< / HEAD><BODY>\r\n"
		"<H1>302 Moved< / H1\r>\n"
		"The document has moved\r\n"
		"<A HREF = \"http://www.google.co.uk/?gfe_rd=cr&amp;ei=9oQoVZKUAYLj8wez24GQCA\">here< / A>.\r\n"
		"< / BODY>< / HTML>");

	// Start the next receive
	Recv();

	Send(buff);
}

void ServerConnection::OnTimer(const boost::posix_time::time_duration & delta) {
	TLOCK;

	TULOCK;
}

void ServerConnection::OnError(const boost::system::error_code & error) {
	TLOCK;
	std::cout << "[" << __FUNCTION__ << "] " << error << std::endl;
	TULOCK;
}

// Server Acceptor:
bool ServerAcceptor::OnAccept(boost::shared_ptr< Connection > connection, const std::string & host, uint16_t port) {
	TLOCK;
	std::cout << "[" << __FUNCTION__ << "] " << host << ":" << port << std::endl;
	TULOCK;

	return true;
}

void ServerAcceptor::OnTimer(const boost::posix_time::time_duration & delta) {
	TLOCK;
	
	TULOCK;
}

void ServerAcceptor::OnError(const boost::system::error_code & error) {
	TLOCK;
	std::cout << "[" << __FUNCTION__ << "] " << error << std::endl;
	TULOCK;
}

// Server Handler:
void NetServer::server_work() {
	while (1) {
		updateHive();

		if (_kbhit()) break;
	}
}
