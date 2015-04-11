#include "stdafx.h"
#include "net/net_handler.h"

int _tmain(int argc, _TCHAR* argv[]) {
	NetServer webserver("192.168.0.2", 80);
	return 0;
}
