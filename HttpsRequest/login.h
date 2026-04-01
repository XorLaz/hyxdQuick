#pragma once

#include <string>
#include <stdio.h>
#include "Request_function.h"
#ifdef WIN32
#pragma comment(lib,"ws2_32.lib")
#endif
#include "Https_Request.hpp"


inline std::string loginHyxdQuick(std::string card,std::string Vis) {

	versions = Vis;
	cards = card;
	//hyxdQuickÊÚÈ¨ÂëµÇÂ¼
	if (login::keys_login()) {
		return VMProtectDecryptStringA("Yes");
	}
	else {
		return login::OutStr(str);
	}

}


