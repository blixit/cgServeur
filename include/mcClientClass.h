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

#include "protoClass.h"

using namespace cgServer::protoClass;

namespace cgApplication{
namespace moduleClientClass{
	class moduleClientClass{
		public :
			protoClass comm;
			bool isRunning;
            std::thread::id listen_thread_id;
			std::thread::id write_thread_id;

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

			void tlisten();
			void twrite();
			void bind(bool val){_bind = val;};
			void binddata(bool val){_binddata = val;};
 
			void sendRequest(string const& dest, string const& meth, string const& param, string const& data);
			void get(string const& dest, string const& param);
			void post(string const& dest, string const& param, string const& value);
			void sms(string const& dest, string const& msg);
			void update(string const& dest, string const& param, string const& value); 
			void del(string const& dest, string const& param); 
			void session(string const& dest, string const& param, string const& value); 
			void invite(string const& dest, string const& param); 
			void file(string const& dest, string const& param, string const& value);  

		private :
			int _id = 0;
			string _pseudo = "";
			chrono::time_point<chrono::system_clock> _connexion_time = chrono::system_clock::now();
            SOCKET _sock;
            SOCKADDR_IN _sin;

            bool _bind = true;
            bool _binddata = false;



			void wsaInit();
			void wsaClean();
	};
	typedef moduleClientClass asMClient;
}
}
#endif //MODULECLIENTCLASS_H