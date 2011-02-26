#include <iostream>
#include <fstream>
#include <vector>
#include <string.h>
#include <string>
#include <sstream>
#include <omnetpp.h>
#include <crng.h>


/**
 * Builds a network dynamically, with the topology coming from a
 * text file.
 */
class NetBuilder : public cSimpleModule
{
  protected:
    void connect(cGate *src, cGate *dest, double delay, double ber, double datarate);
    void buildNetwork(cModule *parent);
    void connectClient(cModule *parent);
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);
    std::map<long ,cModule*> nodeid2mod;
    int numberRouter;
    int numberStorage;
    int numberRefletor;
    int numberIndexador;
    int numberProcessador;
    int numberClient;
    long totalModule;
private:
    std::map<long ,cModule*> generateModuleCDNDyn(cModule *parent);
    void generateConnectionCDNStatic(std::map<long ,cModule*> & nodeid2mod);
    void generateConnectionCDNRandom(std::map<long ,cModule*> & nodeid2mod);
};

Define_Module(NetBuilder);


void NetBuilder::initialize()
{
    // build the network in event 1, because it is undefined whether the simkernel
    // will implicitly initialize modules created *during* initialization, or this needs
    // to be done manually.
    scheduleAt(0, new cMessage());
}

void NetBuilder::handleMessage(cMessage *msg)
{
    if (!msg->isSelfMessage())
        error("This module does not process messages.");

    delete msg;

	buildNetwork(getParentModule());
}

void NetBuilder::connect(cGate *src, cGate *dest, double delay, double ber, double datarate)
{
    cDatarateChannel *channel = NULL;
    if (delay>0 || ber>0 || datarate>0)
    {
        channel = cDatarateChannel::create("channel");
        if (delay>0)
            channel->setDelay(delay);
        if (ber>0)
            channel->setBitErrorRate(ber);
        if (datarate>0)
            channel->setDatarate(datarate);
    }
    src->connectTo(dest, channel);
}

std::map<long ,cModule*> NetBuilder::generateModuleCDNDyn(cModule *parent)
{
    //number of CDN elements
	numberRouter = par("numberRouter").longValue();
    numberStorage = par("numberStorage").longValue();
    numberRefletor = par("numberRefletor").longValue();
    numberIndexador = par("numberIndexador").longValue();
    numberProcessador = par("numberProcessador").longValue();

    std::map<long ,cModule*> nodeid2mod;

    totalModule = numberRouter + numberStorage + numberRefletor + numberIndexador + numberProcessador;

    // create module
    for(int var = 0;var < totalModule;++var){
        const char *displayString = "";
        const char *modtypename = "";
        modtypename = "inet.nodes.inet.Router";
        if(var >= numberRouter){
            modtypename = "src.cdn.node.CDNNode";
        }
        std::stringstream number;
        number << var;

        cModuleType *modtype = cModuleType::get(modtypename);
        if(!modtype){
            throw cRuntimeError("module type `%s' for node `%d' not found", modtypename, var);
        }

        cModule *mod;
        if(var < numberRouter){
            mod = modtype->create("router", parent);
            mod->setName(std::string("router").append(number.str()).c_str());
        }else
            if(var < numberRouter + numberStorage){
                mod = modtype->create("storage", parent);
                mod->setName(std::string("storage").append(number.str()).c_str());
                mod->par("numUdpApps").setLongValue(1000);
                mod->par("udpAppType").setStringValue("Storage");
                displayString = "i=device/server";
            }else
                if(var < numberRouter + numberStorage + numberRefletor){
                    mod = modtype->create("refletor", parent);
                    mod->setName(std::string("refletor").append(number.str()).c_str());
                    mod->par("numUdpApps").setLongValue(1000);
                    mod->par("udpAppType").setStringValue("Refletor");
                    displayString = "i=abstract/db";
                }else
                    if(var < numberRouter + numberStorage + numberRefletor + numberIndexador){
                        mod = modtype->create("indexador", parent);
                        mod->setName(std::string("indexador").append(number.str()).c_str());
                        mod->par("numUdpApps").setLongValue(1000);
                        mod->par("udpAppType").setStringValue("Indexador");
                        displayString = "i=block/network2";
                    }else
                        if(var < numberRouter + numberStorage + numberRefletor + numberIndexador + numberProcessador){
                            mod = modtype->create("processador", parent);
                            mod->setName(std::string("processador").append(number.str()).c_str());
                            mod->par("numUdpApps").setLongValue(1000);
                            mod->par("udpAppType").setStringValue("Processador");
                            displayString = "i=device/cpu";
                        }

        nodeid2mod[var] = mod;
        // read params from the ini file, etc
        mod->finalizeParameters();
        // modify display string
        if(strcmp("", displayString) != 0)
            mod->setDisplayString(displayString);

    }
    return nodeid2mod;
}

void NetBuilder::generateConnectionCDNStatic(std::map<long ,cModule*> & nodeid2mod)
{
    // read and create connections
    std::string line;
    std::fstream connectionsFile(par("connectionsFile").stringValue(), std::ios::in);
    while(getline(connectionsFile, line, '\n')){
        if(line.empty() || line[0] == '#')
            continue;

        std::vector<std::string> tokens = cStringTokenizer(line.c_str()).asVector();
        if(tokens.size() != 5)
            throw cRuntimeError("wrong line in parameters file: 5 items required, line: \"%s\"", line.c_str());

        // get fields from tokens
        long srcnodeid = atol(tokens[0].c_str());
        long destnodeid = atol(tokens[1].c_str());
        double delay = tokens[2] != "-" ? atof(tokens[2].c_str()) : -1;
        double error = tokens[3] != "-" ? atof(tokens[3].c_str()) : -1;
        double datarate = tokens[4] != "-" ? atof(tokens[4].c_str()) : -1;
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

void NetBuilder::generateConnectionCDNRandom(std::map<long ,cModule*> & nodeid2mod)
{
	long srcnodeid;
	long destnodeid;
	double datarate;
	double delay = 0.01;
	double error = 0;

	for(int var = 0;var < totalModule;++var){
		if(var < numberRouter){
			srcnodeid = var;
			destnodeid = (long) uniform(0, numberRouter, (int) dblrand()*1e6);
			destnodeid = (destnodeid == var)?var+1:destnodeid;
			datarate = uniform(1e6, 1e10, (int) dblrand()*1e6);
		}else
			if(var < numberRouter + numberStorage){
				srcnodeid = var;
				destnodeid = (long) uniform(0, numberRouter, (int) dblrand()*1e6);
				datarate = 1e9;
			}else
				if(var < numberRouter + numberStorage + numberRefletor){
					srcnodeid = var;
					// each storage connect with one router
					destnodeid = var - (numberRouter + numberStorage);
					datarate = 1e9;
				}else
					if(var < numberRouter + numberStorage + numberRefletor + numberIndexador){
						srcnodeid = var;
						destnodeid = (long) uniform(0, numberRouter, (int) dblrand()*1e6);
						datarate = 1e9;
					}else
						if(var < numberRouter + numberStorage + numberRefletor + numberIndexador + numberProcessador){
							srcnodeid = var;
							destnodeid = (long) uniform(0, numberRouter, (int) dblrand()*1e6);
							datarate = 1e9;
						}
		EV << srcnodeid << " " << destnodeid << endl;
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

void NetBuilder::buildNetwork(cModule *parent)
{
    nodeid2mod = generateModuleCDNDyn(parent);
    generateConnectionCDNRandom(nodeid2mod);

    std::map<long,cModule *>::iterator it;

    // final touches: buildinside, initialize()
    for (it=nodeid2mod.begin(); it!=nodeid2mod.end(); ++it)
    {
        cModule *mod = it->second;
        mod->buildInside();
    }

    // multi-stage init
    bool more = true;
    for (int stage=0; more; stage++) {
        more = false;
        for (it=nodeid2mod.begin(); it!=nodeid2mod.end(); ++it) {
            cModule *mod = it->second;
            if (mod->callInitialize(stage))
                more = true;
        }
    }

    // Connect client
    connectClient(parent);
}


void NetBuilder::connectClient(cModule *parent){
	numberClient = par("numberClient").longValue();
	for(int var = totalModule;var < numberClient+totalModule;++var){
		cModuleType *modtype = cModuleType::get("src.cdn.node.CDNNode");
		if(!modtype){
			throw cRuntimeError("module type `%s' for node `%d' not found", "src.cdn.node.CDNNode", var);
		}

		std::stringstream number;
		number << var;

		cModule *mod = modtype->create("client", parent);
		mod->setName(std::string("client").append(number.str()).c_str());
        mod->par("numUdpApps").setLongValue(1000);
        mod->par("udpAppType").setStringValue("Client");

		nodeid2mod[var] = mod;
		// read params from the ini file, etc
		mod->finalizeParameters();

		long srcnodeid = var;
		long destnodeid = (long) uniform(0, numberRouter, (int) dblrand()*1e6);
		double datarate = 1e6;
		double delay = 0.01;
		double error = 0;
		EV << srcnodeid << " " << destnodeid << endl;
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
