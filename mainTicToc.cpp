#include <iostream>
#include <thread>
#include <sstream>

using namespace std;

#include "includeApp/mcClientClass.h"
#include "include/protoClass.h"
#include "include/serveur_exception.h"

using namespace cgApplication::moduleClientClass;
using namespace cgServer::protoClass;

std::thread::id listen_thread_id;
std::thread::id write_thread_id;

void tlisten(asMClient* cgc){
	listen_thread_id = std::this_thread::get_id();
	while(cgc->isRunning){
		try{
			cgc->comm.read(cgc->sock());
			cout << cgc->comm.requete() << endl;
			cgc->comm.bind();
		}catch(read_exception e){
			if(cgc->comm.requete().length()==0)
				cgc->isRunning = false;
			cout << e.what() << endl;
		}
	}
}

void twrite(asMClient* cgc){
	write_thread_id = std::this_thread::get_id();
	while(cgc->isRunning){
		try{
			string meth, param, dest;
			cout << "meth : " << endl;
	    	cin >> meth;

			cout << "dest : " << endl;
	    	cin >> dest;

			cout << "param : " << endl;
	    	cin >> param;
 			 
 			stringstream stream;
 			stream << cgc->id();
    		cgc->comm.build(dest,stream.str(),meth,param,cgc->pseudo());  //sending p-test
		    //cout << "send test " << comm.requete() << endl;
		
			cgc->comm.write(cgc->sock());
		}catch(write_exception e){
			cout << e.what() << endl;
		}catch(serveur_exception e){
			cout << e.what() << endl;
		}
	}
}

void* on_invite(void* args){
	asMClient* c = (asMClient*)args;

	cout << "invited by " << c->comm.src() << endl;

	return NULL;
}

int main(){ 
    asMClient cgc; 

    //launching serveur
    system("xterm ./cgserveur.exe &");

    cgc.comm.binds.push_back({string(REQUETE(_invite)+"-"+NET_PARAM_INV_SEND), on_invite, &cgc});

    try{ 
    	cgc.sconnect("127.0.0.1",1607);
	    cgc.init(cgc.comm,"Alain"); 
 
	    thread threadlisten(tlisten, &cgc);
	    thread threadwrite(twrite, &cgc);
 
	    threadwrite.join();
	    threadlisten.join();
	    cgc.sdisconnect();
    }catch(serveur_exception e){
    	cout << e.what() << endl; 
    }

	return 0;
}