#include "../include/sessionClass.h"


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
        client->c_number(listeClients.size() - 1);

        return client;
    }

    void sessionClass::removeClient(const asClient &client){
        listeClients.erase (listeClients.begin()+client.c_number());
    }

    void sessionClass::clear(){
        listeClients.clear();
    }
}
}
