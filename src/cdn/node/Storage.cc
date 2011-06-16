//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#include "Storage.h"
#include <time.h>

Define_Module(Storage);

void Storage::showVideoSet()
{
	int i = 1;
	std::cout <<  "---------------------"<<endl;
    for(vector<VideoSet*>::iterator itVideoSet = this->_videoSetVector.begin();itVideoSet < this->_videoSetVector.end();itVideoSet++){
        //Add videoSet in vector
        VideoSet *videoSet = (*itVideoSet);
        vector<Video*> videoVector = videoSet->getVideoVector();
        std::cout <<  i << "-" << this->_videoSetVector.size() << " VSID " << videoSet->getId() << " CDNID " << videoSet->getCDNId() << " VIDEOS " << videoSet->getSizeVideo() << endl;
        i++;
        int j = 1;
        for(vector<Video*>::iterator itVideo = videoVector.begin();itVideo < videoVector.end();itVideo++){
        	Video *video = (*itVideo);
        	vector<Segment*> segmentVector = video->getSegmentVector();
        	std::cout << j << "-" << videoSet->getSizeVideo() << " VID " << video->getId() << " SEGMENTS " << video->getSizeSegment() << endl;
        	j++;
        	int l = 1;
        	for(vector<Segment*>::iterator itSegment = segmentVector.begin(); itSegment < segmentVector.end();itSegment++){
        		Segment *segment = (*itSegment);
        		if(segment == NULL)
        			continue;
        		std::cout << l << "-" << video->getSizeSegment() << " SID " << segment->getId() << endl;//" START " << s->getStart() << endl;
        		l++;
        	}
        }
    }
    std::cout <<  "---------------------"<<endl;
}

void Storage::initialize()
{

	//Não está funcionando
	srand(time(NULL));
	int numberVideo = uniform(1, 100, (int)(dblrand()) * 1000);
    //TODO criar varias videoset para varios cdn
	int id = rand();
	std::cout << "VSID " << id << " Number Video " << numberVideo << endl;
    VideoSet* videoSet = new VideoSet(id, 1, "ITVp");
    for (int i = 0; i < numberVideo; i++) {
		//TODO pegar bernauli e bitarte para saber o tamanho do video e depois o num de seg
		//TODO reduzir tamanho inicial para zero
    	int videoSize = uniform(1000000, 10000000, (int) dblrand()*1000);
		int numberSegment = ceil(videoSize / VideoSet::SIZE_SEG);
		id = rand();
		std::cout << "VID " << id << " Number Segment " << numberSegment << endl;
		Video* video = new Video(id, videoSize, 125);
			for (int j = 0; j < numberSegment; j++){
				id = rand();
				std::cout << "SID " << id << endl;
				Segment* segment = new Segment(id, VideoSet::SIZE_SEG, j*VideoSet::SIZE_SEG);
				//std::cout << "->" << video.getSizeSegment() << endl;
				video->addSegment(segment);
				//std::cout << "-->" << video.getSizeSegment() << endl;
			}
		//std::cout << "->" << videoSet.getSizeVideo() << endl;
		videoSet->addVideo(video);
		//std::cout << "-->" << videoSet.getSizeVideo() << endl;
	}
    this->_videoSetVector.push_back(videoSet);
    showVideoSet();
}

void Storage::handleMessage(cMessage *msg)
{
}

vector<VideoSet*> Storage::getVideoSetVector() {
	return this->_videoSetVector;
}
