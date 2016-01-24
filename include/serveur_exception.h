#ifndef SERVEUR_EXCEPTION_H
#define SERVEUR_EXCEPTION_H

#include <exception>

#define READ_EXCEPTION 1
#define WRITE_EXCEPTION 2


class serveur_exception: public exception{
    public:
        /** Default constructor */
        serveur_exception(int cd,const char* str) throw(){
            erreur = string(str);
            _code = cd;
        };
        /*serveur_exception(const char* str) throw(){
            erreur = string(str);
            _code = 0;
        };*/
        serveur_exception(std::string str) throw(){
            erreur = str;
            _code = 0;
        };
        /** Default destructor */
        virtual ~serveur_exception() throw(){};

        virtual const char* what() const throw(){
            return erreur.c_str();
        };

        int code() const{return _code;};
    protected: 
        int _code;
        std::string erreur;
};

class write_exception : public serveur_exception{
     public:
        /** Default constructor */
        write_exception(int cd,const char* str) throw()  : serveur_exception(cd,str){
            erreur = string(str);
            _code = cd;
        };

        write_exception(std::string str) throw()  : serveur_exception(str){
            erreur = str;
            _code = WRITE_EXCEPTION; 
        };
        /** Default destructor */
        virtual ~write_exception() throw(){};
 

};


class read_exception : public serveur_exception{
     public:
        /** Default constructor */
        read_exception(int cd,const char* str) throw()  : serveur_exception(cd,str){
            erreur = string(str);
            _code = cd;
        };

        read_exception(std::string str) throw()  : serveur_exception(str){
            erreur = str;
            _code = READ_EXCEPTION; 
        };
        /** Default destructor */
        virtual ~read_exception() throw(){};
 

};

#endif // SERVEUR_EXCEPTION_H
