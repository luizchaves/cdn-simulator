package src.cdn.builder.old;

import inet.nodes.inet.Router;
import src.cdn.nodeApp.CDNNode;
import ned.DatarateChannel;

//types:
channel fiberline extends DatarateChannel
{
    parameters:
        delay = 0.1us;
        datarate = 100Mbps;
}
channel fiberline10G extends DatarateChannel
{
    parameters:
        delay = 0.1us;
        datarate = 10Gbps;
}
channel fiberline2G extends DatarateChannel
{
    parameters:
        delay = 0.1us;
        datarate = 2.5Gbps;
}
channel fiberline622M extends DatarateChannel
{
    parameters:
        delay = 0.1us;
        datarate = 622Mbps;
}
channel fiberline155M extends DatarateChannel
{
    parameters:
        delay = 0.1us;
        datarate = 155Mbps;
}
channel fiberline257M extends DatarateChannel
{
    parameters:
        delay = 0.1us;
        datarate = 257Mbps;
}
channel fiberline34M extends DatarateChannel
{
    parameters:
        delay = 0.1us;
        datarate = 34Mbps;
}
channel fiberline20M extends DatarateChannel
{
    parameters:
        delay = 0.1us;
        datarate = 2Mbps;
}
channel fiberline6M extends DatarateChannel
{
    parameters:
        delay = 0.1us;
        datarate = 6Mbps;
}

network NetworkOld
{
    
    parameters:
        int numberStorage = default(1);
        int numberRefletor = default(27);
        int numberIndexador = default(1);
        int numberProcessador = default(1);
        int numberPOPCDN = default(27);
        int numberClient = default(1);
        @display("bgl=2");
    submodules:
        routers[numberPOPCDN+3+numberPOPClient]: Router;
        storages[numberStorage]: CDNNode {
            @display("i=device/server");
        }
        refletors[numberRefletor]: CDNNode{
            @display("i=abstract/db");
        }
        indexadors[numberIndexador]: CDNNode{
            @display("i=block/network2");
        }
        processadors[numberProcessador]: CDNNode{
            @display("i=device/cpu");
        }
        clients[numberClient]: CDNNode;
    connections allowunconnected:
        //Link Routers POPCDN and Refletor
        routers[0].pppg++ <--> fiberline <--> storages[0].pppg++;
        //Link Routers POPCDN and Refletor
        routers[0].pppg++ <--> fiberline <--> indexadors[0].pppg++;
        //Link Routers POPCDN and Refletor
        routers[0].pppg++ <--> fiberline <--> processadors[0].pppg++;
        //Link Routers POPCDN and Refletor
        for i = 0..numberPOPCDN-1 {
            routers[i].pppg++ <--> fiberline <--> refletors[i].pppg++;
        }
        //Link Routers POPClient and POPCDN
        routers[0].pppg++ <--> fiberline6M <--> routers[6].pppg++;
		routers[1].pppg++ <--> fiberline34M <--> routers[20].pppg++;
		routers[2].pppg++ <--> fiberline20M <--> routers[24].pppg++;
		routers[3].pppg++ <--> fiberline20M <--> routers[24].pppg++;
        routers[4].pppg++ <--> fiberline2G <--> routers[16].pppg++;
        routers[4].pppg++ <--> fiberline20M <--> routers[20].pppg++;
        routers[5].pppg++ <--> fiberline2G <--> routers[12].pppg++;
        routers[5].pppg++ <--> fiberline2G <--> routers[16].pppg++;
        routers[6].pppg++ <--> fiberline6M <--> routers[0].pppg++;
        routers[6].pppg++ <--> fiberline622M <--> routers[8].pppg++;
        routers[6].pppg++ <--> fiberline155M <--> routers[10].pppg++;
        routers[6].pppg++ <--> fiberline155M <--> routers[11].pppg++;
        routers[6].pppg++ <--> fiberline10G <--> routers[12].pppg++;
        routers[6].pppg++ <--> fiberline6M <--> routers[19].pppg++;
        routers[6].pppg++ <--> fiberline2G <--> routers[22].pppg++;
        routers[6].pppg++ <--> fiberline10G <--> routers[24].pppg++;
        routers[6].pppg++ <--> fiberline34M <--> routers[26].pppg++;
        routers[7].pppg++ <--> fiberline155M <--> routers[20].pppg++;
        routers[8].pppg++ <--> fiberline622M <--> routers[6].pppg++;
        routers[9].pppg++ <--> fiberline155M <--> routers[20].pppg++;
        routers[10].pppg++ <--> fiberline155M <--> routers[6].pppg++;
        routers[11].pppg++ <--> fiberline155M <--> routers[6].pppg++;
        routers[12].pppg++ <--> fiberline2G <--> routers[5].pppg++;
        routers[12].pppg++ <--> fiberline10G <--> routers[6].pppg++;
        routers[12].pppg++ <--> fiberline10G <--> routers[20].pppg++;
        routers[13].pppg++ <--> fiberline20M <--> routers[20].pppg++;
        routers[14].pppg++ <--> fiberline155M <--> routers[16].pppg++;
        routers[15].pppg++ <--> fiberline2G <--> routers[23].pppg++;
        routers[15].pppg++ <--> fiberline2G <--> routers[24].pppg++;
        routers[16].pppg++ <--> fiberline2G <--> routers[5].pppg++;
        routers[16].pppg++ <--> fiberline2G <--> routers[4].pppg++;
        routers[16].pppg++ <--> fiberline155M <--> routers[14].pppg++;
        routers[16].pppg++ <--> fiberline155M <--> routers[21].pppg++;
        routers[17].pppg++ <--> fiberline34M <--> routers[20].pppg++;
        routers[18].pppg++ <--> fiberline6M <--> routers[24].pppg++;
        routers[19].pppg++ <--> fiberline6M <--> routers[6].pppg++;
        routers[20].pppg++ <--> fiberline155M <--> routers[1].pppg++;
        routers[20].pppg++ <--> fiberline2G <--> routers[4].pppg++;
        routers[20].pppg++ <--> fiberline155M <--> routers[7].pppg++;
        routers[20].pppg++ <--> fiberline155M <--> routers[9].pppg++;
        routers[20].pppg++ <--> fiberline10G <--> routers[12].pppg++;
        routers[20].pppg++ <--> fiberline20M <--> routers[13].pppg++;
        routers[20].pppg++ <--> fiberline34M <--> routers[17].pppg++;
        routers[20].pppg++ <--> fiberline10G <--> routers[24].pppg++;
        routers[20].pppg++ <--> fiberline2G <--> routers[25].pppg++;
        routers[21].pppg++ <--> fiberline155M <--> routers[16].pppg++;
        routers[22].pppg++ <--> fiberline2G <--> routers[6].pppg++;
        routers[22].pppg++ <--> fiberline2G <--> routers[23].pppg++;
        routers[23].pppg++ <--> fiberline2G <--> routers[15].pppg++;
        routers[23].pppg++ <--> fiberline2G <--> routers[22].pppg++;
        routers[24].pppg++ <--> fiberline20M <--> routers[2].pppg++;
        routers[24].pppg++ <--> fiberline20M <--> routers[3].pppg++;
        routers[24].pppg++ <--> fiberline6M <--> routers[6].pppg++;
        routers[24].pppg++ <--> fiberline2G <--> routers[15].pppg++;
        routers[24].pppg++ <--> fiberline6M <--> routers[18].pppg++;
        routers[24].pppg++ <--> fiberline20M <--> routers[20].pppg++;
        routers[24].pppg++ <--> fiberline10G <--> routers[54].pppg++;
        routers[25].pppg++ <--> fiberline34M <--> routers[20].pppg++;
        routers[26].pppg++ <--> fiberline34M <--> routers[6].pppg++;
}
