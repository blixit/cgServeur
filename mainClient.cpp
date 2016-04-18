#include <iostream>
#include <sstream> // std::stringstream, std::stringbuf
#include <exception>
//#include <time.h> // rand

using namespace std;

#include "includeApp/mcClientClass.h"
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
    asProto comm(false);

    //Binding some functions
    comm.binds.push_back({"p-test", test, NULL});
    comm.binds.push_back({"p-retrieve", retrieve, &comm});   
    comm.binds.push_back({"p-shut", disconnect, &cgc});   

    try{
	    cgc.sconnect("127.0.0.1",1607);
	    cout << "sock " << cgc.sock() << endl;

	    cgc.init(comm,"Alain");

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

	    		stringstream stream;
	    		stream << cgc.id();
	    		comm.build(dest,stream.str(),REQUETE(_post),param,"alain");  //sending p-test
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
	}catch(write_exception e1){
		cout << "write_exception : "<< endl;
		cout << e1.what() << endl;
	}catch(serveur_exception e1){
		cout << e1.what() << endl;
	}


    return 0;
}
