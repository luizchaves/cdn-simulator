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

#ifndef __CDN_STORAGE_H_
#define __CDN_STORAGE_H_

#include <omnetpp.h>
#include <vector>
#include <VideoSet.h>
#include <Video.h>
#include <Segment.h>

/**
 * TODO - Generated class
 */
class Storage : public cSimpleModule
{
  public:
	vector<VideoSet*> getVideoSetVector();
    void showVideoSet();
  protected:
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);
  private:
    vector<VideoSet*> _videoSetVector;
};

#endif
