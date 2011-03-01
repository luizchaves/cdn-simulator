#include "Video.h"

Video::Video(int id, double size, double bitrate) {
	this->_id = id;
	this->_size = size;
	this->_bitrate = bitrate;
}

Video::~Video() {
}

double Video::getSize() {
	return this->_size;
}

int Video::getId() {
	return this->_id;
}

double Video::getBitrate() {
	return this->_bitrate;
}

void Video::addSegment(Segment *segment) {
	this->_segmentMap.insert(make_pair(segment->getId(), segment));
}

Segment *Video::getSegment(int id) {
	map<int, Segment*>::iterator segPos= this->_segmentMap.find(id);
	if(segPos == this->_segmentMap.end())
		return NULL;
	return (*segPos).second;
}

int Video::getNumberSegment(){
	return this->_segmentMap.size();
}
