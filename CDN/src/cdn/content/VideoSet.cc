#include "VideoSet.h"

VideoSet::VideoSet(int id, int cdnId, const var *cdnName) {
	this->_id = id;
	this->_cdn = cdn;
	this->_cdnNmae = cdnName;
}

VideoSet::~VideoSet() {
}

int VideoSet::getId() {
	return this->_id;
}

int VideoSet::getCDN() {
	return this->_cdn;
}

void VideoSet::addVideo(Video *video) {

}

void VideoSet::eraseVideo(Video *video) {

}

int VideoSet::getSizeVideo() {
	return 0;
}

int VideoSet::getSizeSegment() {
	return 0;
}
