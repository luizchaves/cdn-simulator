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

#include "Indexer.h"

Define_Module(Indexer)
;

Indexer::Indexer(){
}

Indexer::~Indexer(){

}

void Indexer::initialize(int stage) {
	bindToPort(1000);
	scheduleAt(simTime(), new cMessage());
}

void Indexer::networkIndexer(cModule *mod)
{
	cTopology topo("top");
    topo.extractByProperty("node");
    for (cModule::SubmoduleIterator iter(mod); !iter.end(); iter++) {
			//CDNNode
			if (strcmp(iter()->getModuleType()->getName(), "CDNNode") == 0) {
				//Storage CDNNode
				if (!strcmp(iter()->par("type"), "s")) {
					//insert storage CDNNode in vector
					cModule *storageMod = iter();
					//std::cout << storageMod->getFullName() << endl;
					//std::cout << this->_storageVector.size() << endl;
					this->_storageVector.push_back(storageMod);
					//get vector videoset from storage
					Storage* storage;
					for (cModule::SubmoduleIterator iterCDNContent(storageMod); !iterCDNContent.end(); iterCDNContent++) {
						if (!strcmp(((cModule*) iterCDNContent())->getName(), "udpApp"))
							storage = (Storage*) iterCDNContent();
					}
					vector<VideoSet*> videoSetVector = storage->getVideoSetVector();
					//VideoSet Iterator
					for (vector<VideoSet*>::iterator it = videoSetVector.begin(); it < videoSetVector.end(); it++) {
						//Add videoSet in vector
						VideoSet* vs = (VideoSet*) (*it);
						if (this->_cdnStorageContentMap[vs->getCDNId()] == NULL) {
							this->_cdnStorageContentMap[vs->getCDNId()] = vs;
						} else {
							this->_cdnStorageContentMap[vs->getCDNId()]->addVideo(vs->getVideoVector());
						}
					}
				//Reflector CDNNode
				} else if (!strcmp(iter()->par("type"), "r")) {
					//std::cout << iter()->getFullName() << endl;
					//insert reflector CDNNode in vector
					cModule *refletorMod = iter();
					this->_refletorVector.push_back(refletorMod);
					//get vector cache from refletor
					Reflector* reflector;
					for (cModule::SubmoduleIterator iterCDNContent(refletorMod); !iterCDNContent.end(); iterCDNContent++) {
						if (!strcmp(((cModule*) iterCDNContent())->getName(), "udpApp"))
							reflector = (Reflector*) iterCDNContent();
					}
					vector<LruCache*> cacheVector = reflector->getCacheVector();
					//Cache Iterator
					for (vector<LruCache*>::iterator it = cacheVector.begin(); it < cacheVector.end(); it++) {
						//Add cache in vector
						LruCache* c = (*it);
						if (this->_cdnRefletorContentMap[c->getCDNId()] == NULL) {
							this->_cdnRefletorContentMap[c->getCDNId()] = c;
						} else {
							this->_cdnRefletorContentMap[c->getCDNId()]->addSegment(c->getSegmentVector());
						}
					}
				//Client CDNNode
				} else if (!strcmp(((cModule*) iter())->par("type"), "c")) {
					//insert client CDNNode in vector
					this->_clientVector.push_back(iter());
				}
			}
		}
    // Assign INDEX to ID
    int index = 0;
    for(vector<cModule*>::iterator tempIterator = this->_refletorVector.begin();tempIterator != this->_refletorVector.end();tempIterator++){
        this->_refletorIdToIndex.insert(make_pair((*tempIterator)->getId(), index));
        index++;
    }
    index = 0;
    for(vector<cModule*>::iterator tempIterator = this->_clientVector.begin();tempIterator != this->_clientVector.end();tempIterator++){
        this->_clientIdToIndex.insert(make_pair((*tempIterator)->getId(), index));
        index++;
    }
    index = 0;
    for(vector<cModule*>::iterator tempIterator = this->_storageVector.begin();tempIterator != this->_storageVector.end();tempIterator++){
        this->_storageIdToIndex.insert(make_pair((*tempIterator)->getId(), index));
        index++;
    }
    // Calculate Distance
    this->_refletorToRefletorDistance = new double*[this->_refletorVector.size()];
    for(int i = 0;i <= (int)(this->_refletorVector.size()) - 1;i++){
        this->_refletorToRefletorDistance[i] = new double[this->_refletorVector.size()];
    }
    for(vector<cModule*>::iterator iteratorFrom = this->_refletorVector.begin();iteratorFrom < this->_refletorVector.end();iteratorFrom++){
        for(vector<cModule*>::iterator iteratorTo = this->_refletorVector.begin();iteratorTo < this->_refletorVector.end();iteratorTo++){
            cTopology::Node *refletorNodeFrom = topo.getNodeFor((*iteratorFrom));
            cTopology::Node *refletorNodeTo = topo.getNodeFor((*iteratorTo));
            topo.calculateUnweightedSingleShortestPathsTo(refletorNodeTo);
            this->_refletorToRefletorDistance[this->_refletorIdToIndex[(*iteratorFrom)->getId()]]
                                              [this->_refletorIdToIndex[(*iteratorTo)->getId()]] = refletorNodeFrom->getDistanceToTarget();
        }
    }
    this->_storageToRefletorDistance = new double*[this->_storageVector.size()];
    for(int i = 0;i <= (int)(this->_storageVector.size()) - 1;i++){
        this->_storageToRefletorDistance[i] = new double[this->_refletorVector.size()];
    }
    for(vector<cModule*>::iterator iteratorFrom = this->_storageVector.begin();iteratorFrom < this->_storageVector.end();iteratorFrom++){
        for(vector<cModule*>::iterator iteratorTo = this->_refletorVector.begin();iteratorTo < this->_refletorVector.end();iteratorTo++){
            cTopology::Node *storageNodeFrom = topo.getNodeFor((*iteratorFrom));
            cTopology::Node *refletorNodeTo = topo.getNodeFor((*iteratorTo));
            topo.calculateUnweightedSingleShortestPathsTo(refletorNodeTo);
            this->_refletorToRefletorDistance[this->_refletorIdToIndex[(*iteratorFrom)->getId()]]
                                              [this->_refletorIdToIndex[(*iteratorTo)->getId()]] = storageNodeFrom->getDistanceToTarget();
        }
    }
    this->_refletorToClientDistance = new double*[this->_refletorVector.size()];
    for(int i = 0;i <= (int)(this->_refletorVector.size()) - 1;i++){
        this->_refletorToClientDistance[i] = new double[this->_clientVector.size()];
    }
    for(vector<cModule*>::iterator iteratorFrom = this->_refletorVector.begin();iteratorFrom < this->_refletorVector.end();iteratorFrom++){
        for(vector<cModule*>::iterator iteratorTo = this->_clientVector.begin();iteratorTo < this->_clientVector.end();iteratorTo++){
            cTopology::Node *refletorNodeFrom = topo.getNodeFor((*iteratorFrom));
            cTopology::Node *clientNodeTo = topo.getNodeFor((*iteratorTo));
            topo.calculateUnweightedSingleShortestPathsTo(clientNodeTo);
            this->_refletorToClientDistance[this->_refletorIdToIndex[(*iteratorFrom)->getId()]]
                                              [this->_clientIdToIndex[(*iteratorTo)->getId()]] = refletorNodeFrom->getDistanceToTarget();
        }
    }
    // Closest Node
    for (int k = 0; k <= (int) this->_refletorVector.size() - 1; k++) {
		double minDistance = INT_MAX;
		int selectedStorage = -1;
		for (int i = 0; i <= (int) this->_storageVector.size() - 1; i++) {
			if (this->_storageToRefletorDistance[i][k] <= minDistance) {
				minDistance = this->_storageToRefletorDistance[i][k];
				selectedStorage = i;
			}
		}
		this->_closestStorageToRefletor.insert(make_pair(k, selectedStorage));
	}
    for (int k = 0; k <= (int) this->_clientVector.size() - 1; k++) {
		double minDistance = INT_MAX;
		int selectedSurrogate = -1;
		for (int i = 0; i <= (int) this->_storageVector.size() - 1; i++) {
			if (this->_refletorToClientDistance[i][k] <= minDistance) {
				minDistance = this->_refletorToClientDistance[i][k];
				selectedSurrogate = i;
			}
		}
		this->_closestRefletorToClient.insert(make_pair(k, selectedSurrogate));
	}

}

void Indexer::handleMessage(cMessage *msg) {
	if (msg->isSelfMessage())
	{
		delete msg;
		std::cout << "Indexer Content" << endl;
		networkIndexer(getParentModule()->getParentModule());
	} else{
		if(msg->arrivedOn("udpIn")){
			RequestContentCDN *req = (RequestContentCDN *)msg->getContextPointer();

		}
		/*if (strcmp(((cModule*)msg->getContextPointer())->par("type").stringValue(),"c")==0){
				//TODO Procurar no refletor
				//TODO Procurar no storage
				//TODO Requisita para o cliente o segmento
		}*/
	}
}

cModule *Indexer::getContentInStorage(Segment *segment, int modID) {
	return NULL;
}

cModule *Indexer::getContentInRefletor(Segment *segment, int modID) {

	for (map<int, LruCache*>::iterator it=this->_cdnRefletorContentMap.begin() ; it != this->_cdnRefletorContentMap.end(); it++ ){
		if((*it).second->objectExists(segment->getId()))
			return NULL;
	}
	return NULL;
}

void Indexer::bindToPort(int port) {
	EV<< "Binding to UDP port " << port << endl;

	// TODO UDPAppBase should be ported to use UDPSocket sometime, but for now
	// we just manage the UDP socket by hand...
	cMessage *msg = new cMessage("UDP_C_BIND", UDP_C_BIND);
	UDPControlInfo *ctrl = new UDPControlInfo();
	ctrl->setSrcPort(port);
	ctrl->setSockId(UDPSocket::generateSocketId());
	msg->setControlInfo(ctrl);
	send(msg, "udpOut");

}

void Indexer::sendToUDP(cPacket *msg,
		int srcPort, const IPvXAddress& destAddr, int destPort) {
	// send message to UDP, with the appropriate control info attached
	msg->setKind(UDP_C_DATA);

	UDPControlInfo *ctrl = new UDPControlInfo();
	ctrl->setSrcPort(srcPort);
	ctrl->setDestAddr(destAddr);
	ctrl->setDestPort(destPort);
	msg->setControlInfo(ctrl);

	EV<< "Sending packet: ";
	//printPacket(msg);

	send(msg, "udpOut");
}
