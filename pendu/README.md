`Hangman game` with SGMTP
=========================
To compile the applications : 
``` 
make ph   #compile host application
make pc   #compile client application
```

To make test, retrieve cgserveur.exe into the parent directory, put in the directory you want and launch it. I choice the same directory :
```
xterm -e ./cgserveur.exe &
```
Launch the applications. The order doesn't matter since host and client applications of hangman game are both client of cgserveur.exe.
Warning : launch on the 2 terminals.
```
#tty1
./ph  #launch hangman host
#tty2
./pc  #launch hangman client
```

For purpose of understanding, you have to enter yourself the elements of queries (method, parameter and data). See more in the [wiki](https://github.com/blixit/cgserveur/wiki). 
To start a session : 
```
Dest : ... #id of the host
Meth : S # big 'S' for session  (little 's' means sms)
Param : start
Value :   #nothing
```
The sever gives back remaining tries number and the hidden word (letters are replaced with '_').
To make a try with a letter : 
```
Dest : ... #id of the host
Meth : p #for post
Param : tl # tl = try a letter
Value :  o  #any letter you want
```
If your try is good, the server gives back the updated hidden word. The caracters you entered are revealed.
If not, the server gives back the remaining tries number and the unchanged word.

To make a try with a word : 
```
Dest : ... #id of the host
Meth : p #for post
Param : tw # tw = try a word
Value :  Coucouc ede de  #any word or sentence you want
```
If your try is good, you win.  
If not, the server gives back the remaining tries number and the unchanged word.

On the host side, these functionnalities are realized with 3 listeners : 
```
cgc.comm.binds.push_back({asBind::TYPE_M|asBind::TYPE_P, string(REQUETE(_session)+"-"+NET_PARAM_START), on_start, &cgc});
cgc.comm.binds.push_back({asBind::TYPE_M|asBind::TYPE_P, string(REQUETE(_post)+"-"+NET_PARAM_TRYW), on_tryw, &cgc}); 
cgc.comm.binds.push_back({asBind::TYPE_M|asBind::TYPE_P, string(REQUETE(_post)+"-"+NET_PARAM_TRYL), on_tryl, &cgc}); 
```

and 3 functions : 
```
void* on_start(void* args){... # handles session starting
void* on_tryw(void* args){...  # handles try with letter
void* on_tryl(void* args){...  # handles try with word or sentence
```

On the client side, we just need 4 listeners : 
```
cgc.comm.binds.push_back({asBind::TYPE_M|asBind::TYPE_P, string(REQUETE(_session)+"-"+NET_PARAM_END), on_end, &cgc});
cgc.comm.binds.push_back({asBind::TYPE_M|asBind::TYPE_P, string(REQUETE(_post)+"-"+NET_PARAM_NBTRIES), on_notif, &cgc}); 
cgc.comm.binds.push_back({asBind::TYPE_M|asBind::TYPE_P, string(REQUETE(_post)+"-"+NET_PARAM_WORDUPDATED), on_notif, &cgc}); 
cgc.comm.binds.push_back({asBind::TYPE_M|asBind::TYPE_P, string(REQUETE(_post)+"-"+NET_PARAM_NOTIFY), on_notif, &cgc});
```
And 2 functions : 
```
void* on_end(void* args){  # handles end of the game to destroy user session
void* on_notif(void* args){  #handles server notifications such as updates or tries number
```
In this little example, we can replace last 3 listeners by the 2 following : 
```
cgc.comm.binds.push_back({asBind::TYPE_M|asBind::TYPE_P, string(REQUETE(_session)+"-"+NET_PARAM_END), on_end, &cgc});
cgc.comm.binds.push_back({asBind::TYPE_M, string(REQUETE(_post)"), on_notif, &cgc});  
```
We can do it because there're just 3 queries.

