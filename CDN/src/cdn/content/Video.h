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
	Segment *getSegment(int id);
	int getNumberSegment();

private:
	int _id;
	double _size;
	double _bitrate;
	map<int, Segment*> _segmentMap;
};

#endif /* VIDEO_H_ */
