#include <stdio.h>
#include <time.h> // rand
#include "../include/sessionClass.h"
#include "../include/protoClass.h"
 
using namespace cgServer::protoClass;


namespace cgServer{
namespace sessionClass{

    sessionClass::sessionClass()
    {

    }

    sessionClass::~sessionClass()
    {
        listeClients.clear();
    }

    asClient* sessionClass::addClient(asClient* client){
        listeClients.push_back(client);
        int alea ;
        bool exist = false;
        do{
            alea = rand()%500;
            std::vector<asClient*>::iterator it = listeClients.begin();
            while(it != listeClients.end() && !exist){
                asClient* cli = *it;
                if(cli->c_number() == alea)
                    exist = true; 
                it++;
            }
        }while(exist);
        client->c_number(alea);
        //listeClients.at(listeClients.size() - 1)->c_number(listeClients.size() - 1);

        return listeClients.at(listeClients.size() - 1);
    }

    void sessionClass::removeClient(const asClient &client){
        //listeClients.erase (listeClients.begin()+client.c_number());
        std::vector<asClient*>::iterator it = listeClients.begin(); 
        while(it != listeClients.end()){
            asClient* cli = *it;
            if(cli->c_number() == client.c_number()){ 
                listeClients.erase(it);
                break;
            }
            it++;
        }
    }

    void sessionClass::clear(){
        listeClients.clear();
    }

    asClient* sessionClass::getClient(const asClient& client){
        std::vector<asClient*>::iterator it = listeClients.begin(); 
        while(it != listeClients.end()){
            asClient* cli = *it;
            if(cli->c_number() == client.c_number()){  
                break;
            }
            it++;
        }
        if(it != listeClients.end())
            return *it;
        return NULL;
    }


    asClient* sessionClass::getClient(const int &numero){
        std::vector<asClient*>::iterator it = listeClients.begin(); 
        while(it != listeClients.end()){
            asClient* cli = *it;
            if(cli->c_number() == numero){  
                break;
            }
            it++;
        }
        if(it != listeClients.end())
            return *it;
        return NULL;
    }

    void sessionClass::routineIdentification(asClient* tmpClient) throw(){
        asClient client = *tmpClient;
        //if(listeClients.at(client.c_number())->routineIdentificationDone())
        if(tmpClient == NULL)
            throw "L'identification a échoué.";

        if(tmpClient->routineIdentificationDone())
            return;

        asProto comm;

        try{
             //réception login  
            comm.read(client.c_socket()); 
            if(comm.param()!=NET_PARAM_PSEUDO)
                throw "[sessionClass::routineIdentification] Données corrompues.";
            //on supprime le '\r\n'
            if(comm.data().at(comm.data().length()-1) == '\n' && comm.data().at(comm.data().length()-2) == '\r')
                comm.data(comm.data().substr(0,comm.data().length()-2));

            //listeClients.at(client.c_number())->c_pseudo(comm.data());
            tmpClient->c_pseudo(comm.data());

            //envoie identifiant
            char buf[4] = {0}; 
            sprintf(buf,"%d",client.c_number());            
            comm.build(string(buf),NET_SERVER_ADDR,REQUETE(_post),NET_PARAM_RANK,string(buf)); 
            comm.write(client.c_socket()); 
        }catch(const char* e){
            cout << e << endl;
            throw e;
        }catch(char const* e){
            cout << "--" << e << endl;
            throw e;
        }catch(exception& e){
            cout << "**" << e.what() << endl;
            throw e.what();
        }

        //listeClients.at(client.c_number())->routineIdentificationDone(true);
        tmpClient->routineIdentificationDone(true);
        
    }

    void sessionClass::broadCast(const string& meth, const string& para, const string& message) throw(){
        asProto comm;
        comm.build(NET_BRCAST_ADDR,NET_SERVER_ADDR,meth,para,message);

        for(asClient* client : listeClients){ 
            comm.write(client->c_socket()); 
        }
    }

    void sessionClass::updateListe() throw(){
        asProto comm;
        if(listeClients.size()==0)
            return;
        
        char buf[4]={0};
        sprintf(buf,"%d",(int)listeClients.size());
        broadCast(REQUETE(_update),NET_PARAM_NBCLIENT,string(buf));

        for(asClient* client : listeClients){ 
            char buf[256] = {0};
            sprintf(buf,"%d %s",client->c_number(),client->c_pseudo().c_str());
            broadCast(REQUETE(_post),NET_PARAM_LISTCLIENTS,buf);
        } 
    }
}
}
