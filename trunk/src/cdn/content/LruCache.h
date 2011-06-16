#ifndef LRUCACHE_H
#define LRUCACHE_H
#include "Cache.h"
#include <map>
#include <vector>
/* IMPORTANT !
Every inserted object is considered to be owned by the LruCache.
LruCache simply stores the objects' pointers or returns pointers to the stored objects.
When LruCache is destroyed it destroyes every object stored.
To avoid problems just make extra copies of the objects.
*/
using namespace std;
class LruCache : public Cache{
	private:
		int _id;
		int _cdnId;

		double _totalCapacity;
		double _availableSpace;

		vector<Segment*> _cacheSegments;
		map<int, Segment*> objectsReferenceMap;

		vector<Segment*> _lruQueue;//_lruQueue.begin() is the oldest object
		/*
		size>0
		removes the oldest objects to make availble space equal to size
		If size > _totalCapacity then an exception is thrown
		*/
		void _freeSpace(double size);

	public:
		/*
		totalCapacity>0
		creates a new LruCache of totalCapacity capacity in bytes and loads content from file according to the objectsReference in order to get the objects' size
		*/
		LruCache(int id, int cdnId, double totalCapacity, int shrink);
		//Every object in the _lruQueue is owned by the _lruQueue so it is deleted as well
		~LruCache();
		/*
		id>=0
		returns true if id in _lruQueue else false
		*/
		int getId();
		int getCDNId();
		int getNumberSegment();
		vector<Segment*> getSegmentVector();
		void addSegment(vector<Segment*> segmentVector);


		bool objectExists(int id);
		/*
		object != NULL
		Inserts a new object in _lruQueue. The object becomes newest in the _lruQueue. If the object exists then an exception is thrown. Returns a poiner to the cached object.
		If there is not enough space then the necessary space is freed
		If the object cannot fit in cache then an exception is thrown
		*/
		Segment* getNonExistingObject(Segment* object);
		/*
		id>=0
		Returns a poiner to the cached object. The requested object becomes newest in the _lruQueue
		If the object does not exist then an exception is thrown
		*/
		Segment* getExistingObject(int id);
		/*
		id>=0
		Returns a poiner to the object stored in the private objects reference.
		*/
		Segment* getObjectForced(int id);
};
#endif
