#ifndef VIDEO_H_
#define VIDEO_H_

#include <vector>
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
	Segment *getSegmentById(int id);
	int getSizeSegment();
	vector<Segment*> getSegmentVector();

private:
	int _id;
	double _size;
	double _bitrate;
	vector<Segment*> _segmentVector;
};

#endif /* VIDEO_H_ */
