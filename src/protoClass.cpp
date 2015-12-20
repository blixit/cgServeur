#include "../include/protoClass.h" 

namespace cgServer{
namespace protoClass{
	protoClass::protoClass(){
		//
	}
	protoClass::~protoClass(){
		//
	}
	void protoClass::read(const int& socket) throw(){
		char buffer[BUFLEN];
		int sock_err = recv(socket,buffer,BUFLEN,0); 
		if(sock_err==1)
			throw "[protoClass::read] Erreur lors de la réception d'un message."; 
		requete(string(buffer));		
		vector<string> tab;
		int elemts = split(tab,requete(),'~');

		if(elemts != 5)
			throw "[protoClass::read] Mauvais nombre de paramètres. Message rejeté."; 
		dest(tab.at(0));
		src(tab.at(1));
		methode(tab.at(2));
		param(tab.at(3));
		data(tab.at(4)); 
	}

	void protoClass::write(const int& socket) throw(){
		int sock_err =send(socket,requete().c_str(),requete().length(),0);
		if(sock_err==1)
			throw "[protoClass::write] Erreur lors de l'envoie du message."; 
	}
 
	void protoClass::build(const string& desti,const string& srci, const string& methodei, const string& parami, const string& datai) throw(){
		if(desti.length()==0 || srci.length()==0 || methodei.length()==0 || parami.length()==0 || datai.length()==0)
			throw "[protoClass::build]Un des paramètres est nul.";

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
		
}
}