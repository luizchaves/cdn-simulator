package src.cdn.builder.old;

import inet.nodes.inet.Router;
import src.cdn.node.CDNNode;
import ned.DatarateChannel;

network Network
{
    
    parameters:
        int numberStorage = default(1);
        int numberRefletor = default(27);
        int numberIndexador = default(1);
        int numberProcessador = default(1);
        int numberPOPCDN = default(27);
        int numberPOPClient = default(27);
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
        routers[27].pppg++ <--> fiberline6M <--> routers[33].pppg++;
        routers[28].pppg++ <--> fiberline34M <--> routers[47].pppg++;
        routers[29].pppg++ <--> fiberline20M <--> routers[51].pppg++;
        routers[30].pppg++ <--> fiberline20M <--> routers[51].pppg++;
        routers[31].pppg++ <--> fiberline2G <--> routers[43].pppg++;
        routers[31].pppg++ <--> fiberline2G <--> routers[47].pppg++;
        routers[32].pppg++ <--> fiberline2G <--> routers[39].pppg++;
        routers[32].pppg++ <--> fiberline2G <--> routers[43].pppg++;
        routers[33].pppg++ <--> fiberline6M <--> routers[27].pppg++;
        routers[33].pppg++ <--> fiberline622M <--> routers[35].pppg++;
        routers[33].pppg++ <--> fiberline155M <--> routers[37].pppg++;
        routers[33].pppg++ <--> fiberline155M <--> routers[38].pppg++;
        routers[33].pppg++ <--> fiberline10G <--> routers[39].pppg++;
        routers[33].pppg++ <--> fiberline6M <--> routers[46].pppg++;
        routers[33].pppg++ <--> fiberline2G <--> routers[49].pppg++;
        routers[33].pppg++ <--> fiberline10G <--> routers[51].pppg++;
        routers[33].pppg++ <--> fiberline34M <--> routers[53].pppg++;
        routers[33].pppg++ <--> fiberline10G <--> routers[56].pppg++;
        routers[34].pppg++ <--> fiberline155M <--> routers[47].pppg++;
        routers[35].pppg++ <--> fiberline622M <--> routers[33].pppg++;
        routers[36].pppg++ <--> fiberline155M <--> routers[47].pppg++;
        routers[37].pppg++ <--> fiberline155M <--> routers[33].pppg++;
        routers[38].pppg++ <--> fiberline155M <--> routers[33].pppg++;
        routers[39].pppg++ <--> fiberline2G <--> routers[32].pppg++;
        routers[39].pppg++ <--> fiberline10G <--> routers[33].pppg++;
        routers[39].pppg++ <--> fiberline10G <--> routers[47].pppg++;
        routers[40].pppg++ <--> fiberline20M <--> routers[47].pppg++;
        routers[41].pppg++ <--> fiberline155M <--> routers[43].pppg++;
        routers[42].pppg++ <--> fiberline2G <--> routers[50].pppg++;
        routers[42].pppg++ <--> fiberline2G <--> routers[51].pppg++;
        routers[43].pppg++ <--> fiberline2G <--> routers[32].pppg++;
        routers[43].pppg++ <--> fiberline2G <--> routers[31].pppg++;
        routers[43].pppg++ <--> fiberline155M <--> routers[41].pppg++;
        routers[43].pppg++ <--> fiberline155M <--> routers[48].pppg++;
        routers[44].pppg++ <--> fiberline34M <--> routers[47].pppg++;
        routers[45].pppg++ <--> fiberline6M <--> routers[51].pppg++;
        routers[46].pppg++ <--> fiberline6M <--> routers[33].pppg++;
        routers[47].pppg++ <--> fiberline34M <--> routers[28].pppg++;
        routers[47].pppg++ <--> fiberline2G <--> routers[31].pppg++;
        routers[47].pppg++ <--> fiberline34M <--> routers[34].pppg++;
        routers[47].pppg++ <--> fiberline155M <--> routers[36].pppg++;
        routers[47].pppg++ <--> fiberline10G <--> routers[39].pppg++;
        routers[47].pppg++ <--> fiberline20M <--> routers[40].pppg++;
        routers[47].pppg++ <--> fiberline34M <--> routers[44].pppg++;
        routers[47].pppg++ <--> fiberline10G <--> routers[51].pppg++;
        routers[47].pppg++ <--> fiberline34M <--> routers[52].pppg++;
        routers[48].pppg++ <--> fiberline155M <--> routers[43].pppg++;
        routers[49].pppg++ <--> fiberline2G <--> routers[33].pppg++;
        routers[49].pppg++ <--> fiberline2G <--> routers[50].pppg++;
        routers[50].pppg++ <--> fiberline2G <--> routers[42].pppg++;
        routers[50].pppg++ <--> fiberline2G <--> routers[49].pppg++;
        routers[51].pppg++ <--> fiberline20M <--> routers[29].pppg++;
        routers[51].pppg++ <--> fiberline20M <--> routers[30].pppg++;
        routers[51].pppg++ <--> fiberline10G <--> routers[33].pppg++;
        routers[51].pppg++ <--> fiberline2G <--> routers[42].pppg++;
        routers[51].pppg++ <--> fiberline6M <--> routers[45].pppg++;
        routers[51].pppg++ <--> fiberline10G <--> routers[47].pppg++;
        routers[52].pppg++ <--> fiberline34M <--> routers[47].pppg++;
        routers[53].pppg++ <--> fiberline34M <--> routers[33].pppg++;
        routers[54].pppg++ <--> fiberline10G <--> routers[24].pppg++;
        routers[54].pppg++ <--> fiberline10G <--> routers[55].pppg++;
		routers[55].pppg++ <--> fiberline10G <--> routers[56].pppg++;
}
