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

}

void Video::eraseSegment(Segment *segment) {

}

int Video::getSizeSegment(){
	return 0;
}
