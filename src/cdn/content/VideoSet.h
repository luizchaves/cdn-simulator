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
	void addVideo(vector<Video*> videoMap);
	vector<Video*> getVideoVector();
	Video *getVideoById(int id);
	int getSizeVideo();
	static const double SIZE_SEG;

private:
	int _id;
	int _cdnId;
	vector<Video*> _videoVector;
};

#endif /* VIDEOSET_H_ */
