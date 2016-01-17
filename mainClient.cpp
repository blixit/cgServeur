#include <iostream>
#include <exception>
#include <time.h> // rand

using namespace std;

#include "includeApp/mcClientClass.h"
#include "include/protoClass.h"
#include "include/serveur_exception.h"

using namespace cgApplication::moduleClientClass;
using namespace cgServer::protoClass;



void* test(void*){
	cout << "bindings test" << endl;
	return NULL;
}

void* retrieve(void* _com){
	if(_com == NULL){ 
		return NULL;
	} 
	asProto* comm = (asProto*)_com;


	cout << "bindings retrieve : " << endl;
	cout << comm->data() << endl;
	return NULL;
}

void* disconnect(void* data){
	cout << "bindings disconnect : " << endl;
	if(data == NULL){ 
		return NULL;
	} 
	asMClient* cgc = (asMClient*)data;
	cgc->sdisconnect();
	return NULL;
}

int main()
{
	srand(time(NULL));
    asMClient cgc;
    asProto comm;

    //Binding some functions
    comm.binds.push_back({"p-test", test, NULL});
    comm.binds.push_back({"p-retrieve", retrieve, &comm});   
    comm.binds.push_back({"p-shut", disconnect, &cgc});   

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
	    int nbClients = atoi(comm.data().c_str()), i=0;
	    //update list
	    while(i<nbClients){
	    	comm.read(cgc.sock());
	    	cout << "client °"<<  i++ << " : " << comm.data() << endl; 
	    }


	    //nb = sender, 0 = receiver
	    cout << "sender : " ; 
	    std::string dest, param; 
	    cin >> dest;
	    cout << dest << endl;

	    while(comm.param() != "shut"){
	    	//code sender
	    	if(dest!="0"){ 
	    		cout << "param : " << endl;
	    		cin >> param;
	    		comm.build(dest,id,REQUETE(_post),param,"alain");  //sending p-test
			    cout << "send test " << comm.requete() << endl;
			    comm.write(cgc.sock()); 
	    	} else{ //code receiver
		    	comm.read(cgc.sock()); 
		    	comm.bind(); 
		    }
	    }

	    /*/shut server
	    comm.build(NET_SERVER_ADDR,id,REQUETE(_post),NET_SHUT_DOWN,"alain"); 
	    cout << "send " << comm.requete() << endl;
	    comm.write(cgc.sock());*/

	    cgc.sdisconnect();
	}catch( const char  e[]){
		cout << e << endl;
	}catch(const serveur_exception& e1){
		cout << e1.what() << endl;
	}


    return 0;
}
