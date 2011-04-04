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
	this->_videoMap.insert(make_pair(video->getId(), video));
}

map<int, Video*> VideoSet::getVideoMap(){
	return this->_videoMap;
}

void VideoSet::addVideo(map<int, Video*> videoMap) {
	this->_videoMap.insert(videoMap.begin(), videoMap.end());
}

Video *VideoSet::getVideo(int id) {
	map<int, Video*>::iterator vidPos= this->_videoMap.find(id);
	if(vidPos == this->_videoMap.end())
		return NULL;
	else
		return (*vidPos).second;
}

int VideoSet::getSizeVideo() {
	return this->_videoMap.size();
}

int VideoSet::getNumberSegment() {
	int numberSegment = 0;
	for (map<int, Video*>::iterator it = this->_videoMap.begin() ; it != this->_videoMap.end(); it++ ){
		Video *video = (*it).second;
		numberSegment += video->getNumberSegment();
	}
	return numberSegment;
}
