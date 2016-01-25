 

pthread_cond_t cond_position = PTHREAD_COND_INITIALIZER; /* Création de la condition */
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
bool received_cond = false;

string NET_UPGRID_xy = "xy";

unsigned char grid[9] = {0};
int joueur = -1;
int x=-1,y=-1;
unsigned char in = 0;
int initiated = 0;

string destinataire = "0";

bool empty(){
	int i(0);
	while(grid[i++]=='-');
	return i == 9 ? true : false;
}

bool full(){
	int i(0);
	while(grid[i++]!='-');
	return i == 9 ? true : false;
}

void show(){
	//system("clear");
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			cout << grid[i + 3*j] << " " ;
		}
		cout << endl;
	}
}

void boucle(asMClient *c){
	//
	received_cond = false;
	x=-1;
	y=-1;
	do{		

		if(initiated == 2){
			//wait response
			cout << "waiting ..." << endl;
			while(!received_cond){ 			} 
			received_cond = false;

			grid[y+3*x] = in;
		}else{
			initiated = 2;
		}
		

		show();
		x=-1;
		y=-1;

		while(x<0 || x>2 || y<0 || y>2){
			cout << "x : " ;
			cin >> x;
			cout << "y : " ;
			cin >> y;
			//
		}

		//c'est le joueur 1 qui commence
		stringstream ss;
		if(joueur == 0){
			grid[y+3*x] = 'x';
			ss << x << " " << y << " x";
		}else{
			grid[y+3*x] = 'o';
			ss << x << " " << y << " o";
		}	
		show();
		stringstream ssid;
		ssid << c->id();
		c->comm.build(destinataire, ssid.str(),REQUETE(_post),NET_UPGRID_xy,ss.str());
		c->comm.write(c->sock()); 
		
	}while(!full());
	
}