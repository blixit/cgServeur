#ifndef CLIENTCLASS_H
#define CLIENTCLASS_H

#include <iostream>
#include <chrono>
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

namespace cgServer{
namespace clientClass{
    /** \brief  Implémentation de l'objet client côté serveur
     */

    class clientClass{
        public :
            clientClass(const int &sock=0);
            virtual ~clientClass();

            SOCKET c_socket() const { return _sock;};
            void c_socket(SOCKET val) { _sock = val;};
            SOCKET c_number() const { return _number;};
            void c_number(const size_t &val) { _number=val;};
            string c_pseudo() const {return _pseudo;};
            void c_pseudo(string val) {_pseudo = val;};
            bool routineIdentificationDone() const {return _routineIdentificationDone;};
            void routineIdentificationDone(const bool& val){_routineIdentificationDone = val;};
        private :
            SOCKET _sock;

            size_t _number; //!< position dans la liste des clients
            std::string _pseudo;
            int _rank;
            chrono::time_point<chrono::system_clock> _connexion_time = chrono::system_clock::now();

            bool _routineIdentificationDone = false;


    };
    typedef clientClass asClient;
}
}

#endif // CLIENTCLASS_H
