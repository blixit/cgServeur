#include "../include/protoClass.h" 
#include "../include/serveur_exception.h" 

namespace cgServer{
namespace protoClass{
	protoClass::protoClass(bool const& isServeur){
		isServer(isServeur);
	}
	protoClass::~protoClass(){
		//
	}
	void protoClass::read(const int& socket, int const& length) {
		char buffer[length];
		requete(""); //permet de faire un test sur la variable read si la lecture echoue

		int sock_err = recv(socket,buffer,length,0); 
		if(sock_err==-1)
			throw read_exception("[protoClass::read] Erreur lors de la réception d'un message."); 
		if(sock_err==0)
			throw read_exception(EXCEPT_DEAD_SOCKET,"");

		requete(string(buffer));		

		vector<string> tab;
		int elemts = split(tab,requete(),'~');

		if(elemts != 5)
			throw read_exception("[protoClass::read] Mauvais nombre de paramètres. Message rejeté.\nRead Content:\n"+requete()); 
		dest(tab.at(0));
		src(tab.at(1));
		methode(tab.at(2));
		param(tab.at(3));
		data(tab.at(4)); 
	}

	void protoClass::write(const int& socket) { 
		int sock_err =send(socket,requete().c_str(),BUFLEN,0);
		if(sock_err==1)
			throw write_exception("[protoClass::write] Erreur lors de l'envoi du message.");  
	}
    
    /**
    	Vérifie que le serveur a bien transmis le paquet au destinataire. Pour cela, cette fonction qui fait un read
    	doit être placée juste après l'envoi send() de la fonction write. 
    	Cette fonction est privée (= interne à la classe protoClass)

    	\warning : En fait, dans le cas où le client lance 2 threads (écoute et écriture), si le thread écriture lit la
    	réponse du serveur avant checkStatus() qui se trouve dans le thread écriture, le client reste bloqué dans cet état.
    */
    void protoClass::checkStatus(const int& socket){ 
    	if(isServer()==false){
    		/* Si c'est le client, on ne fait la validation que si le destinataire du message envoyé n'est pas le serveur
    		*/ 
    		if(dest() != NET_SERVER_ADDR && dest()!=NET_BRCAST_ADDR){
    			read(socket);
    			if(param()!=NET_REQ_OK)
    				throw serveur_exception(param()+" : "+data());
    		}
    	} 
    }
 
	void protoClass::build(string const& desti, string const& srci, string const& methodei, string const& parami, string const& datai){
		if(desti.length()==0)
			throw serveur_exception("[protoClass::build] Le destinataire n'est pas fourni.");

		if(srci.length()==0) 
			throw serveur_exception("[protoClass::build] La source n'est pas fournie.");

		if(methodei.length()==0)
			throw serveur_exception("[protoClass::build] La méthode n'est pas fournie.");

		if(parami.length()==0)
			throw serveur_exception("[protoClass::build] Le paramètre n'est pas fourni.");

		dest(desti);
		src(srci);
		methode(methodei);
		param(parami);
		data(datai);
		requete(string(dest()+"~"+src()+"~"+methode()+"~"+param()+"~"+data())); 
	}

	int protoClass::split(vector<string>& vecteur, string chaine, char separateur)
	{
		vecteur.clear();

		string::size_type stTemp = chaine.find(separateur);
		
		while(stTemp != string::npos)
		{
			vecteur.push_back(chaine.substr(0, stTemp));
			chaine = chaine.substr(stTemp + 1);
			stTemp = chaine.find(separateur);
		}
		
		vecteur.push_back(chaine);

		return vecteur.size();
	}

	/**
		\brief Cette méthode exécute la fonction attachée à l'évènement methode-param
		enregistré dans la bindlist
	*/
	void* protoClass::bind(){
            for(auto b : binds){ 
                if(b.type == b.TYPE_ALL){  //0111
                    if(b.msg == methode()+'-'+param()+'-'+data()){ 
                        return b.exec(b.data);
                    }
                }else if(b.type == b.TYPE_M){  // 0100 
                     if(b.msg == methode()){ 
                        return b.exec(b.data);
                    }
                }else if(b.type == b.TYPE_P){  //0010
                    if(b.msg == param()){ 
                        return b.exec(b.data);
                    }
                }else if(b.type == b.TYPE_D){  //0001
                    if(b.msg == data()){
                        return b.exec(b.data);
                    }
                }else if(b.type == (b.TYPE_M|b.TYPE_P)){ ///0110 not 0001 
                    if(b.msg == methode()+'-'+param()){  
                        return b.exec(b.data);
                    }
                }else if(b.type == (b.TYPE_M|b.TYPE_D)){ ///0101 not 0010
                    if(b.msg == methode()+'-'+data()){ 
                        return b.exec(b.data);
                    } 
                }else if(b.type == (b.TYPE_P|b.TYPE_D)){                     
                     if(b.msg == methode()+'-'+param()+'-'+data()){ 
                        return b.exec(b.data);
                    }                                     
                }else{
                    throw serveur_exception("[protoClass::bind] Le type de données à tracker n'a pas été défini.");
                }
            }
            return NULL;
        }
        
        /**
		\brief Cette méthode exécute la fonction attachée à l'évènement methode-param
		enregistré dans la bindlist
	*/
	void* protoClass::bindMP(){
            for(auto b : binds){
                if(b.msg == methode()+'-'+param()){ 
                    return b.exec(b.data);
                }
            }
            return NULL;
        }


	/**
		\brief Cette méthode exécute la fonction attachée au message 
		enregistré dans la bindsKeywordsList
	*/
	void* protoClass::bindData(){
        for(auto b : databinds){
            if(b.msg == data())
                return b.exec(b.data);
        }
        return NULL;
    }

    void protoClass::sendCond(pthread_mutex_t m, pthread_cond_t c){
    	pthread_mutex_lock(&m);  
        pthread_cond_signal (&c); 
        pthread_mutex_unlock(&m);
    }

    void protoClass::waitCond(pthread_mutex_t m, pthread_cond_t c){
    	pthread_mutex_lock(&m);  
        pthread_cond_wait (&c, &m); 
        pthread_mutex_unlock(&m);
    }
    
		
}
}