#ifndef __CDN_INDEXER_H_
#define __CDN_INDEXER_H_

#include <omnetpp.h>
#include <Storage.h>
#include <Cache.h>
#include <VideoSet.h>
#include <Refletor.h>

class Indexer : public cSimpleModule
{
  private:
	map<int, VideoSet*> cdnStorageContentMap;
	map<int, Cache*> cdnRefletorContentMap;

	map<int, int> _refletorIdToIndex;
	map<int, int> _storageIdToIndex;
	map<int, int> _clientIdToIndex;

	vector<cModule*> refletor;
	vector<cModule*> storage;
	vector<cModule*> client;

	map<int, int> _closestStorageToRefletor;
	map<int, int> _closestRefletorToClient;

	double** _refletorToRefletorDistance;
	double** _storageToRefletorDistance;
	double** _refletorToClientDistance;

  protected:
    virtual void initialize(int stage);
    virtual void handleMessage(cMessage *msg);
  public:
	cModule *getContentInStorage(Segment *segment);
	cModule *getContentInRefletor(Segment *segment);
};

#endif
