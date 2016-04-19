#include <thread>
#include <sstream>
#include "../include/mcClientClass.h"
#include "../include/serveur_exception.h" 

using namespace std;

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

	void moduleClientClass::tlisten(){
		listen_thread_id = std::this_thread::get_id();
		while(isRunning){
			try{
				comm.read(sock());
				//cout << comm.requete() << endl;
				if(_bind)
					comm.bind(); 
				if(_binddata)
					comm.bindData(); 
				//thread threadaction(on_bind,cgc);
				//threadaction.detach(); 
			}catch(read_exception e){
				if(comm.requete().length()==0)
					isRunning = false;
				cout << e.what() << endl;
			}catch(exception e){
				cout << "ERREUR" << endl;
				cout << e.what() << endl;
			}
		}
	}

	void moduleClientClass::twrite(){
		write_thread_id = std::this_thread::get_id();
		string waiter;
		cout << "waiter ? " ;
		cin >> waiter;
		if(waiter!="0")
			return;

		while(isRunning){
			try{
				string meth, param, dest,value;
				cout << "dest : " << endl; 
		    	getline(cin,dest);

				cout << "meth : " << endl; 
				getline(cin,meth);

				cout << "param : " << endl; 
				getline(cin,param);

				cout << "value : " << endl; 
				getline(cin,value);

				sendRequest(dest,meth,param,value); 
			}catch(write_exception e){
				cout << e.what() << endl;
			}catch(serveur_exception e){
				cout << e.what() << endl;
			}
		}
	}

	void moduleClientClass::sendRequest(string const& dest, string const& meth, string const& param, string const& data){
		stringstream stream;
	 	stream << id();
		comm.build(dest,stream.str(),meth,param,data);
		comm.write(sock());
	}

	void moduleClientClass::get(string const& dest, string const& param){
		sendRequest(dest,REQUETE(_get),param,"");
	}

	void moduleClientClass::post(string const& dest, string const& param, string const& value){
		sendRequest(dest,REQUETE(_post),param,value);
	}

	void moduleClientClass::sms(string const& dest, string const& msg){
		sendRequest(dest,REQUETE(_sms),NET_PARAM_SMS_SEND,msg);
	}

	void moduleClientClass::update(string const& dest, string const& param, string const& value){
		sendRequest(dest,REQUETE(_update),param,value);
	} 

	void moduleClientClass::del(string const& dest, string const& param){
		sendRequest(dest,REQUETE(_delete),param,"");
	}

	void moduleClientClass::session(string const& dest, string const& param, string const& value){
		sendRequest(dest,REQUETE(_session),param,value);
	}

	void moduleClientClass::invite(string const& dest, string const& param){
		sendRequest(dest,REQUETE(_invite),param,"");
	}

	void moduleClientClass::file(string const& dest, string const& param, string const& value){
		sendRequest(dest,REQUETE(_file),param,value);
	} 
}
}
