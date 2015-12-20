#ifndef CLIENTCLASS_H
#define CLIENTCLASS_H

#include <iostream>
#include <chrono>
#if defined(_WIN32) || defined(WIN32)
    #include <winsock2.h> //socket
    #include <unistd.h>
#else
;
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
            SOCKET c_number() const { return _number;};
            void c_number(const size_t &val) { _number=val;};
        private :
            SOCKET _sock;

            size_t _number; //!< position dans la liste des clients

            std::string _pseudo;
            int _rank;
            chrono::time_point<chrono::system_clock> _connexion_time = chrono::system_clock::now();


    };
    typedef clientClass asClient;
}
}

#endif // CLIENTCLASS_H
