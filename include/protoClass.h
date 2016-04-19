#ifndef PROTOCLASS_H
#define PROTOCLASS_H

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

#include "bindStruct.h"


using namespace std;

namespace cgServer{
namespace protoClass{ 
	const string TypeRequete[8] = {"g","p","s","u","d","S","i","f"};
	enum R{
        _get,
        _post,
        _sms,
        _update,
        _delete,
        _session,
        _invite,
        _file
    };
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
}

#endif //PROTOCLASS_H