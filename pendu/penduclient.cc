#include <iostream>
#include <thread>
#include <sstream> 
#include <vector> 
#include <cstdlib>  //itoa
#include <string.h>  //itoa

using namespace std;

#include "sgmtp.h"
#include "pendu.h"

        

using namespace cgServer::moduleClientClass;
using namespace cgServer::protoClass; 



void* on_end(void* args){
    asMClient* c = (asMClient*)args;     
    cout << "[END]" << c->comm.data() << endl;
    return NULL;
}

void* on_notif(void* args){
    asMClient* c = (asMClient*)args; 
    cout << "[" << c->comm.methode() << "-" << c->comm.param() << "]" << c->comm.data() << endl;
    return NULL;
}

void run(void* args){
    asMClient *c = (asMClient*)args ;
    string ans = "";
    
    while(ans != "quit"){  
        try{
            cout << "Dest : ";
            getline(cin,ans); c->comm.dest(ans);
            cout << "Meth : "; 
            getline(cin,ans); c->comm.methode(ans);
            cout << "Param : ";
            getline(cin,ans); c->comm.param(ans);
            cout << "Value : ";
            getline(cin,ans); c->comm.data(ans);
            c->sendRequest(c->comm.dest(),c->comm.methode(),c->comm.param(),c->comm.data());              
        }catch(write_exception e){
            cout << e.what() << endl;
        }catch(serveur_exception e){
            cout << e.what() << endl;
        }
    }
}

int main(){
    asMClient cgc;  
    cgc.comm.binds.push_back({asBind::TYPE_M|asBind::TYPE_P, string(REQUETE(_session)+"-"+NET_PARAM_END), on_end, &cgc});/*
    cgc.comm.binds.push_back({asBind::TYPE_M|asBind::TYPE_P, string(REQUETE(_post)+"-"+NET_PARAM_NBTRIES), on_notif, &cgc}); 
    cgc.comm.binds.push_back({asBind::TYPE_M|asBind::TYPE_P, string(REQUETE(_post)+"-"+NET_PARAM_WORDUPDATED), on_notif, &cgc}); 
    cgc.comm.binds.push_back({asBind::TYPE_M|asBind::TYPE_P, string(REQUETE(_post)+"-"+NET_PARAM_NOTIFY), on_notif, &cgc}); */
    cgc.comm.binds.push_back({asBind::TYPE_M, string(REQUETE(_post)), on_notif, &cgc});  
    cgc.comm.binds.push_back({asBind::TYPE_M+asBind::TYPE_P+asBind::TYPE_D, string(REQUETE(_post)+"-nbTries-5"), on_notif, &cgc}); 
    cgc.bind(true); 
        
    try{
        cgc.sconnect("127.0.0.1",1607);
        cgc.init(cgc.comm,"Pendu Client");  
        thread threadlisten(&moduleClientClass::tlisten, &cgc);  
        thread threadwrite(run,&cgc);
        
        threadwrite.join();     
        threadlisten.join();

	cgc.sdisconnect();
    }catch(serveur_exception e){
        cout << e.what() << endl;
    }
    
    return 0;
}