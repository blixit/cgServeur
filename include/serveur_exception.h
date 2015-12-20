#ifndef SERVEUR_EXCEPTION_H
#define SERVEUR_EXCEPTION_H

#include <exception>


class serveur_exception: public exception{
    public:
        /** Default constructor */
        serveur_exception(int cd,const char* str) throw(){
            erreur = string(str);
            code = cd;
        };
        serveur_exception(const char* str) throw(){
            erreur = string(str);
            code = 0;
        };
        serveur_exception(std::string str) throw(){
            erreur = str;
            code = 0;
        };
        /** Default destructor */
        virtual ~serveur_exception() throw(){};

        virtual const char* what() const throw(){
            return erreur.c_str();
        };
    protected:
    private:
        int code;
        std::string erreur;
};

#endif // SERVEUR_EXCEPTION_H
