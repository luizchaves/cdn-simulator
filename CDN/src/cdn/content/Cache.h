#ifndef CACHE_H
#define CACHE_H
#include "Video.h"

class Cache {
	public:
		Cache();
		virtual bool objectExists(int id) = 0;
		virtual ~Cache();
		virtual Video* getNonExistingObject(Video* object) = 0;
		virtual Video* getExistingObject(int id) = 0;
		virtual Video* getObjectForced(int id) = 0;
};
#endif
