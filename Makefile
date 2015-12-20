client : mainClient.cpp srcApp/*.cpp
	clear
	echo "------------------------------------------------------------------------------------"
	g++ -Wall -g -std=c++11 -pthread mainClient.cpp srcApp/*.cpp -o cg.exe

serveur : main.cpp src/*.cpp
	clear
	echo "------------------------------------------------------------------------------------"
	g++ -Wall -g -std=c++11 -pthread main.cpp src/*.cpp -o cgserveur.exe 

serveur2 : main.cpp src/protoClass.cpp src/clientClass.cpp src/serverClass.cpp src/serveur_exception.cpp src/sessionClass.cpp
	clear
	echo "------------------------------------------------------------------------------------"
	g++ -Wall -g -std=c++11 -pthread main.cpp src/protoClass.cpp src/clientClass.cpp src/serverClass.cpp src/serveur_exception.cpp src/sessionClass.cpp -o cgserveur.exe 
