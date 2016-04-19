COMP = g++
CXX11 = -std=c++11
FLAGS = -Wall -g 

#serveur dependencies
SLIB = -pthread 
SSRC = main.cpp src/*.cpp
SOUT = cgserveur.exe 

#client dependencies
CLIB = -pthread 
CSRC = mainClient.cpp src/mcClientClass.cpp src/protoClass.cpp src/serveur_exception.cpp
COUT = cgclient.exe

ChatSRC = mainChat.cpp src/mcClientClass.cpp src/protoClass.cpp src/serveur_exception.cpp
ChatOUT = cgchat.exe

TTSRC = mainTicToc.cpp src/mcClientClass.cpp src/protoClass.cpp src/serveur_exception.cpp
TTOUT = cgtictoc.exe

all : tt chat client serveur
	@echo "Compilation finished ! "

client : ${CSRC}
	clear 
	@echo "Compiling client ..."
	${COMP} ${CXX11} ${FLAGS} ${CLIB} ${CSRC} -o ${COUT}
	#g++ -Wall -g -std=c++11 -pthread mainClient.cpp srcApp/*.cpp -o cg.exe

chat : ${ChatSRC}
	clear 
	@echo "Compiling chat ..."
	${COMP} ${CXX11} ${FLAGS} ${CLIB} ${ChatSRC} -o ${ChatOUT}

tt : ${TTSRC}
	clear 
	@echo "Compiling tictoc ..."
	${COMP} ${CXX11} ${FLAGS} ${CLIB} ${TTSRC} -o ${TTOUT}

serveur : ${SSRC}
	clear
	@echo "Compiling serveur ..." 
	${COMP} ${CXX11} ${FLAGS} ${SLIB} ${SSRC} -o ${SOUT}

serveur2 : main.cpp src/protoClass.cpp src/clientClass.cpp src/serverClass.cpp src/serveur_exception.cpp src/sessionClass.cpp
	#clear
	#echo "------------------------------------------------------------------------------------"
	#g++ -Wall -g -std=c++11 -pthread main.cpp src/protoClass.cpp src/clientClass.cpp src/serverClass.cpp src/serveur_exception.cpp src/sessionClass.cpp -o cgserveur.exe 

clean :
	rm -f ${COUT}
	rm -f ${SOUT}
	rm -f ${TTOUT}
	rm -f ${ChatOUT}
