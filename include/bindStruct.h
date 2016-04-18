#ifndef BINDSTRUCT_H
#define BINDSTRUCT_H

typedef void*(*Fonction)(void*); //pointeur de fonctions

/**
	@struct _bindstruct
	Associe un mesage à une fonction.
	@warning Définie l'alias asBind.
*/
typedef struct _bindStruct
{ 
	_bindStruct(std::string _msg, Fonction ptr, void* args){
		msg = _msg;
		exec = ptr;
		data = args;
	}; 
	std::string msg; //message to track
	Fonction exec; //Callback function
	void* data; //customer data
} asBind;

//Définie le type bindingList pour stocker plusieurs associations (methode-param => fonction)
typedef std::vector<asBind> BindingsList;

//Définie le type BindingsKeywordsList pour stocker les mots clés à tracker via la méthode bindData
//de protoClass
typedef std::vector<asBind> BindingsKeywordsList;

#endif //BINDSTRUCT_H