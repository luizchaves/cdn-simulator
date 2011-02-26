#ifndef VIDEO_H_
#define VIDEO_H_

#include <map>
#include <Segment.h>

using namespace std;

class Video {
public:
	Video(int id, double size, double bitrate);
	virtual ~Video();
	double getSize();
	int getId();
	double getBitrate();
	void addSegment(Segment *segment);
	void eraseSegment(Segment *segment);
	int getSizeSegment();

private:
	int _id;
	double _size;
	double _bitrate;
	map<int, Segment*> segmentMap;
};

#endif /* VIDEO_H_ */
