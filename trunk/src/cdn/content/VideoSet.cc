#include "VideoSet.h"

const double VideoSet::SIZE_SEG = 1000000.0;

VideoSet::VideoSet(int id, int cdnId, const char *cdnName) {
	this->_id = id;
	this->_cdnId = cdnId;
}

VideoSet::~VideoSet() {
}

int VideoSet::getId() {
	return this->_id;
}

int VideoSet::getCDNId() {
	return this->_cdnId;
}


void VideoSet::addVideo(Video *video) {
	this->_videoVector.push_back(video);
}

vector<Video*> VideoSet::getVideoVector(){
	return this->_videoVector;
}

void VideoSet::addVideo(vector<Video*> videoVector) {
	this->_videoVector.insert(this->_videoVector.end(), videoVector.begin(), videoVector.end());
}

Video *VideoSet::getVideoById(int id) {
	for(vector<Video*>::iterator it = this->_videoVector.begin(); it < this->_videoVector.end(); it++){
		if((*it)->getId() == id)
			return (*it);
	}
	return NULL;
}

int VideoSet::getSizeVideo() {
	return this->_videoVector.size();
}
