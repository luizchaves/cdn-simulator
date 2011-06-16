#include <iostream>
#include <fstream>
#include <string.h>
#include <string>
#include <sstream>
#include <omnetpp.h>
#include <crng.h>
#include <NetConfigurator.h>
#include <Processor.h>

/**
 * Builds a network dynamically, with the topology coming from a
 * text file: elements and routers.
 */
class NetBuilderCDN: public cSimpleModule {
protected:
	void connect(cGate *src, cGate *dest, double delay, double ber,
			double datarate);
	void connect(cModule *src, cModule *dest, double delay, double ber,
				double datarate);
	void buildNetwork(cModule *parent);
	void connectClient(cModule *parent);
	virtual void initialize();
	virtual void handleMessage(cMessage *msg);
	std::map<long, cModule*> nodeid2mod;
	long totalModule;
	int numberRouter;
	int numberStorage;
	int numberIndexer;
	int numberReflector;
	int numberProcessor;
	int numberClient;
	int numberClientTotal;
private:
	void generateModuleCDN(cModule *parent);
	void generateClientCDN(cModule *parent);
	void generateConnectionCDNRandom(std::map<long, cModule*> & nodeid2mod);
    float extractDataRatio(std::string valueEnlace);
};

Define_Module(NetBuilderCDN);

void NetBuilderCDN::initialize() {
	numberRouter = 0;
	numberStorage = 0;
	numberIndexer = 0;
	numberReflector = 0;
	numberProcessor = 0;
	numberClient = 0;
	//TODO pegar parametrizado
	numberClientTotal = 2;
	scheduleAt(0, new cMessage("BuildCDN", 1));
}

void NetBuilderCDN::handleMessage(cMessage *msg) {
	if (!msg->isSelfMessage())
		error("This module does not process messages.");
	delete msg;
	if(msg->getKind() == 1)
		buildNetwork(getParentModule());
//	if(msg->getKind() == 2)
//		generateClientCDN(getParentModule());
}

void NetBuilderCDN::connect(cGate *src, cGate *dest, double delay, double ber,
		double datarate) {
	cDatarateChannel *channel = NULL;
	if (delay > 0 || ber > 0 || datarate > 0) {
		channel = cDatarateChannel::create("channel");
		if (delay > 0)
			channel->setDelay(delay);
		if (ber > 0)
			channel->setBitErrorRate(ber);
		if (datarate > 0)
			channel->setDatarate(datarate);
	}
	src->connectTo(dest, channel);
}

void NetBuilderCDN::connect(cModule *src, cModule *dest, double delay, double ber,
		double datarate) {
	cDatarateChannel *channel = NULL;
	if (delay > 0 || ber > 0 || datarate > 0) {
		channel = cDatarateChannel::create("channel");
		if (delay > 0)
			channel->setDelay(delay);
		if (ber > 0)
			channel->setBitErrorRate(ber);
		if (datarate > 0)
			channel->setDatarate(datarate);
	}
	src->gate("pppg")->connectTo(dest->gate("pppg"), channel);
}

float NetBuilderCDN::extractDataRatio(std::string valueEnlace)
{
    float datarate = 0;
	char metricDR[5];
    sscanf(valueEnlace.c_str(), "%f%s", &datarate, metricDR);
    for(int i = 0;i < 5;i++){
        metricDR[i] = tolower(metricDR[i]);
    }
    if(strcmp(metricDR, "gbps") == 0)
        datarate *= 1000000000;

    if(strcmp(metricDR, "mbps") == 0)
        datarate *= 1000000;

    if(strcmp(metricDR, "kbps") == 0)
        datarate *= 1000;

    return datarate;
}

void NetBuilderCDN::buildNetwork(cModule *parent) {
	generateModuleCDN(parent);
	generateClientCDN(parent);

	std::map<long, cModule *>::iterator it;
	// final touches: buildinside, initialize()
	for (it = nodeid2mod.begin(); it != nodeid2mod.end(); ++it) {
		cModule *mod = it->second;
		mod->buildInside();
	}

	// multi-stage init
	bool more = true;
	for (int stage = 0; more; stage++) {
		more = false;
		for (it = nodeid2mod.begin(); it != nodeid2mod.end(); ++it) {
			cModule *mod = it->second;
			if (mod->callInitialize(stage))
				more = true;
		}
	}

	//Config IP Net
	for (cModule::SubmoduleIterator iter(getParentModule()); !iter.end(); iter++) {
		if (strcmp(iter()->getFullName(), "netConfigurator") == 0) {
			NetConfigurator* flatNet = (NetConfigurator*)iter();
			flatNet->configNet();
		}
	}

	//load Distances
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

	cTopology topo("topo");
	topo.extractByProperty("node");

	//network's iterator
	for (cModule::SubmoduleIterator iter(getParentModule()); !iter.end(); iter++) {
		//CDNNode
		if (strcmp(iter()->getModuleType()->getName(), "CDNNode") == 0) {
			//Storage CDNNode
			if (!strcmp(((cModule*) iter())->par("type"), "s")) {
				std::cout << iter()->getFullName() << endl;
				//insert storage CDNNode in vector
				storage.push_back((cModule*) iter());
				//get vector videoset from storage
				Storage* storage;
				for (cModule::SubmoduleIterator iterCDNContent(iter()); !iterCDNContent.end(); iterCDNContent++) {
					if (!strcmp(((cModule*) iterCDNContent())->getName(), "udpApp"))
						storage = (Storage*) iterCDNContent();
				}
				vector<VideoSet*> videoSetVector = storage->getVideoSetVector();
				//VideoSet Iterator
				for (vector<VideoSet*>::iterator it = videoSetVector.begin(); it < videoSetVector.end(); it++) {
					//Add videoSet in vector
					VideoSet* vs = (VideoSet*) (*it);
					if (cdnStorageContentMap[vs->getCDNId()] == NULL) {
						cdnStorageContentMap[vs->getCDNId()] = vs;
					} else {
						cdnStorageContentMap[vs->getCDNId()]->addVideo(vs->getVideoVector());
					}
				}
			//Reflector CDNNode
			} else if (!strcmp(((cModule*) iter())->par("type"), "r")) {
				std::cout << iter()->getFullName() << endl;
				//insert reflector CDNNode in vector
				refletor.push_back(((cModule*) iter()));
				//get vector cache from refletor
				Reflector* reflector;
				for (cModule::SubmoduleIterator iterCDNContent(iter()); !iterCDNContent.end(); iterCDNContent++) {
					if (!strcmp(((cModule*) iterCDNContent())->getName(), "udpApp"))
						reflector = (Reflector*) iterCDNContent();
				}
				vector<Cache*> cacheVector = reflector->getCacheVector();
				//Cache Iterator
				for (vector<Cache*>::iterator it = cacheVector.begin(); it < cacheVector.end(); it++) {
					//Add cache in vector
					//TODO Que porra é isso!!! Herança?
					Cache* c = (*it);
					if (cdnRefletorContentMap[c->getCDNId()] == NULL) {
						cdnRefletorContentMap[c->getCDNId()] = c;
					} else {
						cdnRefletorContentMap[c->getCDNId()]->addSegment(c->getSegmentVector());
					}
				}
			//Client CDNNode
			} else if (!strcmp(((cModule*) iter())->par("type"), "c")) {
				//insert client CDNNode in vector
				client.push_back(((cModule*) iter()));
			}
		}
	}

	std::cout<< "----------------- Storage " << cdnStorageContentMap.size() << endl;
	for ( map<int, VideoSet*>::iterator it=cdnStorageContentMap.begin() ; it != cdnStorageContentMap.end(); it++ ){
		std::cout<< "-----------------CND ID " << (*it).second->getCDNId() << endl;
		std::cout<< "-----------------VIDEO SET ID " << (*it).second->getId() << endl;
		vector<Video*> videoSetMap = (*it).second->getVideoVector();
		for (vector<Video*>::iterator itvs = videoSetMap.begin();	itvs != videoSetMap.end(); itvs++) {
			std::cout<< "----------------- VIDEO " << (*itvs)->getId() << endl;
		}
	}
	std::cout<< "----------------- Refletor " << cdnRefletorContentMap.size() << endl;
//	for ( map<int, VideoSet*>::iterator it=cdnStorageContentMap.begin() ; it != cdnStorageContentMap.end(); it++ ){
//		std::cout<< "-----------------CND ID " << (*it).second->getCDNId() << endl;
//		std::cout<< "-----------------VIDEO SET ID " << (*it).second->getId() << endl;
//		vector<Video*> videoSetMap = (*it).second->getVideoVector();
//		for (vector<Video*>::iterator itvs = videoSetMap.begin();	itvs != videoSetMap.end(); itvs++) {
//			std::cout<< "----------------- VIDEO " << (*itvs)->getId() << endl;
//		}
//	}


	//mad ids to indexes
	int index = 0;
	for (vector<cModule*>::iterator tempIterator = refletor.begin(); tempIterator	!= refletor.end(); tempIterator++) {
		_refletorIdToIndex.insert(make_pair((*tempIterator)->getId(), index));
		index++;
	}

	index = 0;
	for (vector<cModule*>::iterator tempIterator = client.begin(); tempIterator != client.end(); tempIterator++) {
		_clientIdToIndex.insert(make_pair((*tempIterator)->getId(), index));
		index++;
	}

	index = 0;
	for (vector<cModule*>::iterator tempIterator = storage.begin(); tempIterator != storage.end(); tempIterator++) {
		_storageIdToIndex.insert(make_pair((*tempIterator)->getId(), index));
		index++;
	}

	//Setup refletor - refletor distances
	_refletorToRefletorDistance = new double*[refletor.size()];

	for (int i = 0; i <= (int) refletor.size() - 1; i++) {
		_refletorToRefletorDistance[i] = new double[refletor.size()];
	}

	for (vector<cModule*>::iterator iteratorFrom = refletor.begin(); iteratorFrom < refletor.end(); iteratorFrom++) {
		for (vector<cModule*>::iterator iteratorTo = refletor.begin(); iteratorTo < refletor.end(); iteratorTo++) {
			cTopology::Node* refletorNodeFrom = topo.getNodeFor((*iteratorFrom));
			cTopology::Node* refletorNodeTo = topo.getNodeFor((*iteratorTo));

			topo.calculateUnweightedSingleShortestPathsTo(refletorNodeTo);
			_refletorToRefletorDistance[_refletorIdToIndex[(*iteratorFrom)->getId()]]
					[_refletorIdToIndex[(*iteratorTo)->getId()]]
					= refletorNodeFrom->getDistanceToTarget();
		}
	}

	//Setup storage - refletor distances
	_storageToRefletorDistance = new double*[storage.size()];

	for (int i = 0; i <= (int) storage.size() - 1; i++) {
		_storageToRefletorDistance[i]	= new double[refletor.size()];
	}

	for (vector<cModule*>::iterator iteratorFrom = storage.begin(); iteratorFrom < storage.end(); iteratorFrom++) {
		for (vector<cModule*>::iterator iteratorTo = refletor.begin(); iteratorTo < refletor.end(); iteratorTo++) {

			cTopology::Node* storageNodeFrom = topo.getNodeFor((*iteratorFrom));
			cTopology::Node* refletorNodeTo = topo.getNodeFor((*iteratorTo));

			topo.calculateUnweightedSingleShortestPathsTo(refletorNodeTo);
			_refletorToRefletorDistance[_refletorIdToIndex[(*iteratorFrom)->getId()]]
					[_refletorIdToIndex[(*iteratorTo)->getId()]]
					= storageNodeFrom->getDistanceToTarget();
		}
	}

	//Setup refletor - client distances
	_refletorToClientDistance = new double*[refletor.size()];

	for (int i = 0; i <= (int) refletor.size() - 1; i++) {
		_refletorToClientDistance[i] = new double[client.size()];
	}

	for (vector<cModule*>::iterator iteratorFrom = refletor.begin(); iteratorFrom < refletor.end(); iteratorFrom++) {
		for (vector<cModule*>::iterator iteratorTo = client.begin(); iteratorTo < client.end(); iteratorTo++) {
			cTopology::Node* refletorNodeFrom = topo.getNodeFor((*iteratorFrom));
			cTopology::Node* clientNodeTo = topo.getNodeFor((*iteratorTo));

			topo.calculateUnweightedSingleShortestPathsTo(clientNodeTo);
			_refletorToRefletorDistance[_refletorIdToIndex[(*iteratorFrom)->getId()]]
					[_clientIdToIndex[(*iteratorTo)->getId()]]
					= refletorNodeFrom->getDistanceToTarget();
		}
	}

	// Setup closest origin to surrogate server
	for (int k = 0; k <= (int) refletor.size() - 1; k++) {
		double minDistance = INT_MAX;
		int selectedStorage = -1;
		for (int i = 0; i <= (int) storage.size() - 1; i++) {
			if (_storageToRefletorDistance[i][k] <= minDistance) {
				minDistance = _storageToRefletorDistance[i][k];
				selectedStorage = i;
			}
		}
		_closestStorageToRefletor.insert(make_pair(k, selectedStorage));
	}

	// Setup closest surrogate server to client
	for (int k = 0; k <= (int) client.size() - 1; k++) {
		double minDistance = INT_MAX;
		int selectedSurrogate = -1;
		for (int i = 0; i <= (int) storage.size() - 1; i++) {
			if (_refletorToClientDistance[i][k] <= minDistance) {
				minDistance = _refletorToClientDistance[i][k];
				selectedSurrogate = i;
			}
		}
		_closestRefletorToClient.insert(make_pair(k, selectedSurrogate));
	}

}

void NetBuilderCDN::generateModuleCDN(cModule *parent) {
	std::string line;
	std::fstream nodesFile(par("routersFile").stringValue(), std::ios::in);
	std::fstream elementsFile(par("elementsFile").stringValue(), std::ios::in);
	std::map<std::string, int> routersName;
	std::map<std::string, int> storageName;
	std::map<std::string, int> indexerName;
	std::map<std::string, int> reflectorName;
	std::map<std::string, int> processorName;
	bool scanAttraction = false;
	std::string tokensAttraction;
	std::map<std::string, int> routersAttraction;


	while (getline(nodesFile, line, '\n')) {
		if (line.empty() || line[0] == '#')
			continue;

		// Split in space
		cStringTokenizer tokenizer(line.c_str());
		tokenizer.setDelimiter(" ");
		std::vector<std::string> tokens = tokenizer.asVector();

		// Split in comma
		cStringTokenizer tokenizerEnlace(line.c_str());
		tokenizerEnlace.setDelimiter(",");
		std::vector<std::string> tokensEnlace = tokenizerEnlace.asVector();

		if (strcmp(tokens[0].c_str(), "NODES") == 0) {
			// Description of Node's Name
			cStringTokenizer tokenizerRouters(tokens[1].c_str());
			tokenizerRouters.setDelimiter(",");
			std::vector<std::string> tokensRouters =
					tokenizerRouters.asVector();
			for (std::vector<std::string>::iterator it = tokensRouters.begin(); it
					< tokensRouters.end(); it++) {
				const char *displayString = "";
				const char *modtypename = "inet.nodes.inet.Router";
				cModuleType *modtype = cModuleType::get(modtypename);
				if(!modtype)
					throw cRuntimeError("module type `%s' for node `%d' not found",modtypename, numberRouter);
				cModule *mod;
				mod = modtype->create("router", parent);
				mod->setName((*it).c_str());
				nodeid2mod[numberRouter] = mod;
				// read params from the ini file, etc
				mod->finalizeParameters();
				// modify display string
				if(strcmp("", displayString) != 0)
					mod->setDisplayString(displayString);
				routersName.insert(std::make_pair((*it), numberRouter));
				EV << "NODE ID " << (*it).c_str() << " " << numberRouter << endl;
				numberRouter++;
			}
		} else if (strcmp(tokens[0].c_str(), "ATTRACTION") == 0){
			scanAttraction = true;
			tokensAttraction = tokens[1].c_str();
		} else if (tokensEnlace.size() == 4) {
			// Enlaces
			// get fields from tokens
			EV << tokensEnlace[0] << " " << tokensEnlace[1] << " " << tokensEnlace[2] << " " << tokensEnlace[3] << endl;
			long srcnodeid = routersName.find(tokensEnlace[0].c_str())->second;
			long destnodeid = routersName.find(tokensEnlace[1].c_str())->second;
			float datarate = -1;
			datarate = extractDataRatio(tokensEnlace[2].c_str());
			float delay = -1;
			char metricD[3];
			sscanf(tokensEnlace[3].c_str(), "%f%c", &delay, metricD);
			double error = 0;
			EV << "Enlace " << srcnodeid << ", " << destnodeid << ", " << datarate << ", " << delay << endl;
			if(nodeid2mod.find(srcnodeid) == nodeid2mod.end())
				throw cRuntimeError("wrong line in connections file: node with id=%ld not found", srcnodeid);

			if(nodeid2mod.find(destnodeid) == nodeid2mod.end())
				throw cRuntimeError("wrong line in connections file: node with id=%ld not found", destnodeid);

			cModule *srcmod = nodeid2mod[srcnodeid];
			cModule *destmod = nodeid2mod[destnodeid];
			cGate *srcIn, *srcOut, *destIn, *destOut;
			srcmod->getOrCreateFirstUnconnectedGatePair("pppg", false, true, srcIn, srcOut);
			destmod->getOrCreateFirstUnconnectedGatePair("pppg", false, true, destIn, destOut);
			// connect
			connect(srcOut, destIn, delay, error, datarate);
			connect(destOut, srcIn, delay, error, datarate);
		}
	}
	if (scanAttraction == true){
		// Define attraction level
		//TODO Chek if attraction is empty
		cStringTokenizer tokenizerAttraction(tokensAttraction.c_str());
		tokenizerAttraction.setDelimiter(",");
		std::vector<std::string> tokensAttraction = tokenizerAttraction.asVector();
		// Calcute value of Attraction by default
		if(routersName.size() != routersAttraction.size()){
			for(std::map<std::string,int>::iterator itRoutersName = routersName.begin();itRoutersName != routersName.end();itRoutersName++){
				std::map<std::string,int>::iterator itRoutersAttraction;
				itRoutersAttraction = routersAttraction.find(itRoutersName->first);
				cModule *mod = nodeid2mod[itRoutersName->second];
				if(itRoutersAttraction == routersAttraction.end()){
					routersAttraction.insert(std::make_pair(itRoutersName->first, mod->gateCount()/2));
				}
			}
		}
		//Insert values of attraction in map
		for(std::vector<std::string>::iterator it = tokensAttraction.begin();it < tokensAttraction.end();it++){
			cStringTokenizer tokenizer((*it).c_str());
			tokenizer.setDelimiter("=");
			std::vector<std::string> contentAttraction = tokenizer.asVector();
			routersAttraction.insert(std::make_pair(contentAttraction[0], atol(contentAttraction[1].c_str())));
		}
	}

	while (getline(elementsFile, line, '\n')) {
			float delayDefault;
			float datarateDefault;

			if (line.empty() || line[0] == '#')
				continue;

			// Split in space
			cStringTokenizer tokenizer(line.c_str());
			tokenizer.setDelimiter(" ");
			std::vector<std::string> tokens = tokenizer.asVector();

			if (strcmp(tokens[0].c_str(), "LOCAL_CONNECTIVITY") == 0) {
				// Description of Node's Name
				cStringTokenizer valueLinkElements(tokens[1].c_str());
				valueLinkElements.setDelimiter(",");
				std::vector<std::string> value =
						valueLinkElements.asVector();
				char metricD[3];
				sscanf(value[1].c_str(), "%f%c", &delayDefault, metricD);
				datarateDefault = extractDataRatio(value[0].c_str());
			}

			// Split in comma
			cStringTokenizer enlaces(line.c_str());
			enlaces.setDelimiter(",");
			std::vector<std::string> tokensEnlaces = enlaces.asVector();

			//TODO Check id attribuition for unordered kind of module
			if (tokensEnlaces.size() == 5 || tokensEnlaces.size() == 4 || (tokensEnlaces.size() == 3 && isalpha(tokensEnlaces[2][1]))) {
				// chek kind and create element
				const char *displayString = "";
				const char *modtypename = "src.cdn.node.CDNNode";
				const char *modName;
				int *number;
				int addNumber = 0;
				std::map<std::string, int> *mapElement;
				cModuleType *modtype = cModuleType::get(modtypename);
				if(!modtype)
					throw cRuntimeError("module type `%s' for node `%d' not found",modtypename, numberRouter);
				cModule *mod;
				if(strcmp(tokensEnlaces[1].c_str(), "STORAGE") == 0){
					modName = "storage";
					number = &numberStorage;
					mapElement = &storageName;
					addNumber = numberRouter;
					displayString = "i=device/server";
					mod = modtype->create(modName, parent);
					mod->par("numUdpApps").setLongValue(1);
					mod->par("udpAppType").setStringValue("Storage");
					mod->par("type").setStringValue("s");
				} else if(strcmp(tokensEnlaces[1].c_str(), "INDEXER") == 0){
					modName = "indexer";
					number = &numberIndexer;
					mapElement = &indexerName;
					addNumber = numberRouter + numberStorage;
					displayString = "i=block/network2";
					mod = modtype->create(modName, parent);
					mod->par("numUdpApps").setLongValue(1);
					mod->par("udpAppType").setStringValue("Indexer");
					mod->par("type").setStringValue("i");
				} else if(strcmp(tokensEnlaces[1].c_str(), "REFLECTOR") == 0) {
					modName = "reflector";
					number = &numberReflector;
					mapElement = &reflectorName;
					addNumber = numberRouter + numberStorage + numberIndexer;
					displayString = "i=abstract/db";
					mod = modtype->create(modName, parent);
					mod->par("numUdpApps").setLongValue(1);
					mod->par("udpAppType").setStringValue("Reflector");
					mod->par("type").setStringValue("r");
				} else if(strcmp(tokensEnlaces[1].c_str(), "PROCESSOR") == 0) {
					modName = "processor";
					number = &numberProcessor;
					mapElement = &processorName;
					addNumber = numberRouter + numberStorage + numberIndexer + numberReflector;
					displayString = "i=device/cpu";
					mod = modtype->create(modName, parent);
					mod->par("numUdpApps").setLongValue(1);
					mod->par("udpAppType").setStringValue("Processor");
					mod->par("type").setStringValue("p");
				}
				mod->setName(tokensEnlaces[0].c_str());
				nodeid2mod[*number + addNumber] = mod;
				// read params from the ini file, etc
				mod->finalizeParameters();
				// modify display string
				if(strcmp("", displayString) != 0)
					mod->setDisplayString(displayString);
				// insert in map
				mapElement->insert(std::make_pair(tokensEnlaces[0], *number + addNumber));
				EV << "NODE ID " << modName << " " << *number+addNumber << endl;
				*number +=1;
				// connect
				//EV << tokensEnlaces[0] << " " << tokensEnlaces[1] << " " << tokensEnlaces[2] << " " << tokensEnlaces[3] << tokensEnlaces[4] << endl;
				long srcnodeid = routersName.find(tokensEnlaces[2].c_str())->second;
				long destnodeid = mapElement->find(tokensEnlaces[0].c_str())->second;
				float datarate = -1;
				float delay = -1;
				if (tokensEnlaces.size() == 5) {
					char metricD[3];
					sscanf(tokensEnlaces[4].c_str(), "%f%c", &delay, metricD);
					datarate = extractDataRatio(tokensEnlaces[3].c_str());
				}
				if (tokensEnlaces.size() == 4) {
					delay = delayDefault;
					datarate = extractDataRatio(tokensEnlaces[3].c_str());
				}
				if (tokensEnlaces.size() == 3) {
					delay = delayDefault;
					datarate = datarateDefault;
				}
				double error = 0;
				EV << "Enlace " << srcnodeid << ", " << destnodeid << ", " << datarate << ", " << delay << endl;
				if(nodeid2mod.find(srcnodeid) == nodeid2mod.end())
					throw cRuntimeError("wrong line in connections file: node with id=%ld not found", srcnodeid);

				if(nodeid2mod.find(destnodeid) == nodeid2mod.end())
					throw cRuntimeError("wrong line in connections file: node with id=%ld not found", destnodeid);

				cModule *srcmod = nodeid2mod[srcnodeid];
				cModule *destmod = nodeid2mod[destnodeid];
				cGate *srcIn, *srcOut, *destIn, *destOut;
				srcmod->getOrCreateFirstUnconnectedGatePair("pppg", false, true, srcIn, srcOut);
				destmod->getOrCreateFirstUnconnectedGatePair("pppg", false, true, destIn, destOut);
				// connect
				connect(srcOut, destIn, delay, error, datarate);
				connect(destOut, srcIn, delay, error, datarate);
			}
			if (tokensEnlaces.size() == 3 && !isalpha(tokensEnlaces[2][1])) {
				// Identify kind of component and create module
				const char *displayString = "";
				const char *modtypename = "src.cdn.node.CDNNode";
				const char *modName;
				int *number;
				int addNumber = 0;
				std::map<std::string, int> *mapElement;
				int repeatNumber = 0;
				char metricD[1] = "";
				sscanf(tokensEnlaces[2].c_str(), "%c%d", metricD, &repeatNumber);
				if (isdigit(metricD[0]))
					sscanf(tokensEnlaces[2].c_str(), "%d", &repeatNumber);
				for (int var = 0; var < repeatNumber; ++var) {
					cModuleType *modtype = cModuleType::get(modtypename);
					if(!modtype)
						throw cRuntimeError("module type `%s' for node `%d' not found",modtypename, numberRouter);
					cModule *mod;
					if(strcmp(tokensEnlaces[1].c_str(), "STORAGE") == 0){
						modName = "storage";
						number = &numberStorage;
						mapElement = &storageName;
						addNumber = numberRouter;
						displayString = "i=device/server";
						mod = modtype->create(modName, parent);
						mod->par("numUdpApps").setLongValue(1);
						mod->par("udpAppType").setStringValue("Storage");
						mod->par("type").setStringValue("s");
					} else if(strcmp(tokensEnlaces[1].c_str(), "INDEXER") == 0){
						modName = "indexer";
						number = &numberIndexer;
						mapElement = &indexerName;
						addNumber = numberRouter + numberStorage;
						displayString = "i=block/network2";
						mod = modtype->create(modName, parent);
						mod->par("numUdpApps").setLongValue(1);
						mod->par("udpAppType").setStringValue("Indexer");
						mod->par("type").setStringValue("i");
					} else if(strcmp(tokensEnlaces[1].c_str(), "REFLECTOR") == 0) {
						modName = "reflector";
						number = &numberReflector;
						mapElement = &reflectorName;
						addNumber = numberRouter + numberStorage + numberIndexer;
						displayString = "i=abstract/db";
						mod = modtype->create(modName, parent);
						mod->par("numUdpApps").setLongValue(1);
						mod->par("udpAppType").setStringValue("Reflector");
						mod->par("type").setStringValue("r");
					} else if(strcmp(tokensEnlaces[1].c_str(), "PROCESSOR") == 0) {
						modName = "processor";
						number = &numberProcessor;
						mapElement = &processorName;
						addNumber = numberRouter + numberStorage + numberIndexer + numberReflector;
						displayString = "i=device/cpu";
						mod = modtype->create(modName, parent);
						mod->par("numUdpApps").setLongValue(1);
						mod->par("udpAppType").setStringValue("Processor");
						mod->par("type").setStringValue("p");
					}
					std::stringstream numberStream;
					numberStream << *number;
					std::string nameModule = std::string(tokensEnlaces[0].c_str()).append(numberStream.str()).c_str();
					// Check exist id
					if (mapElement->find(nameModule) != mapElement->end()) {
						std::stringstream numberStream2;
						numberStream2 << *number+1;
						nameModule = std::string(tokensEnlaces[0].c_str()).append(numberStream2.str()).c_str();
					}
					mod->setName(nameModule.c_str());
					nodeid2mod[*number+addNumber] = mod;
					// read params from the ini file, etc
					mod->finalizeParameters();
					// modify display string
					if(strcmp("", displayString) != 0)
						mod->setDisplayString(displayString);
					// insert in map
					mapElement->insert(std::make_pair(nameModule, *number+addNumber));
					EV << "NODE ID " << modName << " " << *number+addNumber << endl;
					*number += 1;
					// connect
					//TODO escolher roteador FDP
					//TODO real random
					//TODO se o caracter for diferente não pode escolher roteadores que já possuam
					// o mesmo elemento e se for positivo escolher o grau de atração
					int addRouter = uniform(0, numberRouter, (int) dblrand()*1e6);
					std::map<std::string, int>::iterator itRouter =routersName.begin();
					for (int var = 0; var < addRouter; ++var) {
						itRouter ++;
					}
					long srcnodeid = itRouter->second;
					long destnodeid = mapElement->find(mod->getName())->second;
					float datarate = -1;
					float delay = -1;
					delay = delayDefault;
					datarate = datarateDefault;
					double error = 0;
					EV << "Enlace " << srcnodeid << ", " << destnodeid << ", " << datarate << ", " << delay << endl;
					if(nodeid2mod.find(srcnodeid) == nodeid2mod.end())
						throw cRuntimeError("wrong line in connections file: node with id=%ld not found", srcnodeid);

					if(nodeid2mod.find(destnodeid) == nodeid2mod.end())
						throw cRuntimeError("wrong line in connections file: node with id=%ld not found", destnodeid);

					cModule *srcmod = nodeid2mod[srcnodeid];
					cModule *destmod = nodeid2mod[destnodeid];
					cGate *srcIn, *srcOut, *destIn, *destOut;
					srcmod->getOrCreateFirstUnconnectedGatePair("pppg", false, true, srcIn, srcOut);
					destmod->getOrCreateFirstUnconnectedGatePair("pppg", false, true, destIn, destOut);
					// connect
					connect(srcOut, destIn, delay, error, datarate);
					connect(destOut, srcIn, delay, error, datarate);
				}
			}
	}
}

void NetBuilderCDN::generateClientCDN(cModule *parent) {
	for(int client = 0; client < numberClientTotal; client++){
		cModuleType *modtype = cModuleType::get("src.cdn.node.CDNNode");
		if(!modtype){
			throw cRuntimeError("module type `%s' for node `%d' not found", "src.cdn.node.CDNNode", numberClient);
		}

		std::stringstream number;
		number << numberClient;

		cModule *mod = modtype->create("client", parent);
		mod->setName(std::string("CL").append(number.str()).c_str());
		mod->par("numUdpApps").setLongValue(1);
		mod->par("udpAppType").setStringValue("Client");
		mod->par("type").setStringValue("c");

		//std::cout << "Numero cliente " << numberRouter+numberStorage+numberIndexer+numberReflector+numberProcessor+numberClient << endl;
		nodeid2mod[numberRouter+numberStorage+numberIndexer+numberReflector+numberProcessor+numberClient] = mod;
		// read params from the ini file, etc
		mod->finalizeParameters();

		long srcnodeid = (long) uniform(0, numberRouter, (int) dblrand()*1e6);
		long destnodeid = numberRouter+numberStorage+numberIndexer+numberReflector+numberProcessor+numberClient;
		double datarate = 100000000;
		double delay = 10;
		double error = 0;
		EV << "Enlace " << srcnodeid << ", " << destnodeid << ", " << datarate << ", " << delay << endl;
		if(nodeid2mod.find(srcnodeid) == nodeid2mod.end())
			throw cRuntimeError("wrong line in connections file: node with id=%ld not found", srcnodeid);

		if(nodeid2mod.find(destnodeid) == nodeid2mod.end())
			throw cRuntimeError("wrong line in connections file: node with id=%ld not found", destnodeid);

		cModule *srcmod = nodeid2mod[srcnodeid];
		//std::cout << "Connecting " << srcmod->getFullName();
		cModule *destmod = nodeid2mod[destnodeid];
		//std::cout << " to " << destmod->getFullName() << endl;
		cGate *srcIn, *srcOut, *destIn, *destOut;
		srcmod->getOrCreateFirstUnconnectedGatePair("pppg", false, true, srcIn, srcOut);
		destmod->getOrCreateFirstUnconnectedGatePair("pppg", false, true, destIn, destOut);
		// connect
		connect(srcOut, destIn, delay, error, datarate);
		connect(destOut, srcIn, delay, error, datarate);
		numberClient++;
	}

	//TODO se a quantidade de clientes ativos ainda não foi atingido pode enviar uma nova messagem
//	if(numberClient < 1)
//		scheduleAt(simTime()+exponential(1.0), new cMessage("BuildCDN", 2));
}

