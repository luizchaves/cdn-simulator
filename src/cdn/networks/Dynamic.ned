//
// This file is part of an OMNeT++/OMNEST simulation example.
//
// Copyright (C) 1992-2008 Andras Varga
//
// This file is distributed WITHOUT ANY WARRANTY. See the file
// `license' for details on this and other legal matters.
//

package src.cdn.networks;

//import inet.networklayer.autorouting.FlatNetworkConfigurator;
import inet.nodes.inet.Router;
import inet.world.ChannelInstaller;
import src.cdn.node.CDNNode;
import src.cdn.builder.NetBuilderCDN;
import src.cdn.builder.NetConfigurator;
import ned.DatarateChannel;


network Dynamic
{
    submodules:
        builder: NetBuilderCDN {
            routersFile = "/home/lucachaves/program/omnetpp-4.1/samples/CDN/src/cdn/builder/configFile/routersRNP.txt";
            elementsFile = "/home/lucachaves/program/omnetpp-4.1/samples/CDN/src/cdn/builder/configFile/elements.txt";
            clientsFile = "/home/lucachaves/program/omnetpp-4.1/samples/CDN/src/cdn/builder/configFile/clients.txt";
            @display("p=52,62");
        }
        netConfigurator: NetConfigurator {
            @display("p=191,62");
        }
}


