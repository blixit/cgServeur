#ifndef SERVEUR_EXCEPTION_H
#define SERVEUR_EXCEPTION_H

#include <exception>


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
    private:
        int _code;
        std::string erreur;
};

#endif // SERVEUR_EXCEPTION_H
