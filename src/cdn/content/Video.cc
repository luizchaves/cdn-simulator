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
	this->_segmentVector.push_back(segment);
}

Segment *Video::getSegmentById(int id) {
	for(vector<Segment*>::iterator it = this->_segmentVector.begin(); it < this->_segmentVector.end(); it++){
		if((*it)->getId() == id)
			return (*it);
	}
	return NULL;
}

int Video::getSizeSegment(){
	return this->_segmentVector.size();
}

vector<Segment*> Video::getSegmentVector(){
	return this->_segmentVector;
}
