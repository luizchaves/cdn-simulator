#define LRUCACHE_CC
#include "LruCache.h"
#include <assert.h>
#include <algorithm>
//#include "AnakreonCommon.h"
#include "Exceptions.h"
#include <omnetpp.h>

LruCache::LruCache(int id, int cdnId, double totalCapacity, int shrink){
	assert(totalCapacity >= 0);
	this->_id = id;
	this->_cdnId = cdnId;
	this->_totalCapacity = totalCapacity;
	this->_availableSpace = totalCapacity;
	/*
	if(!strcmp(cacheContents, "NULL")){
		return;
	}
	char line[1024];
	int objectId = -1;
	int fileId = -1;

	///////////////////////////////////////////////////
	FILE* objectsReferenceFile = fopen(objectsReference,"r");
	if(!objectsReferenceFile){
		throw cRuntimeError("Unable to read file <%s> (line: %d, file: %s)", objectsReference, __LINE__, __FILE__);
	}
	double size = -1.0;
	while ( fgets( line, 1024, objectsReferenceFile )){
		if ( 3 != sscanf( line, "%d %lf %d", &objectId, &size, &fileId) ){
			throw cRuntimeError("Invlaid objects file format near line <%s> (line: %d, file: %s)", line, __LINE__, __FILE__);
		}

		if (objectId < 0 || size < 0.0 || fileId < 0){
			throw cRuntimeError("Invlaid objects file format near line <%s> (line: %d, file: %s)", line, __LINE__, __FILE__);
		}
		objectsReferenceMap.insert(make_pair(objectId, new Segment(size, objectId, fileId)));
	}
	fclose(objectsReferenceFile);
	///////////////////////////////////////////////////


	///////////////////////////////////////////////////
	FILE* cacheContentsFile = fopen(cacheContents,"r");
	if(!cacheContentsFile){
		throw cRuntimeError("Unable to read file <%s> (line: %d, file: %s)", cacheContents, __LINE__, __FILE__);
	}

	while ( fgets( line, 1024, cacheContentsFile )){

			if ( 1 == sscanf( line, "%d", &objectId ) ){
				map<int, Segment*> ::iterator mapPos = objectsReferenceMap.find(objectId);
				if(mapPos == objectsReferenceMap.end()){
					throw cRuntimeError("Unable to find object <%d> while loading objects (line: %d, file: %s)", objectId, __LINE__, __FILE__);
				}
				Segment* newObj = (*mapPos).second->clone();
				this->_cachedObjects.insert(make_pair(objectId, newObj));
				this->_availableSpace -= objectsReferenceMap[objectId]->getSize();
				this->_lruQueue.push_back(newObj);
			}
			else{
				throw cRuntimeError("Invlaid objects file format near line <%s> (line: %d, file: %s)", line, __LINE__, __FILE__);
			}
	}
	if(this->_availableSpace < 0){
		throw cRuntimeError("Cache content file objects exceed the capacity (line: %d, file: %s)", __LINE__, __FILE__);
	}
	fclose(cacheContentsFile);

	if (shrink == 1){
		this->_totalCapacity = this->_totalCapacity - this->_availableSpace;
		this->_availableSpace = 0;
	}
	//////////////////////////////////////////////////
	*/
}
void LruCache::_freeSpace(double size){
	assert(size > 0);
	if(size > this->_totalCapacity){
		throw cRuntimeError("size to free > total capacity (line: %d, file: %s)",  __LINE__, __FILE__);
	}
	while(size > 0){
		if ( this->_lruQueue.size() == 0){
			throw cRuntimeError("Empty lru while still removing objects (line: %d, file: %s)",  __LINE__, __FILE__);
		}

		vector<Segment*>::iterator cachePos;
		vector<Segment*>::iterator vecPos;

		for(vector<Segment*>::iterator it = this->_cacheSegments.begin(); it < this->_cacheSegments.end(); it++)
			if((*it)->getId() == this->_lruQueue.at(0)->getId())
				cachePos = it;
		vecPos = this->_lruQueue.begin();

		size -= this->_lruQueue.at(0)->getSize();
		_availableSpace += this->_lruQueue.at(0)->getSize();

		delete (*cachePos);
		this->_cacheSegments.erase(cachePos);
		this->_lruQueue.erase(vecPos);
	}
}
LruCache::~LruCache(){
	//std::for_each(this->_cacheSegments.rbegin(), this->_cacheSegments.rend(), delete_map<int, Segment*>());
	this->_cacheSegments.clear();
	this->_lruQueue.clear();
}
bool LruCache::objectExists(int id){
	assert(id>=0);
	for(vector<Segment*>::iterator it = this->_cacheSegments.begin(); it < this->_cacheSegments.end(); it++)
		if((*it)->getId() == id)
			return true;
	return false;
}

vector<Segment*> LruCache::getSegmentVector(){
	return this->_cacheSegments;
}

void LruCache::addSegment(vector<Segment*> segmentVector) {
	this->_cacheSegments.insert(this->_cacheSegments.end(), segmentVector.begin(), segmentVector.end());
}

Segment* LruCache::getNonExistingObject(Segment* object){
	assert(object);
	if(this->objectExists(object->getId())){
		delete object; object = NULL;
		throw new ObjectAlreadyExistsException();
	}
	if(object->getSize() > this->_totalCapacity){
		delete object; object = NULL;
		throw new ObjectTooBigException();
	}

	double lruCacheSize = 0;
	for(vector<Segment*>::iterator tempIterator = this->_lruQueue.begin(); tempIterator != this->_lruQueue.end(); tempIterator++ ) {
		lruCacheSize += (*tempIterator)->getSize();
	}
	if ( lruCacheSize < ( object->getSize() - this->_availableSpace) ){
		delete object; object = NULL;
		throw new ObjectTooBigException();
	}

	if(this->_availableSpace < object->getSize()){
		this->_freeSpace(object->getSize() - this->_availableSpace);
	}
	this->_cacheSegments.push_back(object);
	this->_lruQueue.push_back(object);
	this->_availableSpace -= object->getSize();
	return object;
}
Segment* LruCache::getExistingObject(int id){
	assert(id>=0);
	if(!this->objectExists(id)){
		throw new ObjectDoesNotExistException();
	}
	Segment* objectInMap = this->_cacheSegments[id];

		for(vector<Segment*>::iterator tempIterator = this->_lruQueue.begin(); tempIterator != this->_lruQueue.end(); tempIterator++ ) {
			if((*tempIterator)->getId() == id){
				Segment* object = *tempIterator; // get the object reference
				this->_lruQueue.erase(tempIterator);
				this->_lruQueue.push_back(object);
				return object;
			}
		}
	throw new ObjectDoesNotExistException();
}
Segment* LruCache::getObjectForced(int id){
	return this->objectsReferenceMap[id];
}

int LruCache::getId(){
	return this->_id;
}

int LruCache::getCDNId(){
	return this->_cdnId;
}

int LruCache::getNumberSegment(){
	return this->_cacheSegments.size();
}
