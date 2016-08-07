/* 
 * File:   sgmtp.hpp
 * Author: blixit
 *
 * Created on 4 août 2016, 21:19
 */

#ifndef SGMTP_HPP
#define SGMTP_HPP

#include <iostream>
#if defined(_WIN32) || defined(WIN32)
    #include <winsock2.h> //socket
    #include <unistd.h>
#else
    #include <arpa/inet.h>
    #include <unistd.h>
    #include <sys/socket.h> 
    #include <errno.h>

    #define BUFLEN 1024  //Max length of buffer 
    #define INVALID_SOCKET -1
    #define SOCKET_ERROR -1

    typedef int SOCKET;
    typedef struct sockaddr_in SOCKADDR_IN;
    typedef struct sockaddr SOCKADDR;
    typedef struct in_addr IN_ADDR;
#endif
#include <exception>
#include <vector>
#include "pthread.h"
    
using namespace std; 

#define READ_EXCEPTION 1
#define WRITE_EXCEPTION 2


class serveur_exception: public exception{
    public:
        /** Default constructor */
        serveur_exception(int cd,const char* str) throw(){
            erreur = string(str);
            _code = cd;
        };
        /*serveur_exception(const char* str) throw(){
            erreur = string(str);
            _code = 0;
        };*/
        serveur_exception(std::string str) throw(){
            erreur = str;
            _code = 0;
        };
        /** Default destructor */
        virtual ~serveur_exception() throw(){};

        virtual const char* what() const throw(){
            return erreur.c_str();
        };

        int code() const{return _code;};
    protected: 
        int _code;
        std::string erreur;
};

class write_exception : public serveur_exception{
     public:
        /** Default constructor */
        write_exception(int cd,const char* str) throw()  : serveur_exception(cd,str){
            erreur = string(str);
            _code = cd;
        };

        write_exception(std::string str) throw()  : serveur_exception(str){
            erreur = str;
            _code = WRITE_EXCEPTION; 
        };
        /** Default destructor */
        virtual ~write_exception() throw(){};
 

};


class read_exception : public serveur_exception{
     public:
        /** Default constructor */
        read_exception(int cd,const char* str) throw()  : serveur_exception(cd,str){
            erreur = string(str);
            _code = cd;
        };

        read_exception(std::string str) throw()  : serveur_exception(str){
            erreur = str;
            _code = READ_EXCEPTION; 
        };
        /** Default destructor */
        virtual ~read_exception() throw(){};
 

};

namespace cgServer{
    namespace protoClass{ 
        //######################################
        // BINDSTRUCT
        typedef void*(*Fonction)(void*); //pointeur de fonctions 
        typedef struct _bindStruct
        { 
                _bindStruct(char _type, std::string _msg, Fonction ptr, void* args){
                        type = _type;
                        msg = _msg;
                        exec = ptr;
                        data = args;
                }; 
                char type; //type of message to track : 1.methode, 2.parameter or 3.data
                std::string msg; //message to track
                Fonction exec; //Callback function
                void* data; //customer data
                static const char TYPE_ALL = 7; // m + p + d
                static const char TYPE_M = 4; // m 
                static const char TYPE_P = 2; // p
                static const char TYPE_D = 1; // d  
        } asBind;

        //Définie le type bindingList pour stocker les evenements de type méthode-parametre  (methode-param => fonction)
        typedef std::vector<asBind> BindingsList;

        //Définie le type BindingsKeywordsList pour stocker les mots clés à tracker via la méthode bindData (evenement de type données)
        //de protoClass
        typedef std::vector<asBind> BindingsKeywordsList;
        
        //######################################
        // PROTOCLASS
        
        const string TypeRequete[8] = {"g","p","s","u","d","S","i","f"};
	enum R{ _get, _post, _sms, _update, _delete, _session, _invite, _file  };
	#define REQUETE(id) TypeRequete[id]   
	//ORIGIN ou Adresses
	const string NET_SERVER_ADDR = "-1";
        const string NET_BRCAST_ADDR = "b";
	const string NET_ANY_ADDR = "a";

        //# parametres de requete réseau
        const string NET_SHUT_DOWN = "shut";
        /*' status de la requete 
        Public Const NET_REQ_OQP As String = "oqp"
        Public Const NET_REQ_END As String = "end"*/
        const string NET_REQ_OK = "ok";
        const string NET_REQ_FALL = "fall";

        //objet de la requete get ou post ou update
        const string NET_PARAM_NBCLIENT = "nbC";
        const string NET_PARAM_PSEUDO = "pseudo";
        const string NET_PARAM_RANK = "rank";
        const string NET_PARAM_NOTIF = "notif";
        const string NET_PARAM_LISTCLIENTS = "lc";
        /*Public Const NET_PARAM_FILE As String = "file"
        ' objet de la requete start
        Public Const NET_PARAM_SESSION_START As String = "sta"
        Public Const NET_PARAM_SESSION_END As String = "end"
        ' objet de la requete invite*/
        const string NET_PARAM_INV_SEND = "send";    
        const string NET_PARAM_INV_ACCEPT = "accept";// 'recoit une invitation
        const string NET_PARAM_INV_DECLINE = "decline";// 'recoit une invitation

        /*SMS*/
        const string NET_PARAM_SMS_SEND = "sms";

        /*' objet de la requete file
        Public Const NET_PARAM_FILE_SEND As String = "send"
        Public Const NET_PARAM_FILE_RECV As String = "recv"
        Public Const NET_PARAM_FILE_OK As String = NET_REQ_OK
        Public Const NET_PARAM_FILE_FALL As String = NET_REQ_FALL*/

        //EXCEPTIONS
        const int EXCEPT_EMPTY = 0;  
        const int EXCEPT_DEAD_SOCKET = 0;

        class protoClass{
                public : 
                        protoClass(bool const& isServeur);
                        ~protoClass();

                        void read(const int& sock, int const& length = BUFLEN) ;
                        void write(const int& sock) ;
            void precv(const int& sock, string src, int const& length = BUFLEN);
            void psend(const int& sock, string dest);

            bool isServer() const{return _isServer;}
            void isServer(bool const& val){ _isServer = val;};
                        string requete() const {return _requete;};
                        void requete(const string& val) { _requete = val;};
                        string dest() const {return _dest;};
                        void dest(const string& val) { _dest = val;};
                        string src() const {return _src;};
                        void src(const string& val) { _src = val;};
                        string methode() const {return _methode;};
                        void methode(const string& val) { _methode = val;};
                        string param() const {return _param;};
                        void param(const string& val) { _param = val;};
                        string data() const {return _data;};
                        void data(const string& val) { _data = val;};

                        void build(string const& dest, string const& src, string const& methode, string const& param, string const& data) ;
                        int split(vector<string>& vecteur, string chaine, char separateur);

            BindingsList binds; /**< Bindlist : pour associer des évènements à des fonctions */
            BindingsKeywordsList databinds; /**< BindingsKeywordsList : liste des mots à tracker via bindData */
            void* bind();
            void* bindMP();
            void* bindData();

            void sendCond(pthread_mutex_t m, pthread_cond_t c);
            void waitCond(pthread_mutex_t m, pthread_cond_t c);

                private :
            bool _isServer;
                        string _requete;
                        string _dest ;
                        string _src ;
                        string _methode ;
                        string _param ;
                        string _data ;

            void checkStatus(const int& socket);

        };

        typedef protoClass asProto;
    }
    
    namespace clientClass{ 
        class clientClass{
            public :
                clientClass(const int &sock=0);
                virtual ~clientClass();

                SOCKET c_socket() const { return _sock;};
                void c_socket(SOCKET val) { _sock = val;};
                SOCKET c_number() const { return _number;};
                void c_number(const size_t &val) { _number=val;};
                string c_pseudo() const {return _pseudo;};
                void c_pseudo(string val) {_pseudo = val;};
                bool routineIdentificationDone() const {return _routineIdentificationDone;};
                void routineIdentificationDone(const bool& val){_routineIdentificationDone = val;};
            private :
                SOCKET _sock;

                size_t _number; //!< position dans la liste des clients
                std::string _pseudo;
                int _rank;
                chrono::time_point<chrono::system_clock> _connexion_time = chrono::system_clock::now();

                bool _routineIdentificationDone = false;


        };
        typedef clientClass asClient;
    }
    
    using namespace protoClass;
    namespace moduleClientClass{
	class moduleClientClass{
		public :
			asProto comm;
			bool isRunning;
            std::thread::id listen_thread_id;
			std::thread::id write_thread_id;

			moduleClientClass();
			virtual ~moduleClientClass();

			int id() const{return _id;};
			void id(int val){ _id = val;};
			string pseudo() const{return _pseudo;};
			void pseudo(string val){ _pseudo = val;};
			SOCKET sock() const{return _sock;};
			void sock(SOCKET val){ _sock = val;};

			int sconnect(string const& ip, int const& port) ;
			void sdisconnect();
			void init(asProto & comm, string const& pseudo); 

			void tlisten();
			void twrite();
			void bind(bool val){_bind = val;};
			void binddata(bool val){_binddata = val;};
 
			void sendRequest(string const& dest, string const& meth, string const& param, string const& data);
			void get(string const& dest, string const& param);
			void post(string const& dest, string const& param, string const& value);
			void sms(string const& dest, string const& msg);
			void update(string const& dest, string const& param, string const& value); 
			void del(string const& dest, string const& param); 
			void session(string const& dest, string const& param, string const& value); 
			void invite(string const& dest, string const& param); 
			void file(string const& dest, string const& param, string const& value);  

		private :
			int _id = 0;
			string _pseudo = "";
			chrono::time_point<chrono::system_clock> _connexion_time = chrono::system_clock::now();
            SOCKET _sock;
            SOCKADDR_IN _sin;

            bool _bind = true;
            bool _binddata = false;



			void wsaInit();
			void wsaClean();
	};
	typedef moduleClientClass asMClient;
    }
    
    using namespace clientClass;
    namespace sessionClass{ 
        class sessionClass
        {
            public:
                /** Default constructor */
                sessionClass();
                /** Default destructor */
                virtual ~sessionClass();

                asClient* addClient(asClient* client);
                void removeClient(const asClient &client);
                asClient* getClient(const asClient& client);
                asClient* getClient(const int &numero);

                void clear();

                void routineIdentification(asClient* client) ;
                void broadCast(const string& meth, const string& para, const string& message) ;
                void updateListe() ;
            protected:
            private:
                vector <asClient*> listeClients;
        };
        typedef sessionClass asSession;
    }
    
    using namespace sessionClass;
    namespace serverClass{
        class serverClass
        {
            public:
                /** Default constructor */
                serverClass();
                /** Default destructor */
                virtual ~serverClass();

                /** \brief Initialise une socket, recoit une connexion et retourne le num�ro de la socket
                 * \return la socket client
                 * \throw En cas d'erreur l�ve une exception
                 */
                SOCKET initService() ;
                int startService() ;
                void stopService();

                void startSession(const SOCKET& cli_sock) ;
                static void* Parallel_startSession(void*) ;
                void stopSession(const asClient &client);
                static void* Parallel_sessionController(void* data) ;
            protected:
            private:
                bool breakloop;

                int port;
                SOCKET sock;
                SOCKADDR_IN sin;

                asSession session;

                void wsaInit();
                void wsaClean();
        };
        typedef serverClass asServer;

    }
}

#endif /* SGMTP_HPP */

