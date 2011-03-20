#include "Segment.h"

Segment::Segment(int id, double size, double start) {
	this->_id = id;
	this->_size = size;
	this->_start = start;
}

Segment::~Segment() {
}

double Segment::getStart() {
	return this->_start;
}

double Segment::getSize() {
	return this->_size;
}

int Segment::getId() {
	return this->_id;
}

Segment* Segment::clone(){
	Segment* cloned = this;

	return cloned;
}
