#ifndef BINDSTRUCT_H
#define BINDSTRUCT_H

namespace cgServer{
namespace protoClass{ 
typedef void*(*Fonction)(void*); //pointeur de fonctions


/**
	@struct _bindstruct
	Associe un mesage à une fonction.
	@warning Définie l'alias asBind.
*/
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
}
}
#endif //BINDSTRUCT_H