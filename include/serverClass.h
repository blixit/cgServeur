#ifndef SERVERCLASS_H
#define SERVERCLASS_H

#if defined(_WIN32) || defined(WIN32)
    #include <winsock2.h> //socket
    #include <unistd.h>
#else
;
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

            int startSession(const SOCKET& cli_sock) throw();
            void stopSession(const asClient &client);
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
