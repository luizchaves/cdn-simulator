#ifndef VIDEOSET_H_
#define VIDEOSET_H_

#include <map>
#include <Video.h>

using namespace std;

class VideoSet {
public:
	VideoSet(int id, int cdn);
	virtual ~VideoSet();
	int getId();
	int getCDN();
	void addVideo(Video *video);
	void eraseVideo(Video *video);
	int getSizeVideo();
	int getSizeSegment();

private:
	int _id;
	int _cdnId;
	const var *cdnName;
	map<int, Video*> videoMap;
};

#endif /* VIDEOSET_H_ */
