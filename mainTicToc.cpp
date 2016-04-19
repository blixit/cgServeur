#include <iostream>
#include <thread>
#include <sstream>

using namespace std;

#include "include/mcClientClass.h"
#include "include/protoClass.h"
#include "include/serveur_exception.h"

using namespace cgApplication::moduleClientClass;
using namespace cgServer::protoClass;

#include "srcGame/tt.cc"
#include "srcGame/event.cc" 

/**
Le segmenfault qu"on sort de la boucle vient du fait qu'on passe des pointeurs (locaux) au
thread boucle. Il faudrait déclarer cgc en globale ou envisager d'utiliser des 
shared_ptr
http://stackoverflow.com/questions/14148412/c11-stdthreaddetach-and-access-to-shared-data
*/
int main(){ 
    asMClient cgc; 

    //launching serveur
    system("xterm ./cgserveur.exe &");
    sleep(1);
     
    //cgc.comm.databinds.push_back({"invite",on_invite,&cgc});
    cgc.comm.binds.push_back({string(REQUETE(_invite)+"-"+NET_PARAM_INV_SEND), on_invite, &cgc});
    cgc.comm.binds.push_back({string(REQUETE(_invite)+"-"+NET_PARAM_INV_ACCEPT), on_accept, &cgc});
    cgc.comm.binds.push_back({string(REQUETE(_post)+"-"+NET_UPGRID_xy), on_position, &cgc});
    cgc.comm.binds.push_back({string(REQUETE(_post)+"-"+NET_PARAM_PSEUDO), on_pseudo, &cgc});
    cgc.comm.binds.push_back({string(REQUETE(_sms)+"-"+NET_PARAM_SMS_SEND), on_sms, &cgc});
    cgc.bind(true);
    
    try{ 
    	cgc.sconnect("127.0.0.1",1607);
	    cgc.init(cgc.comm,"alain"); 

	    cgc.get(NET_SERVER_ADDR,NET_PARAM_PSEUDO);
	    cgc.sms(NET_SERVER_ADDR,"coucouc");
  
	    thread threadlisten(&moduleClientClass::tlisten, &cgc);
	    thread threadwrite(&moduleClientClass::twrite, &cgc);
	    sleep(2);
	    cout << "id thread : " << cgc.write_thread_id << endl;
 
	    threadwrite.join();
	    threadlisten.join();

	    cgc.sdisconnect();
    }catch(serveur_exception e){
    	cout << e.what() << endl; 
    }

	return 0;
}