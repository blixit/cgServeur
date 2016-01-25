
#include "../includeApp/mcClientClass.h"
#include "../include/serveur_exception.h"


namespace cgApplication{
namespace moduleClientClass{

	moduleClientClass::moduleClientClass() : comm(false){
	    wsaInit();
	    isRunning = false; 
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

		isRunning = true;

		return 0;
	}

	void moduleClientClass::sdisconnect(){
		close(sock());
		sock(-1);
		isRunning = false;
	}

	void moduleClientClass::init(protoClass & comm, string const& pseudoUser){
		pseudo(pseudoUser);
		comm.build(NET_BRCAST_ADDR,NET_ANY_ADDR,REQUETE(_post),NET_PARAM_PSEUDO,pseudoUser); 
	    //cout << "send " << comm.requete() << endl;
	    comm.write(sock());

	    //identifiant
	    comm.read(sock());
	    id(stoi(comm.data().c_str()));
	    cout << "id " << id() << endl;
	    //bcast notif connexion
	    comm.read(sock());
	    cout << "notif " << comm.data() << endl;
	    //bcast nbc
	    comm.read(sock());
	    cout << "nbC " << comm.data() << endl;
	    int nbClients = atoi(comm.data().c_str()), i=0;
	    //update list
	    while(i<nbClients){
	    	comm.read(sock());
	    	cout << "client °"<<  i++ << " : " << comm.data() << endl; 
	    }
	}
	void* moduleClientClass::on_invite(void* args){
		return NULL;
	}
}
}
