#ifndef MODULECLIENTCLASS_H
#define MODULECLIENTCLASS_H

#include <iostream>
#include <chrono>
#include <exception>
#if defined(_WIN32) || defined(WIN32)
    #include <winsock2.h> //socket
    #include <unistd.h>
#else
    #include <arpa/inet.h>
    #include <unistd.h>
    #include <sys/socket.h> 
    #include <errno.h>

    #define BUFLEN 1024  //Max length of buffer 
    #define INVALID_SOCKET -1
    #define SOCKET_ERROR -1

    typedef int SOCKET;
    typedef struct sockaddr_in SOCKADDR_IN;
    typedef struct sockaddr SOCKADDR;
    typedef struct in_addr IN_ADDR;
#endif

using namespace std;

#include "../include/protoClass.h"
using namespace cgServer::protoClass;

namespace cgApplication{
namespace moduleClientClass{
	class moduleClientClass{
		public :
			protoClass comm;
			bool isRunning;

			moduleClientClass();
			virtual ~moduleClientClass();

			int id() const{return _id;};
			void id(int val){ _id = val;};
			string pseudo() const{return _pseudo;};
			void pseudo(string val){ _pseudo = val;};
			SOCKET sock() const{return _sock;};
			void sock(SOCKET val){ _sock = val;};

			int sconnect(string const& ip, int const& port) ;
			void sdisconnect();
			void init(protoClass & comm, string const& pseudo);

		private :
			int _id = 0;
			string _pseudo = "";
			chrono::time_point<chrono::system_clock> _connexion_time = chrono::system_clock::now();
            SOCKET _sock;
            SOCKADDR_IN _sin;

			void wsaInit();
			void wsaClean();
	};
	typedef moduleClientClass asMClient;
}
}
#endif //MODULECLIENTCLASS_H