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

Define_Module(Indexador)
;

void Indexador::initialize(int stage) {
	if (stage == 99) {
		for (cModule::SubmoduleIterator iter(
				getParentModule()->getParentModule()); !iter.end(); iter++) {
			if (strcmp(iter()->getModuleType()->getName(), "CDNNode") == 0) {
				if (!strcmp(((cModule*) iter())->par("type"), "s")) {
					this->storage.push_back((cModule*) iter());
					vector<VideoSet*> videoSetVector =
							((Storage*) ((cModule*) iter())->getSubmodule(
									"udpApp", 0))->getVideoSet();
					for (vector<VideoSet*>::iterator it =
							videoSetVector.begin(); it < videoSetVector.end(); it++) {
						if (cdnStorageContentMap[(*it)->getCDNId()] == NULL) {
							this->cdnStorageContentMap.insert(make_pair(
									(*it)->getCDNId(), (*it)));
						} else {
							this->cdnStorageContentMap[(*it)->getCDNId()]->addVideo(
									(*it)->getVideoMap());
						}
					}
				} else if (!strcmp(((cModule*) iter())->par("type"), "r")) {
					this->refletor.push_back(((cModule*) iter()));
					//cdnCacheContentMap
					vector<Cache*> cacheVector =
							((Refletor*) ((cModule*) iter())->getSubmodule(
									"udpApp", 0))->getCacheVector();
					for (vector<Cache*>::iterator it =
							cacheVector.begin(); it < cacheVector.end(); it++) {
						if (cdnRefletorContentMap[(*it)->getCDNId()] == NULL) {
							this->cdnRefletorContentMap.insert(make_pair(
									(*it)->getCDNId(), (*it)));
						} else {
							this->cdnRefletorContentMap[(*it)->getCDNId()]->addSegment(
									(*it)->getSegmentMap());
						}
					}
				} else if (!strcmp(((cModule*) iter())->par("type"), "c")) {
					this->client.push_back(((cModule*) iter()));
				}
			}
		}
		//for ( map<int, VideoSet*>::iterator it=this->cdnStorageContentMap.begin() ; it != this->cdnStorageContentMap.end(); it++ )
		//ev << "-----------------" << (*it).first << " => " << (*it).second << endl;

		cTopology topo("topo");
		topo.extractByProperty("node");

		//mad ids to indexes
		int index = 0;
		for (vector<cModule*>::iterator tempIterator = this->refletor.begin(); tempIterator
				!= this->refletor.end(); tempIterator++) {
			this->_refletorIdToIndex.insert(make_pair(
					(*tempIterator)->getParentModule()->getId(), index));
			index++;
		}

		index = 0;
		for (vector<cModule*>::iterator tempIterator = this->client.begin(); tempIterator
				!= this->client.end(); tempIterator++) {
			this->_clientIdToIndex.insert(make_pair(
					(*tempIterator)->getParentModule()->getId(), index));
			index++;
		}

		index = 0;
		for (vector<cModule*>::iterator tempIterator = this->storage.begin(); tempIterator
				!= this->storage.end(); tempIterator++) {
			this->_storageIdToIndex.insert(make_pair(
					(*tempIterator)->getParentModule()->getId(), index));
			index++;
		}

		//Setup refletor - refletor distances
		this->_refletorToRefletorDistance = new double*[this->refletor.size()];

		for (int i = 0; i <= (int) this->refletor.size() - 1; i++) {
			this->_refletorToRefletorDistance[i]
					= new double[this->refletor.size()];
		}

		for (vector<cModule*>::iterator iteratorFrom = this->refletor.begin(); iteratorFrom
				< this->refletor.end(); iteratorFrom++) {
			for (vector<cModule*>::iterator iteratorTo = this->refletor.begin(); iteratorTo
					< this->refletor.end(); iteratorTo++) {
				cTopology::Node* refletorNodeFrom = topo.getNodeFor(
						(*iteratorFrom));
				cTopology::Node* refletorNodeTo =
						topo.getNodeFor((*iteratorTo));

				topo.calculateUnweightedSingleShortestPathsTo(refletorNodeTo);
				this->_refletorToRefletorDistance[this->_refletorIdToIndex[(*iteratorFrom)->getId()]][this->_refletorIdToIndex[(*iteratorTo)->getId()]]
						= refletorNodeFrom->getDistanceToTarget();

			}
		}

		//Setup storage - refletor distances
		this->_storageToRefletorDistance = new double*[this->storage.size()];

		for (int i = 0; i <= (int) this->storage.size() - 1; i++) {
			this->_storageToRefletorDistance[i]
					= new double[this->refletor.size()];
		}

		for (vector<cModule*>::iterator iteratorFrom = this->storage.begin(); iteratorFrom
				< this->storage.end(); iteratorFrom++) {
			for (vector<cModule*>::iterator iteratorTo = this->refletor.begin(); iteratorTo
					< this->refletor.end(); iteratorTo++) {

				cTopology::Node* storageNodeFrom = topo.getNodeFor(
						(*iteratorFrom));
				cTopology::Node* refletorNodeTo =
						topo.getNodeFor((*iteratorTo));

				topo.calculateUnweightedSingleShortestPathsTo(refletorNodeTo);
				this->_refletorToRefletorDistance[this->_refletorIdToIndex[(*iteratorFrom)->getId()]][this->_refletorIdToIndex[(*iteratorTo)->getId()]]
						= storageNodeFrom->getDistanceToTarget();

			}
		}

		//Setup refletor - client distances
		this->_refletorToClientDistance = new double*[this->refletor.size()];

		for (int i = 0; i <= (int) this->refletor.size() - 1; i++) {
			this->_refletorToClientDistance[i]
					= new double[this->client.size()];
		}

		for (vector<cModule*>::iterator iteratorFrom = this->refletor.begin(); iteratorFrom
				< this->refletor.end(); iteratorFrom++) {
			for (vector<cModule*>::iterator iteratorTo = this->client.begin(); iteratorTo
					< this->client.end(); iteratorTo++) {
				cTopology::Node* refletorNodeFrom = topo.getNodeFor(
						(*iteratorFrom));
				cTopology::Node* clientNodeTo = topo.getNodeFor((*iteratorTo));

				topo.calculateUnweightedSingleShortestPathsTo(clientNodeTo);
				this->_refletorToRefletorDistance[this->_refletorIdToIndex[(*iteratorFrom)->getId()]][this->_clientIdToIndex[(*iteratorTo)->getId()]]
						= refletorNodeFrom->getDistanceToTarget();

			}
		}

		// Setup closest origin to surrogate server
		for (int k = 0; k <= (int) this->refletor.size() - 1; k++) {
			double minDistance = INT_MAX;
			int selectedStorage = -1;
			for (int i = 0; i <= (int) this->storage.size() - 1; i++) {
				if (this->_storageToRefletorDistance[i][k] <= minDistance) {
					minDistance = this->_storageToRefletorDistance[i][k];
					selectedStorage = i;
				}
			}
			this->_closestStorageToRefletor.insert(
					make_pair(k, selectedStorage));
		}

		// Setup closest surrogate server to client
		for (int k = 0; k <= (int) this->client.size() - 1; k++) {
			double minDistance = INT_MAX;
			int selectedSurrogate = -1;
			for (int i = 0; i <= (int) this->storage.size() - 1; i++) {
				if (this->_refletorToClientDistance[i][k] <= minDistance) {
					minDistance = this->_refletorToClientDistance[i][k];
					selectedSurrogate = i;
				}
			}
			this->_closestRefletorToClient.insert(make_pair(k,
					selectedSurrogate));
		}
	}
}

void Indexador::handleMessage(cMessage *msg) {
	// TODO - Generated method body
}

cModule *Indexador::getContentInStorage(Segment *segment) {
	return NULL;
}

cModule *Indexador::getContentInRefletor(Segment *segment) {
	return NULL;
}
