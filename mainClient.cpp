#include <iostream>
#include <exception>
#include <time.h> // rand

using namespace std;

#include "includeApp/mcClientClass.h"
#include "include/protoClass.h"
#include "include/serveur_exception.h"

using namespace cgApplication::moduleClientClass;
using namespace cgServer::protoClass;


int main()
{
	srand(time(NULL));
    asMClient cgc;
    asProto comm;
    try{
	    cgc.sconnect("127.0.0.1",1607);
	    cout << "sock " << cgc.sock() << endl;

	    
	    comm.build(NET_BRCAST_ADDR,NET_ANY_ADDR,REQUETE(_post),NET_PARAM_PSEUDO,"alain"); 
	    cout << "send " << comm.requete() << endl;
	    comm.write(cgc.sock());

	    //identifiant
	    comm.read(cgc.sock());
	    string id = comm.data();
	    cout << "id " << comm.data() << endl;
	    //bcast notif connexion
	    comm.read(cgc.sock());
	    cout << "notif " << comm.data() << endl;
	    //bcast nbc
	    comm.read(cgc.sock());
	    cout << "nbC " << comm.data() << endl;
	    //update list
	    comm.read(cgc.sock());
	    cout << "client1 " << comm.data() << endl; 

	    comm.build(NET_SERVER_ADDR,id,REQUETE(_post),NET_SHUT_DOWN,"alain"); 
	    cout << "send " << comm.requete() << endl;
	    comm.write(cgc.sock());

	    cgc.sdisconnect();
	}catch( const char  e[]){
		cout << e << endl;
	}catch(const serveur_exception& e1){
		cout << e1.what() << endl;
	}


    return 0;
}
