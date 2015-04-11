#include "stdafx.h" // May need to delete this if not using precompiled headers
#include "net_handler.h"

boost::mutex global_lock;

#include <fstream>

bool connected = false;
bool just_disconnected = false;

std::string buffer_to_str(std::vector<uint8_t> buff) {
	std::string str(buff.begin(), buff.end());
	return str;
}

std::vector<uint8_t> str_to_buffer(std::string str) {
	std::vector<uint8_t> buffer;
	std::copy(str.begin(), str.end(), std::back_inserter(buffer));
	return buffer;
}

std::string file_getwhole(std::string filepath) {
	std::string output = "";
	
	try {
		std::string buff;
		std::ifstream file(filepath);
		if (file.is_open()) {

			while (std::getline(file, buff))
				output += buff+"\r\n";

			file.close();
		}
		else throw std::runtime_error("Could not open file '" + filepath + "'");
	}
	catch (std::exception& e) {
		std::cerr << "\nFile exception: " << e.what() << std::endl;
	}

	if (output.empty()) return "<NULL>";
	else return output;
}

///////////////////////////////////////////////////////////////

// Server events:

// Server Connection:
void ServerConnection::OnAccept(const std::string & host, uint16_t port) {
	TLOCK;
	std::cout << "Accepted: Connected to host " << host << " through port " << port << std::endl;
	TULOCK;
	// Start the next receive
	Recv();
}

void ServerConnection::OnConnect(const std::string & host, uint16_t port) {
	TLOCK;
	std::cout << "[" << __FUNCTION__ << "] " << host << ":" << port << std::endl;
	TULOCK;

	connected = true;

	// Start the next receive
	Recv();
}

void ServerConnection::OnDisconnect() {
	std::cout << ">> !Connection with client ended!" << std::endl;
	connected = false;
	just_disconnected = true;
}

void ServerConnection::OnSend(const std::vector<uint8_t> & buffer) {
	TLOCK;
	std::cout << "Sent: " << buffer_to_str(buffer) << std::endl;
	TULOCK;

	// After the client received the html file:
	Disconnect();
}

void ServerConnection::OnRecv(std::vector<uint8_t> & buffer) {
	
	std::cout << "Request: " << buffer_to_str(buffer)<<std::endl;

	std::vector<uint8_t> reply = str_to_buffer(
		"HTTP/1.1 200 OK\r\n"
		"Content-Type: text/html\r\n\r\n" + file_getwhole("..\\web\\index.html"));

	// Start the next receive
	Recv();
	
	Send(reply);
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
int NetServer::server_work() {
	std::cout << "\nListening through port " << port << " . . ." << std::endl;

	// Connected: 
	while (1) {
		updateHive();

		if (just_disconnected) {
			just_disconnected = false;
			break;
		}
	}

	return 1;
}
