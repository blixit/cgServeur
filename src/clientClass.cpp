#include "../include/clientClass.h"

namespace cgServer{
namespace clientClass{

    clientClass::clientClass(const int &sock){
        _sock = sock;
        _pseudo = "nacder";
        _rank = 0;
        _connexion_time = chrono::system_clock::now();
    }
    clientClass::~clientClass(){

    }
}
}
