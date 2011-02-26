#include <iostream>
#include <fstream>
#include <vector>
#include <string.h>
#include <string>
#include <sstream>
#include <omnetpp.h>


/**
 * Builds a network dynamically, with the topology coming from a
 * text file.
 */
class NetBuilder : public cSimpleModule
{
  protected:
    void connect(cGate *src, cGate *dest, double delay, double ber, double datarate);
    void buildNetwork(cModule *parent);
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);
private:
    std::map<long ,cModule*> generateModuleCDNStatic(cModule *parent);
    void generateConnectionCDNStatic(std::map<long ,cModule*> & nodeid2mod);
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

std::map<long ,cModule*> NetBuilder::generateModuleCDNStatic(cModule *parent)
{
    //number of CDN elements
	int numberRouter = par("numberRouter").longValue();
    int numberStorage = par("numberStorage").longValue();
    int numberRefletor = par("numberRefletor").longValue();
    int numberIndexador = par("numberIndexador").longValue();
    int numberProcessador = par("numberProcessador").longValue();

    std::map<long ,cModule*> nodeid2mod;

    long totalModule = numberRouter + numberStorage + numberRefletor + numberIndexador + numberProcessador;

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
                displayString = "i=device/server";
            }else
                if(var < numberRouter + numberStorage + numberRefletor){
                    mod = modtype->create("refletor", parent);
                    mod->setName(std::string("refletor").append(number.str()).c_str());
                    displayString = "i=abstract/db";
                }else
                    if(var < numberRouter + numberStorage + numberRefletor + numberIndexador){
                        mod = modtype->create("indexador", parent);
                        mod->setName(std::string("indexador").append(number.str()).c_str());
                        displayString = "i=block/network2";
                    }else
                        if(var < numberRouter + numberStorage + numberRefletor + numberIndexador + numberProcessador){
                            mod = modtype->create("processador", parent);
                            mod->setName(std::string("processador").append(number.str()).c_str());
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

void NetBuilder::buildNetwork(cModule *parent)
{
    std::map<long ,cModule*> nodeid2mod = generateModuleCDNStatic(parent);
    generateConnectionCDNStatic(nodeid2mod);
    //Dynamic

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

}


