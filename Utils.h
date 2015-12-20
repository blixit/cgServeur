#ifndef CGSERVEUR_UTILS
#define CGSERVEUR_UTILS

#if defined(_WIN32) || defined(WIN32)
	typedef int socklen_t;
	#define closesocket(X)	close(X)
#endif

#include <vector>

namespace cgutils{ 
	
}


#endif // CGSERVEUR_UTILS
