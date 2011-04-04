#ifndef CACHE_H
#define CACHE_H
#include "Video.h"

class Cache {
	public:
		Cache();
		virtual int getId() = 0;
		virtual int getCDNId() = 0;
		virtual int getNumberSegment() = 0;
		virtual map<int, Segment*> getSegmentMap() = 0;
		virtual void addSegment(map<int, Segment*> segmentMap) = 0;
		virtual bool objectExists(int id) = 0;
		virtual ~Cache();
		virtual Segment* getNonExistingObject(Segment* object) = 0;
		virtual Segment* getExistingObject(int id) = 0;
		virtual Segment* getObjectForced(int id) = 0;
};
#endif
