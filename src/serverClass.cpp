
#include <pthread.h> 
#include <tuple>

using namespace std;

#include "../Utils.h"
#include "../include/clientClass.h"
#include "../include/serverClass.h"
#include "../include/serveur_exception.h"
#include "../include/protoClass.h"

using namespace cgServer::clientClass;
using namespace cgServer::protoClass;

namespace cgServer{
namespace serverClass{
    pthread_cond_t cond_sessionModified = PTHREAD_COND_INITIALIZER; /* Création de la condition */
    pthread_mutex_t mutex_sessionModified = PTHREAD_MUTEX_INITIALIZER; /* Création du mutex */

    serverClass::serverClass(){
        wsaInit();
        port = 1607;
        breakloop = false;
    }

    serverClass::~serverClass(){
        wsaClean();
        cout << "Arrêt du service." << endl;
    }

    void serverClass::wsaInit(){
    #if defined(_WIN32) || defined(WIN32)
        WSADATA wsa;
        if(WSAStartup(MAKEWORD(2, 2), &wsa) < 0)
        {
            cerr << "WSAStartup failed !";
            exit(EXIT_FAILURE);
        }
    #endif
    }

    void serverClass::wsaClean(){
    #if defined(_WIN32) || defined(WIN32)
        WSACleanup();
    #endif
    }

    SOCKET serverClass::initService() throw(){
        breakloop = false;

        socklen_t recsize = sizeof(sin);
        sock = socket(AF_INET , SOCK_STREAM , 0 );
        if(sock == INVALID_SOCKET)
            throw serveur_exception("L'initialisation du serveur a échoué.");
        int tmp = 1;
        setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&tmp,sizeof(int));

        sin.sin_addr.s_addr = htonl(INADDR_ANY); /* Adresse IP automatique */
        sin.sin_port = htons(port); /* Listage du port */
        sin.sin_family = AF_INET; /* Protocole familial (IP) */

        if(bind(sock, (SOCKADDR*)&sin, recsize)==-1)
            throw serveur_exception("Le binding a échoué.");
        if(listen(sock, 5)==-1)
            throw serveur_exception("La mise en écoute a échoué.");

        cout << "Serveur en écoute sur le PORT "<< port << " !"<<endl;

        SOCKET csock;
        SOCKADDR_IN csin;
        socklen_t crecsize = sizeof(csin);
        csock = accept(sock, (SOCKADDR*)&csin, &crecsize);
        if((int)csock == SOCKET_ERROR)
            throw serveur_exception("Une erreur est survenue lors de la connexion du client.");
        //Timeout receive operations
        struct timeval tv;
        tv.tv_sec = 60; //timeout
        tv.tv_usec = 0;
        //setsockopt(csock, SOL_SOCKET, SO_RCVTIMEO, (char *)&tv, sizeof(struct timeval));

        cout << "Une nouvelle connexion sur la socket " << csock << "." << endl;
        return csock;
    }

    int serverClass::startService() throw(){

        cout << "Serveur initialisé ..." << endl;

        //initialisation de la session
        session.clear();

        pthread_t sessionCtrlParallel;
        tuple<serverClass*> data (this);
        pthread_create (&sessionCtrlParallel, NULL, &serverClass::Parallel_sessionController, &data); 

        while(1){ 
            SOCKET csock = initService(); 

            //nouveau thread;
            //thread sessionparallel(&serverClass::startSession,csock);
            pthread_t sessionparallel;
            tuple<serverClass*,int> data2 (this,csock);
            pthread_create (&sessionparallel, NULL, &serverClass::Parallel_startSession, &data2); 

            stopService();

            int stop = 0;
            /*cout << "arreter ? (1/0) " ;
            cin >> stop;*/

            if(stop==1)
                breakloop = true;

            if(breakloop)
                break;
        }
        pthread_cancel(sessionCtrlParallel);
        
        return 0;
    }

    void serverClass::stopService(){
        shutdown(sock, 2);
        close(sock);
    } 

    void serverClass::startSession(const SOCKET& cli_sock) throw(){
        
        asClient* client;
        try{
            //gestion du client actuel 
            client = new asClient(cli_sock); 
            session.addClient(client); 
            //identification
            session.routineIdentification(client);  
            //broadcast notification connexion
            char buf[client->c_pseudo().length()+20];
            sprintf(buf,"%s est connecté.",client->c_pseudo().c_str());
            session.broadCast(REQUETE(_post),NET_PARAM_NOTIF,string(buf));
            //update liste des connectés
            session.updateListe();
        }catch(char const* e){
            cout << e << endl;
            stopSession(*client); 
            return;
        } 

        //traitements concernant la session
        //std::string str = "coucou: coucou\n\n";
        //std::string str2 = "\n\n";
        int i(0);
        while(1){
            try{            

                asProto comm;
                comm.read(client->c_socket()); 
                cout << comm.requete() << endl;

                //si la soruce est différente du numéro enregistré pour la connexion active
                if(atoi(comm.src().c_str()) != client->c_number())
                    continue;

                if(comm.dest()==NET_SERVER_ADDR){
                    if(comm.methode()==REQUETE(_get)){
                        if (comm.param()==NET_PARAM_PSEUDO){
                            char buf[4] = {0};
                            sprintf(buf,"%d",client->c_number());
                            comm.build(string(buf),NET_SERVER_ADDR,REQUETE(_post),NET_PARAM_PSEUDO,client->c_pseudo());
                            comm.write(client->c_socket());
                        }else if(comm.param()==""){

                        }
                        else{
                            cout << "Pas compris la paramètre !!" << endl; 
                        }
                    }
                    else if(comm.methode()==REQUETE(_post)){
                        if (comm.param()==NET_SHUT_DOWN){
                            cout << "extinction" << endl;
                            pthread_mutex_lock (&mutex_sessionModified); // On verrouille le mutex 
                            pthread_cond_signal (&cond_sessionModified); // On délivre le signal : condition remplie 
                            pthread_mutex_unlock (&mutex_sessionModified);
                            break;
                        }else{
                            cout << "Pas compris la paramètre !!" << endl; 
                        } 
                    }
                    else if(comm.methode()==REQUETE(_sms)){
                        if (comm.param()==NET_SHUT_DOWN){
                             
                        }else{
                            cout << "Pas compris la paramètre !!" << endl; 
                        }
                    }
                    else if(comm.methode()==REQUETE(_update)){ 
                        if (comm.param()==NET_SHUT_DOWN){
                             
                        }else{
                            cout << "Pas compris la paramètre !!" << endl; 
                        }     
                    }
                    else if(comm.methode()==REQUETE(_delete)){
                        if (comm.param()==NET_SHUT_DOWN){
                             
                        }else{
                            cout << "Pas compris la paramètre !!" << endl; 
                        }     
                    }
                    else if(comm.methode()==REQUETE(_session)){
                        if (comm.param()==NET_SHUT_DOWN){
                             
                        }else{
                            cout << "Pas compris la paramètre !!" << endl; 
                        }     
                    }
                    else if(comm.methode()==REQUETE(_invite)){
                        if (comm.param()==NET_SHUT_DOWN){
                             
                        }else{
                            cout << "Pas compris la paramètre !!" << endl; 
                        }     
                    }
                    else if(comm.methode()==REQUETE(_file)){
                        if (comm.param()==NET_SHUT_DOWN){
                             
                        }else{
                            cout << "Pas compris la paramètre !!" << endl; 
                        }     
                    }
                    else{
                        cout << "Pas compris la méthode !!" << endl; 
                    }
                    
                }else if(comm.dest()==NET_BRCAST_ADDR){
                    //Transfert 
                    comm.write(client->c_socket());
                    session.broadCast(comm.methode(),comm.param(),comm.data());
                }
                else{
                    //Transfert
                    asClient* cli = session.getClient(atoi(comm.dest().c_str()));
                    comm.write(cli->c_socket());
                }

                
                /*/sleep(1);
                cout << "- " << i << endl;
                send(cli_sock, str.c_str(), str.length(), 0);
                send(cli_sock, str2.c_str(), str2.length(), 0);
                send(cli_sock, str.c_str(), str.length(), 0);
                send(cli_sock, str.c_str(), str.length(), 0);
                if(++i > 5){
                    pthread_mutex_lock (&mutex_sessionModified); // On verrouille le mutex 
                    pthread_cond_signal (&cond_sessionModified); // On délivre le signal : condition remplie 
                    pthread_mutex_unlock (&mutex_sessionModified); // On déverrouille le mutex 
                    break;
                }*/
            }catch(char const* e){
                cout << e << endl;
                break;
            }  
        }

        stopSession(*client);   
    }

    void* serverClass::Parallel_startSession(void* data) throw(){
        //rend le thread annulable à tout moment
        pthread_setcanceltype (PTHREAD_CANCEL_ASYNCHRONOUS, NULL);

        tuple<serverClass*,int>* donnee = (tuple<serverClass*,int> *)data;
        serverClass* serveur = get<0>(*donnee);
        int csock = get<1>(*donnee);

        serveur->startSession(csock);
 
        return NULL;
    }

    void serverClass::stopSession(const asClient &client){
        shutdown(client.c_socket(), 2);
        close(client.c_socket());
        session.removeClient(client);
    }

    void* serverClass::Parallel_sessionController(void* data) throw(){
        tuple<serverClass*>* donnee = (tuple<serverClass*> *)data;
        serverClass* serveur = get<0>(*donnee);

        while(1){
            //wait 
            pthread_mutex_lock(&mutex_sessionModified); // On verrouille le mutex 
            pthread_cond_wait (&cond_sessionModified, &mutex_sessionModified); // On attend que la condition soit remplie 
            serveur->session.updateListe();
            cout << "Mise à jour des listes ... "<< endl;
            pthread_mutex_unlock(&mutex_sessionModified); // On déverrouille le mutex */
        }
        return NULL;
    }

}
}
