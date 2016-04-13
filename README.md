`CgServeur` for C++
===================

Version 1.0.0 
[![Build Status](https://github.com/blixit/cgserveur.git?branch=master)](https://github.com/blixit/cgserveur)

# OVERVIEW #
------------

This repository stores the source code of cgServeur.
CgServeur is a simple c++ library implementing the protocol communication between a given client and a given server. You can use the given client and serveur to make your applications accessible on the network.

EXAMPLE 1 : TIC-TOC 
-------
```
#include <iostream>
#include <thread>
#include <sstream>

using namespace std;

#include "includeApp/mcClientClass.h"
#include "include/protoClass.h"
#include "include/serveur_exception.h"

using namespace cgApplication::moduleClientClass;
using namespace cgServer::protoClass;

#include "srcGame/tt.cc"

std::thread::id listen_thread_id;
std::thread::id write_thread_id;

void* on_bind(void* args){
	asMClient* c = (asMClient*)args;
	pthread_mutex_lock(&mutex);   
	c->comm.bind();
    pthread_mutex_unlock(&mutex);
	return NULL;
}

void tlisten(asMClient* cgc){
	listen_thread_id = std::this_thread::get_id();
	while(cgc->isRunning){
		try{
			cgc->comm.read(cgc->sock());
			//cout << cgc->comm.requete() << endl;
			cgc->comm.bind(); 
			//thread threadaction(on_bind,cgc);
			//threadaction.detach(); 
		}catch(read_exception e){
			if(cgc->comm.requete().length()==0)
				cgc->isRunning = false;
			cout << e.what() << endl;
		}catch(exception e){
			cout << "ERREUR" << endl;
			cout << e.what() << endl;
		}
	}
}

void twrite(asMClient* cgc){
	write_thread_id = std::this_thread::get_id();
	string waiter;
	cout << "waiter ? " ;
	cin >> waiter;
	if(waiter!="0")
		return;

	while(cgc->isRunning){
		try{
			string meth, param, dest;
			cout << "dest : " << endl;
	    	cin >> dest;

			cout << "meth : " << endl;
			cin >> meth;

			cout << "param : " << endl;
	    	cin >> param;
 			 
 			stringstream stream;
 			stream << cgc->id();
    		cgc->comm.build(dest,stream.str(),meth,param,cgc->pseudo());  //sending p-test
		    //cout << "send test " << comm.requete() << endl;
		
			cgc->comm.write(cgc->sock());
			break;
		}catch(write_exception e){
			cout << e.what() << endl;
		}catch(serveur_exception e){
			cout << e.what() << endl;
		}
	}
}

void* on_invite(void* args){
	asMClient* c = (asMClient*)args;

	cout << "invited by " << c->comm.src() << endl;
	joueur = 0;
	destinataire = c->comm.src();
	stringstream ss;
	ss << c->id();

	c->comm.build(destinataire, ss.str(),REQUETE(_invite),NET_PARAM_INV_ACCEPT,"");
	c->comm.write(c->sock()); 

	initiated = 1;
	std::fill(grid,grid+9,'-');	
	//boucle(c);
	thread(boucle,c).detach();

	return NULL;
}

void* on_accept(void* args){
	asMClient* c = (asMClient*)args;

	cout << "accepted by " << c->comm.src() << endl;
	joueur = 1;
	destinataire = c->comm.src();

	initiated = 2;
	std::fill(grid,grid+9,'-');	

	//boucle(c);
	thread(boucle,c).detach();

	return NULL;
}

void* on_position(void* args){
	asMClient* c = (asMClient*)args;

	try{
		std::vector<string> pos;
		c->comm.split(pos,c->comm.data(),' ');
		x = atoi(pos[0].c_str());
		y = atoi(pos[1].c_str());
		in = pos[2][0];

				pthread_mutex_lock(&mutex); 
				received_cond = true;
				pthread_mutex_unlock(&mutex); 
		//c->comm.sendCond(mutex,cond_position);
		cout << "on position send cond" << endl;
	}catch(exception e){
		cout << e.what() << endl;
	}
	return NULL;
}
/**
Le segmenfault qu"on sort de la boucle vient du fait qu'on passe des pointeurs (locaux) au
thread boucle. Il faudrait déclarer cgc en globale ou envisager d'utiliser des 
shared_ptr
http://stackoverflow.com/questions/14148412/c11-stdthreaddetach-and-access-to-shared-data
*/
int main(){ 
    asMClient cgc; 

    //launching serveur
    system("xterm ./cgserveur.exe &");
    sleep(1);

    //cgc.comm.databinds.push_back({"invite",on_invite,&cgc});
    cgc.comm.binds.push_back({string(REQUETE(_invite)+"-"+NET_PARAM_INV_SEND), on_invite, &cgc});
    cgc.comm.binds.push_back({string(REQUETE(_invite)+"-"+NET_PARAM_INV_ACCEPT), on_accept, &cgc});
    cgc.comm.binds.push_back({string(REQUETE(_post)+"-"+NET_UPGRID_xy), on_position, &cgc});
    
    try{ 
    	cgc.sconnect("127.0.0.1",1607);
	    cgc.init(cgc.comm,"alain"); 
 
	    thread threadlisten(tlisten, &cgc);
	    thread threadwrite(twrite, &cgc);
 
	    threadwrite.join();
	    threadlisten.join();
	    cgc.sdisconnect();
    }catch(serveur_exception e){
    	cout << e.what() << endl; 
    }

	return 0;
}
```

Others examples :

> [`mainChat.cpp`](./mainChat.cpp) for a chat client.
> [`mainClient.cpp`](./mainClient.cpp) for a simple client.

### Dependencies  

When you use CgServeur, you just need to link the Pthread librairy to suuport thread calls. Pthread is native on linux plateforms. 

### Building and Installing

A makefile is provided to pass the compilation. The compilation concerns both client and server applications.
The compilator used is g++>=4.8.4 with `std=c++11` option enabled. 

#### Install from git repository

```bash
$ git clone https://github.com/blixit/cgserveur.git
$ cd cgserveur 
$ make
```

### Documentation

You can get addtional information on the
[wiki](https://github.com/blixit/cgserveur/wiki) 
 
Contributing
------------
`cgserveur` is developed on GitHub at [cgServeur](https://github.com/blixit/cgserveur) and on bitbucket.org too.

License
-------
### Who do I talk to? ###

* Repo owner or admin : Blixit
