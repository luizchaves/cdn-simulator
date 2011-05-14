#include <iostream>
#include <fstream>
#include <string.h>
#include <string>
#include <sstream>
#include <omnetpp.h>
#include <crng.h>

/**
 * Builds a network dynamically, with the topology coming from a
 * text file: elements and routers.
 */
class NetBuilderCDN: public cSimpleModule {
protected:
	void connect(cGate *src, cGate *dest, double delay, double ber,
			double datarate);
	void buildNetwork(cModule *parent);
	void connectClient(cModule *parent);
	virtual void initialize();
	virtual void handleMessage(cMessage *msg);
	std::map<long, cModule*> nodeid2mod;
	long totalModule;
private:
	std::map<long, cModule*> generateModuleCDN(cModule *parent);
	void generateConnectionCDNRandom(std::map<long, cModule*> & nodeid2mod);
};

Define_Module(NetBuilderCDN);

void NetBuilderCDN::initialize() {
	scheduleAt(0, new cMessage());
}

void NetBuilderCDN::handleMessage(cMessage *msg) {
	if (!msg->isSelfMessage())
		error("This module does not process messages.");
	delete msg;
	buildNetwork(getParentModule());
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

std::map<long, cModule*> NetBuilderCDN::generateModuleCDN(cModule *parent) {
	std::string line;
	std::fstream nodesFile(par("routersFile").stringValue(), std::ios::in);
	std::fstream elementsFile(par("routersFile").stringValue(), std::ios::in);
	std::map<long, cModule*> nodeid2mod;
	std::map<std::string, int> routersName;
	bool scanAttraction = false;
	std::map<std::string, int> routersAttraction;
	std::string tokensAttraction;
	int numberRouter = 0;

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
			//TODO
			float datarate = -1;
			char metricDR[5];
			sscanf(tokensEnlace[2].c_str(), "%f%s", &datarate, metricDR);
			for(int i = 0; i < 5; i++){
				metricDR[i] = tolower(metricDR[i]);
			}
			if(strcmp(metricDR,"gbps")==0)
				datarate *= 1000000000;
			if(strcmp(metricDR,"mbps")==0)
				datarate *= 1000000;
			if(strcmp(metricDR,"kbps")==0)
				datarate *= 1000;
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
				std::cout << "Atraction " << mod->getFullPath() << " " << mod->gateCount()/2 << endl;
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

	return nodeid2mod;
}

void NetBuilderCDN::buildNetwork(cModule *parent) {
	nodeid2mod = generateModuleCDN(parent);
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
}

