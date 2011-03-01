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

#include "Indexador.h"

Define_Module(Indexador);

void Indexador::initialize()
{
	for (cModule::SubmoduleIterator iter(getParentModule()->getParentModule()); !iter.end(); iter++){
		if (strcmp(iter()->getModuleType()->getName(), "CDNNode") == 0){
			if(!strcmp(((cModule*)iter())->par("type"), "s")){
				this->storage.push_back(((cModule*)iter()));
				vector<VideoSet*> videoSetVector = ((Storage*)((cModule*)iter())->getSubmodule("udpApp", 0))->getVideoSet();
				for (vector<VideoSet*>::iterator it = videoSetVector.begin(); it < videoSetVector.end(); it++ ) {
					if(cdnStorageContentMap[(*it)->getCDNId()] == NULL){
						this->cdnStorageContentMap.insert(make_pair((*it)->getCDNId(), (*it)));
					} else {
						this->cdnStorageContentMap[(*it)->getCDNId()]->addVideo((*it)->getVideoMap());
					}
				}
			} else
				if(!strcmp(((cModule*)iter())->par("type"), "r")){
					this->refletor.push_back(((cModule*)iter()));
					//cdnCacheContentMap
				} else
					if(!strcmp(((cModule*)iter())->par("type"), "c")){
						this->client.push_back(((cModule*)iter()));
					}
		}
	}
	//for ( map<int, VideoSet*>::iterator it=this->cdnStorageContentMap.begin() ; it != this->cdnStorageContentMap.end(); it++ )
	    //ev << "-----------------" << (*it).first << " => " << (*it).second << endl;

	//closestStorageToRefletor
	//closestRefletorToClient
}

void Indexador::handleMessage(cMessage *msg)
{
    // TODO - Generated method body
}

cModule *Indexador::getContentInStorage(Video *video) {
	return NULL;
}

cModule *Indexador::getContentInRefletor(Segment *segment) {
	return NULL;
}
