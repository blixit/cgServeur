#ifndef USER_EVENT_CC
#define USER_EVENT_CC


void* on_bind(void* args){
	asMClient* c = (asMClient*)args;
	pthread_mutex_lock(&mutex);   
	c->comm.bind();
    pthread_mutex_unlock(&mutex);
	return NULL;
}

void* on_sms(void* args){
	asMClient* c = (asMClient*)args; 
	cout << "[" << c->comm.src() << "] : " << c->comm.data() << endl;
	c->sms(c->comm.src(),"ha ha ha");
	return NULL;
}

void* on_pseudo(void* args){
	asMClient* c = (asMClient*)args; 
	cout << "recu : " << c->comm.requete() << endl;
	return NULL;
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

#endif