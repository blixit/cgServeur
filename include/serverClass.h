#ifndef SERVERCLASS_H
#define SERVERCLASS_H

#include <iostream>
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

#include "sessionClass.h"

using namespace cgServer::sessionClass;

namespace cgServer{
namespace serverClass{
    class serverClass
    {
        public:
            /** Default constructor */
            serverClass();
            /** Default destructor */
            virtual ~serverClass();

            void wsaInit();
            void wsaClean();

            /** \brief Initialise une socket, recoit une connexion et retourne le numéro de la socket
             * \return la socket client
             * \throw En cas d'erreur lève une exception
             */
            SOCKET initService() throw();
            int startService() throw();
            void stopService();

            void startSession(const SOCKET& cli_sock) throw();
            static void* Parallel_startSession(void*) throw();
            void stopSession(const asClient &client);
            static void* Parallel_sessionController(void* data) throw();
        protected:
        private:
            bool breakloop;

            int port;
            SOCKET sock;
            SOCKADDR_IN sin;

            asSession session;
    };
    typedef serverClass asServer;

}
}
#endif // SERVERCLASS_H
