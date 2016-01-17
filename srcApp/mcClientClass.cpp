
#include "../includeApp/mcClientClass.h"
#include "../include/serveur_exception.h"


namespace cgApplication{
namespace moduleClientClass{

	moduleClientClass::moduleClientClass(){
	    wsaInit();
	}
	moduleClientClass::~moduleClientClass(){
	    wsaClean();
	}

	void moduleClientClass::wsaInit(){
	#if defined(_WIN32) || defined(WIN32)
	    WSADATA wsa;
	    if(WSAStartup(MAKEWORD(2, 2), &wsa) < 0)
	    {
	        cerr << "WSAStartup failed !";
	        exit(EXIT_FAILURE);
	    }
	#endif
	}

	void moduleClientClass::wsaClean(){
	#if defined(_WIN32) || defined(WIN32)
	    WSACleanup();
	#endif
	}

	int moduleClientClass::sconnect(string const& ip, int const& port) {
		sock(socket(AF_INET, SOCK_STREAM, 0));
		if(sock() == INVALID_SOCKET)
			throw serveur_exception("L'initialisation du client a échoué.");

		_sin.sin_addr.s_addr = inet_addr(ip.c_str()); /* l'adresse se trouve dans le champ h_addr de la structure hostinfo */
		_sin.sin_port = htons(port); /* on utilise htons pour le port */
		_sin.sin_family = AF_INET;

		if(connect(sock(),(SOCKADDR *) &_sin, sizeof(SOCKADDR)) == SOCKET_ERROR)
			throw serveur_exception("La connexion au serveur a échoué.");

		return 0;
	}

	void moduleClientClass::sdisconnect(){
		close(sock());
		sock(-1);
	}
}
}
