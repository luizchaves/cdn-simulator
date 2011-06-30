#ifndef __CDN_INDEXER_H_
#define __CDN_INDEXER_H_

#include <omnetpp.h>
#include <Storage.h>
#include <Cache.h>
#include <VideoSet.h>
#include <Reflector.h>
#include <UDPSocket.h>
#include <UDPControlInfo_m.h>
#include <IPAddressResolver.h>
#include <RequestContentCDN.h>

class Indexer : public cSimpleModule
{
  private:
	std::map<int, VideoSet*> _cdnStorageContentMap;
	std::map<int, LruCache*> _cdnRefletorContentMap;

	std::vector<cModule*> _refletorVector;
	std::vector<cModule*> _storageVector;
	std::vector<cModule*> _clientVector;

	std::map<int, int> _refletorIdToIndex;
	std::map<int, int> _storageIdToIndex;
	std::map<int, int> _clientIdToIndex;

	std::map<int, int> _closestStorageToRefletor;
	std::map<int, int> _closestRefletorToClient;

	double** _refletorToRefletorDistance;
	double** _storageToRefletorDistance;
	double** _refletorToClientDistance;

	void bindToPort(int port);
	void sendToUDP(cPacket *msg,
			int srcPort, const IPvXAddress& destAddr, int destPort);

  protected:
    virtual void initialize(int stage);
    virtual void handleMessage(cMessage *msg);

  public:
	cModule *getContentInStorage(Segment *segment, int modId);
	cModule *getContentInRefletor(Segment *segment, int modId);
    void networkIndexer(cModule *mod);
    Indexer();
    ~Indexer();
};

#endif
