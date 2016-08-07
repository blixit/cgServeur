#include <iostream>
#include <thread>
#include <sstream>

using namespace std;

#include "include/mcClientClass.h"
#include "include/protoClass.h"
#include "include/serveur_exception.h"

using namespace cgServer::moduleClientClass;
using namespace cgServer::protoClass;

std::thread::id listen_thread_id;
std::thread::id write_thread_id;

void test(int *t){
	cout << "test" << endl;
}

void tlisten(asMClient* cgc){
	listen_thread_id = std::this_thread::get_id();
	while(cgc->isRunning){
		try{
			cgc->comm.read(cgc->sock());
			cout << cgc->comm.requete() << endl;
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
			string param, dest;
			cout << "dest : " << endl;
	    	cin >> dest;

			cout << "param : " << endl;
	    	cin >> param;
 			 
 			stringstream stream;
 			stream << cgc->id();
    		cgc->comm.build(dest,stream.str(),REQUETE(_post),param,cgc->pseudo());  //sending p-test
		    //cout << "send test " << comm.requete() << endl;
		
			cgc->comm.write(cgc->sock());
		}catch(write_exception e){
			cout << e.what() << endl;
		}catch(serveur_exception e){
			cout << e.what() << endl;
		}
	}
}

int main(){ 
    asMClient cgc,cgc2; 

    //launching serveur
    system("xterm ./cgserveur.exe &");


    try{
    	string ip;
    	cout << "ip : ";
    	cin >> ip;
    	cgc.sconnect(ip,1607);
	    cgc.init(cgc.comm,"Alain"); 

	    int t=0;
	    thread threadtest(test,&t);
	    thread threadlisten(tlisten, &cgc);
	    thread threadwrite(twrite, &cgc);

	    threadtest.join();
	    threadwrite.join();
	    threadlisten.join();
	    cgc.sdisconnect();
    }catch(serveur_exception e){
    	cout << e.what() << endl; 
    }

	return 0;
}