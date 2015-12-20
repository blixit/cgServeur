#include <iostream>
#include <time.h> // rand

using namespace std;

#include "include/serverClass.h"

using namespace cgServer::serverClass;

int main()
{
	srand(time(NULL));
    asServer cgs;
    cgs.startService();
    return 0;
}
