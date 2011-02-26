#ifndef SEGMENT_H_
#define SEGMENT_H_

class Segment {
public:
	Segment(int id, double size, double start);
	virtual ~Segment();
	double getStart();
	double getSize();
	int getId();
private:
	int _id;
	double _size;
	double _start;
};

#endif /* SEGMENT_H_ */
