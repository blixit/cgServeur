COMP = g++
CXX11 = -std=c++11
FLAGS = -Wall -g 

#serveur dependencies
SLIB = -pthread 
SSRC = main.cpp src/*.cpp
SOUT = cgserveur.exe 

#client dependencies
CLIB = -pthread 
CSRC = mainClient.cpp srcApp/*.cpp src/protoClass.cpp src/serveur_exception.cpp
COUT = cgclient.exe

all : client serveur
	@echo "Compilation finished ! "

client : ${CSRC}
	clear 
	@echo "Compiling client ..."
	${COMP} ${CXX11} ${FLAGS} ${CLIB} ${CSRC} -o ${COUT}
	#g++ -Wall -g -std=c++11 -pthread mainClient.cpp srcApp/*.cpp -o cg.exe

serveur : ${SSRC}
	@echo "Compiling serveur ..." 
	${COMP} ${CXX11} ${FLAGS} ${SLIB} ${SSRC} -o ${SOUT}

serveur2 : main.cpp src/protoClass.cpp src/clientClass.cpp src/serverClass.cpp src/serveur_exception.cpp src/sessionClass.cpp
	#clear
	#echo "------------------------------------------------------------------------------------"
	#g++ -Wall -g -std=c++11 -pthread main.cpp src/protoClass.cpp src/clientClass.cpp src/serverClass.cpp src/serveur_exception.cpp src/sessionClass.cpp -o cgserveur.exe 
