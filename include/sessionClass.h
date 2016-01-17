#ifndef SESSIONCLASS_H
#define SESSIONCLASS_H
    
#include <vector>

using namespace std;

#include "clientClass.h"
using namespace cgServer::clientClass;

namespace cgServer{
namespace sessionClass{
    /** \brief  Gestion des sessions clients
     */
    class sessionClass
    {
        public:
            /** Default constructor */
            sessionClass();
            /** Default destructor */
            virtual ~sessionClass();

            asClient* addClient(asClient* client);
            void removeClient(const asClient &client);
            asClient* getClient(const asClient& client);
            asClient* getClient(const int &numero);

            void clear();

            void routineIdentification(asClient* client) ;
            void broadCast(const string& meth, const string& para, const string& message) ;
            void updateListe() ;
        protected:
        private:
            vector <asClient*> listeClients;
    };
    typedef sessionClass asSession;
}
}

#endif // SESSIONCLASS_H
