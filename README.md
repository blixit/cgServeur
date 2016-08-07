`CgServeur` for C++
===================

Version 2.0.0 
[![Build Status](https://github.com/blixit/cgserveur.git?branch=master)](https://github.com/blixit/cgserveur)

# OVERVIEW #
------------

This repository stores the source code of cgServeur.
CgServeur is a simple c++ 'framework' implementing a communication protocol (SGMTP) between a given client and a given server through a third party server. You can use this protocol to help your applications to communicate easily on the network. No knowledge is required in network programming. Data transfert are voluntarily limited to 1024o for test. This value will certainly be modified in the next releases. Or it could be configurable.

SGMTP means Simple Gaming Message Tansfert Protocol. Its targets simple applications and developers which want to get rid of the network programming layer of their application.

### Examples

> [`hangman (v2)`](./pendu) for the hangman client and serveur on the terminal.

> [`mainTicToc.cpp (v1 being updated)`](./mainTicToc.cpp) for a tictoc client on the terminal.

> [`mainChat.cpp (v1 being updated)`](./mainChat.cpp) for a chat client.

> [`mainClient.cpp (v1 being updated)`](./mainClient.cpp) for a simple client.

### New Version

The version 2.0 brings the new features listed below : 

- classes and functions can be loaded thanks to the new static library implementing SGMTP protocol. You just need libsgmtp.a and sgmtp.h in your programs.
- ProtoClass::Bind() is now ProtoClass::BindMP() since it just tracks one type of query (Method + Parameter).  The new method ProtoClass::Bind() tracks all kind of messages thanks to the field 'type' added to the asBind structure. The type to track can be a Method, a Parameter, a Data or combinaison of them.

### Dependencies  

When you use CgServeur, you just need to link the Pthread librairy to suuport thread calls. Pthread is native on linux plateforms. Now with the version 2.0, you need to link libsgmtp.a to your project.

### Building and Installing

A makefile is provided to pass the compilation. The compilation concerns both client and server applications.
The project has been compiled with g++>=4.8.4 with `std=c++11` option enabled. 

To compile the library sgmtp : 
```
make cleanlibsta liba  #clean and compile the library
make serveur   #compile serveur application
```

If you need to install or upgrade gcc, follow these steps : 

```
sudo add-apt-repository ppa:ubuntu-toolchain-r/test
sudo apt-get update
sudo apt-get install gcc-4.9 g++-4.9
sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-4.9 60 --slave /usr/bin/g++ g++ /usr/bin/g++-4.9

```

#### Install from git repository

```bash
git clone https://github.com/blixit/cgserveur.git
cd cgserveur 
make
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
