`CgServeur` for C++
===================

Version 1.0.0 
[![Build Status](https://github.com/blixit/cgserveur.git?branch=master)](https://github.com/blixit/cgserveur)

# OVERVIEW #
------------

This repository stores the source code of cgServeur.
CgServeur is a simple c++ library implementing the protocol communication between a given client and a given server. You can use the given client and serveur to make your applications accessible on the network.

### Examples

> [`mainTicToc.cpp`](./mainTicToc.cpp) for a tictoc client on the terminal.

> [`mainChat.cpp`](./mainChat.cpp) for a chat client.

> [`mainClient.cpp`](./mainClient.cpp) for a simple client.

### Dependencies  

When you use CgServeur, you just need to link the Pthread librairy to suuport thread calls. Pthread is native on linux plateforms. 

### Building and Installing

A makefile is provided to pass the compilation. The compilation concerns both client and server applications.
The project has been compiled with g++>=4.8.4 with `std=c++11` option enabled. 

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