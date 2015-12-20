#include <iostream>
#if defined(_WIN32) || defined(WIN32)
    #include <winsock2.h> //socket
    #include <unistd.h>
#else
;
#endif

#include <winsock2.h> //socket
#include <unistd.h>
#include <thread>


using namespace std;

#include "../Utils.h"
#include "../include/clientClass.h"
#include "../include/serverClass.h"
#include "../include/serveur_exception.h"

using namespace cgServer::clientClass;

namespace cgServer{
namespace serverClass{

    serverClass::serverClass(){
        wsaInit();
        port = 1607;
        breakloop = false;
    }

    serverClass::~serverClass(){
        wsaClean();
        cout << "Arrêt du service." << endl;
    }

    void serverClass::wsaInit(){
    #if defined(_WIN32) || defined(WIN32)
        WSADATA wsa;
        if(WSAStartup(MAKEWORD(2, 2), &wsa) < 0)
        {
            cerr << "WSAStartup failed !";
            exit(EXIT_FAILURE);
        }
    #endif
    }

    void serverClass::wsaClean(){
    #if defined(_WIN32) || defined(WIN32)
        WSACleanup();
    #endif
    }

    SOCKET serverClass::initService() throw(){
        breakloop = false;

        socklen_t recsize = sizeof(sin);
        sock = socket(AF_INET , SOCK_STREAM , 0 );
        if(sock == INVALID_SOCKET)
            throw serveur_exception("L'initialisation du serveur a échoué.");

        sin.sin_addr.s_addr = htonl(INADDR_ANY); /* Adresse IP automatique */
        sin.sin_port = htons(port); /* Listage du port */
        sin.sin_family = AF_INET; /* Protocole familial (IP) */

        if(bind(sock, (SOCKADDR*)&sin, recsize)==-1)
            throw serveur_exception("Le binding a échoué.");
        if(listen(sock, 5)==-1)
            throw serveur_exception("La mise en écoute a échoué.");

        cout << "Serveur initialisé sur le PORT "<< port << " !"<<endl;

        SOCKET csock;
        SOCKADDR_IN csin;
        socklen_t crecsize = sizeof(csin);
        csock = accept(sock, (SOCKADDR*)&csin, &crecsize);
        if((int)csock == SOCKET_ERROR)
            throw serveur_exception("Une erreur est survenue lors de la connexion du client.");
        cout << "Une nouvelle sur la connexion sur la socket " << csock << "." << endl;
        return csock;
    }

    int serverClass::startService() throw(){

        cout << "Serveur initialisé ..." << endl;

        //initialisation de la session
        session.clear();

        while(true){
            SOCKET csock = initService();

            //nouveau thread;
            thread sessionparallel(startSession,csock);

            stopService();

            int stop = 1;
            /*cout << "arreter ? (1/0) " ;
            cin >> stop;*/

            if(stop==1)
                breakloop = true;

            if(breakloop)
                break;
        }
        return 0;
    }

    void serverClass::stopService(){
        shutdown(sock, 2);
        closesocket(sock);
    }

    int serverClass::startSession(const SOCKET& cli_sock) throw(){
        //gestion du client actuel
        asClient* client = new asClient(cli_sock);
        session.addClient(client);

        //traitements concernant la session

        stopSession(*client);

        return 0;
    }

    void serverClass::stopSession(const asClient &client){
        shutdown(client.c_socket(), 2);
        closesocket(client.c_socket());
        session.removeClient(client);
    }

}
}
