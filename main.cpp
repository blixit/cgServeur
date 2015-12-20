#include <iostream>

using namespace std;

#include "include/serverClass.h"

using namespace cgServer::serverClass;

int main()
{
    asServer cgs;
    cgs.startService();
    return 0;
}
