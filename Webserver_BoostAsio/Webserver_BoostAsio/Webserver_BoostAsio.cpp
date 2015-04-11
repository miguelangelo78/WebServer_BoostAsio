#include "stdafx.h"
#include "net/net_handler.h"

int _tmain(int argc, _TCHAR* argv[]) {
	NetServer webserver("127.0.0.1", 80);
	return 0;
}
