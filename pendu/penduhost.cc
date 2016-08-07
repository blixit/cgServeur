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

typedef struct _gamesession{
    string cli_id;
    string cli_pseudo;
    string hiddenworld;
    string currentworld;
    int tries;
} GameSession;

vector <GameSession*> sessions;

void* on_start(void* args){
    asMClient* c = (asMClient*)args; 
    
    for(auto _gs : sessions){
        if(_gs->cli_id == c->comm.src()){
            c->post(_gs->cli_id,NET_PARAM_NOTIF,"End the current session before starting another");
            return NULL;
        } 
    }
    GameSession* gs = new GameSession();
    gs->cli_id = c->comm.src();  
    gs->cli_pseudo = "gamer"+gs->cli_id; cout << "pseudo : " << gs->cli_pseudo << endl;
    gs->tries = 5;
    gs->hiddenworld = "Bonjour";
    gs->currentworld.resize(gs->hiddenworld.size());
    for(int i(0); i< (int)gs->hiddenworld.size();i++)
        gs->currentworld[i] = '_';
    sessions.push_back(gs);
    
    c->post(gs->cli_id,NET_PARAM_NBTRIES,"5");
    c->post(gs->cli_id,NET_PARAM_WORDUPDATED,gs->currentworld);
    return NULL;
}

void* on_tryw(void* args){
    asMClient* c = (asMClient*)args;  
    GameSession* gs;
    unsigned int pos(0);
    for(auto _gs : sessions){
        if(_gs->cli_id == c->comm.src()){
            gs = _gs;
            break;
        }
        pos++;
    }
    if(pos == sessions.size())
        return NULL;
    
    if(c->comm.data() == gs->hiddenworld){
        sessions.erase(sessions.begin()+pos);
        c->session(gs->cli_id,NET_PARAM_END,"You win");
    }else{
        gs->tries--;
        c->post(gs->cli_id,NET_PARAM_NOTIF,"You are wrong");
        if(gs->tries == 0){
            sessions.erase(sessions.begin()+pos);
            c->session(gs->cli_id,NET_PARAM_END,"Game Over");
        }else{
            c->post(gs->cli_id,NET_PARAM_NBTRIES,SSTR(gs->tries));
        }
    } 
    return NULL;
}

void* on_tryl(void* args){
    asMClient* c = (asMClient*)args;    
    GameSession* gs;
    unsigned int pos(0);
    for(auto _gs : sessions){
        if(_gs->cli_id == c->comm.src()){
            gs = _gs;
            break;
        }
        pos++;
    }
    if(pos == sessions.size())
        return NULL;
    
    if(c->comm.data().length()>0){
        char l = c->comm.data().at(0);
        int posl = gs->hiddenworld.find(l);
        if(posl >= 0){
            for(int i(0); i<(int)gs->hiddenworld.size();i++){
                gs->currentworld[i] = gs->hiddenworld[i] == l ? l : gs->currentworld[i];
            }
            if(gs->currentworld != gs->hiddenworld){
                c->post(gs->cli_id,NET_PARAM_NOTIF,"Bad ass !! you got it !!");
                c->post(gs->cli_id,NET_PARAM_WORDUPDATED,gs->currentworld);
            }else{
                sessions.erase(sessions.begin()+pos); 
                c->session(gs->cli_id,NET_PARAM_END,"You win");
            }
        }else{
            gs->tries--;
            c->post(gs->cli_id,NET_PARAM_NOTIF,"You are wrong");
            if(gs->tries == 0){
                sessions.erase(sessions.begin()+pos); 
                c->session(gs->cli_id,NET_PARAM_END,"Game Over");
            }else{
                c->post(gs->cli_id,NET_PARAM_NBTRIES,SSTR(gs->tries));
            }
        } 
    }
         
    return NULL;
}
 
int main(){
    asMClient cgc; 
    cgc.comm.binds.push_back({asBind::TYPE_M|asBind::TYPE_P, string(REQUETE(_session)+"-"+NET_PARAM_START), on_start, &cgc});
    cgc.comm.binds.push_back({asBind::TYPE_M|asBind::TYPE_P, string(REQUETE(_post)+"-"+NET_PARAM_TRYW), on_tryw, &cgc}); 
    cgc.comm.binds.push_back({asBind::TYPE_M|asBind::TYPE_P, string(REQUETE(_post)+"-"+NET_PARAM_TRYL), on_tryl, &cgc}); 
    cgc.bind(true);
    
    sessions.clear();
        
    try{
        cgc.sconnect("127.0.0.1",1607);
	    cgc.init(cgc.comm,"Pendu Host"); 
            cout << "Is running : " <<  cgc.isRunning << endl;
	    thread threadlisten(&moduleClientClass::tlisten, &cgc); 
            
            threadlisten.join();

	    cgc.sdisconnect();
    }catch(serveur_exception e){
        cout << e.what() << endl;
    }
    
    return 0;
}