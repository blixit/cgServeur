#ifndef BINDSTRUCT_H
#define BINDSTRUCT_H

typedef void*(*Fonction)(void*); //pointeur de fonctions

/**
	\struct _bindstruct
	Associe un mesage à une fonction.
	\warning Définie l'alias asBind.
*/
typedef struct _bindStruct
{ 
	_bindStruct(std::string _msg, Fonction ptr, void* args){
		msg = _msg;
		exec = ptr;
		data = args;
	}; 
	std::string msg;
	Fonction exec;
	void* data; //customer data
} asBind;

//Définie le type bindingList pour stocker plusieurs associations
typedef std::vector<asBind> BindingsList;

#endif //BINDSTRUCT_H