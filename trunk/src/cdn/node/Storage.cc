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

void Storage::initialize()
{
	int numberVideo = uniform(0, 100, (int) dblrand()*1000);
	//TODO criar várias videoset
	VideoSet videoSet(time(NULL), 1, "ITVp");
	for (int i = 0; i < numberVideo; i++) {
		//TODO pegar bernauli e bitarte para saber o tamanho do video e depois o num de seg
		int numberSegment = uniform(0, 100, (int) dblrand()*1000);
		Video video(time(NULL), uniform(0, 100, (int) dblrand()*1000), 125);
			for (int j = 0; j < numberSegment; j++){
				Segment segment(time(NULL), VideoSet::SIZE_SEG, j*VideoSet::SIZE_SEG);
				video.addSegment(&segment);
			}
		videoSet.addVideo(&video);
	}
	this->_videoSet.push_back(&videoSet);
}

void Storage::handleMessage(cMessage *msg)
{
}

vector<VideoSet*> Storage::getVideoSet() {
	return this->_videoSet;
}
