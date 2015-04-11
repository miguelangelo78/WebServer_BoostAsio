#pragma once
#include "net_sockapi\network_sockapi.h"
#include <boost\thread\mutex.hpp>
#include <conio.h>

extern boost::mutex global_lock;
#define TLOCK global_lock.lock();
#define TULOCK global_lock.unlock();

// Server events:
class ServerConnection : public Connection
{
private:
	void OnAccept(const std::string & host, uint16_t port);
	void OnConnect(const std::string & host, uint16_t port);
	void OnSend(const std::vector< uint8_t > & buffer);
	void OnRecv(std::vector< uint8_t > & buffer);
	void OnTimer(const boost::posix_time::time_duration & delta);
	void OnError(const boost::system::error_code & error);
public:
	ServerConnection(boost::shared_ptr< Hive > hive) :Connection(hive) {}
	~ServerConnection(){}
};

class ServerAcceptor : public Acceptor
{
private:
	bool OnAccept(boost::shared_ptr< Connection > connection, const std::string & host, uint16_t port);
	void OnTimer(const boost::posix_time::time_duration & delta);
	void OnError(const boost::system::error_code & error);

public:
	ServerAcceptor(boost::shared_ptr< Hive > hive) :Acceptor(hive) {}
	~ServerAcceptor(){}
};

// Client events:
class ClientConnection : public Connection {
private:
	void OnAccept(const std::string & host, uint16_t port);
	void OnConnect(const std::string & host, uint16_t port);
	void OnSend(const std::vector< uint8_t > & buffer);
	void OnRecv(std::vector< uint8_t > & buffer);
	void OnTimer(const boost::posix_time::time_duration & delta);
	void OnError(const boost::system::error_code & error);
public:
	ClientConnection(boost::shared_ptr< Hive > hive) : Connection(hive) {}
	~ClientConnection() {}
};

typedef boost::shared_ptr<Hive> sptr_hive_t;
typedef boost::shared_ptr<ServerAcceptor> sptr_sacceptor_t;
typedef boost::shared_ptr<ServerConnection> sptr_sconn_t;
typedef boost::shared_ptr<ClientConnection> sptr_cconn_t;

#define NEW_HIVE new Hive()
#define NEW_SACCEPTOR(hive) new ServerAcceptor(hive)
#define NEW_SCONNECTION(hive) new ServerConnection(hive)
#define NEW_CCONNECTION(hive) new ClientConnection(hive)

// Handlers:

// Server handler:
class NetServer {
public:
	NetServer(std::string host, uint16_t port) :hive(NEW_HIVE), acceptor(NEW_SACCEPTOR(hive)), conn(NEW_SCONNECTION(hive)) {
		acceptor->Listen(host, port);
		acceptor->Accept(conn);
		server_work();
	}
	~NetServer() {
		hive->Stop();
	}

private:
	sptr_hive_t hive;
	sptr_sacceptor_t acceptor;
	sptr_sconn_t conn;

	void updateHive() {
		hive->Poll();
		Sleep(1);
	}
	void server_work();
};

// Client handler:
class NetClient {
public:
	NetClient(std::string host, uint16_t port):hive(NEW_HIVE), conn(NEW_CCONNECTION(hive)) {
		conn->Connect(host, port);
		client_work();
	}
	~NetClient() {
		hive->Stop();
	}

private:
	void updateHive() {
		hive->Poll();
		Sleep(1);
	}
	void client_work();

	sptr_hive_t hive;
	sptr_cconn_t conn;
};
