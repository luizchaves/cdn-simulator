#ifndef VIDEOSET_H_
#define VIDEOSET_H_

#include <map>
#include <vector>
#include <Video.h>

using namespace std;

class VideoSet {
public:
	VideoSet(int id, int cdn, const char *cdnName);
	virtual ~VideoSet();
	int getId();
	int getCDNId();
	const char *getCDNName();
	void addVideo(Video *video);
	void addVideo(map<int, Video*> videoMap);
	map<int, Video*> getVideoMap();
	Video *getVideo(int id);
	int getSizeVideo();
	int getNumberSegment();
	static const double SIZE_SEG;

private:
	int _id;
	int _cdnId;
	const char *_cdnName;
	map<int, Video*> _videoMap;
};

#endif /* VIDEOSET_H_ */
