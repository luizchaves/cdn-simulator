#ifndef __CDN_INDEXADOR_H_
#define __CDN_INDEXADOR_H_

#include <omnetpp.h>
#include <Storage.h>
#include <Cache.h>
#include <VideoSet.h>

class Indexador : public cSimpleModule
{
  private:
	map<int, VideoSet*> cdnStorageContentMap;
	map<int, Cache*> cdnRefletorContentMap;
	vector<cModule*> refletor;
	vector<cModule*> storage;
	vector<cModule*> client;
	map<int, int> closestStorageToRefletor;
	map<int, int> closestRefletorToClient;
  protected:
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);
  public:
	cModule *getContentInStorage(Video *video);
	cModule *getContentInRefletor(Segment *segment);
};

#endif
