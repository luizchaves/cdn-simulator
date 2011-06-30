package src.cdn.nodeApp;

import inet.transport.tcp.TCP;
import inet.util.TCPDump;
import inet.util.NAMTraceWriter;
import inet.transport.udp.UDP;
import inet.transport.tcp.ITCP;
import inet.transport.sctp.SCTP;
import inet.nodes.inet.NetworkLayer;
import inet.networklayer.ipv4.RoutingTable;
import inet.networklayer.common.InterfaceTable;
import inet.linklayer.ppp.PPPInterface;
import inet.linklayer.ext.ExtInterface;
import inet.linklayer.ethernet.EthernetInterface;
import inet.base.NotificationBoard;
import inet.applications.udpapp.UDPApp;
import inet.applications.tcpapp.TCPApp;
import inet.applications.sctpapp.SCTPApp;
import inet.applications.pingapp.PingApp;


module CDNNode
{
    parameters:
        @node();
        @labels(node,ethernet-node);
        @display("i=device/pc2;bgl=2");
        int numUdpApps = default(0);
        int numTcpApps = default(0);
        string udpAppType = default("n/a");
        string tcpAppType = default("n/a");
        string type = default("c");//c - client; r - refletor; s - storage; i - indexador; p - processador
        //string tcpType = default("TCP");  // tcp implementation (e.g. TCP, TCP_old, TCP_NSC)
    gates:
        inout pppg[] @labels(PPPFrame-conn);
        inout ethg[] @labels(EtherFrame-conn);
    submodules:
        notificationBoard: NotificationBoard {
            parameters:
                @display("p=73,54");
        }
        interfaceTable: InterfaceTable {
            parameters:
                @display("p=73,134");
        }
        routingTable: RoutingTable {
            parameters:
                routerId = "";
                @display("p=73,214");
        }
        udpApp[numUdpApps]: <udpAppType> like UDPApp {
            parameters:
                @display("p=222,54");
        }
        udp: UDP {
            parameters:
                @display("p=222,141");
        }
        networkLayer: NetworkLayer {
            parameters:
                proxyARP = false;
                @display("p=314,214;q=queue");
            gates:
                ifIn[sizeof(pppg)+sizeof(ethg)];
                ifOut[sizeof(pppg)+sizeof(ethg)];
        }
        ppp[sizeof(pppg)]: PPPInterface {
            parameters:
                @display("p=222,310,row,90;q=txQueue");
        }
        eth[sizeof(ethg)]: EthernetInterface {
            parameters:
                @display("p=389,310,row,90;q=txQueue");
        }
        tcpApp[numTcpApps]: <tcpAppType> like TCPApp {
            @display("p=389,54");
        }
        tcp: TCP {
            @display("p=389,141");
        }
    connections allowunconnected:
        for i=0..numUdpApps-1 {
            udpApp[i].udpOut --> udp.appIn++;
            udpApp[i].udpIn <-- udp.appOut++;
        }
        
        for i=0..numTcpApps-1 {
            tcpApp[i].tcpOut --> tcp.appIn++;
            tcpApp[i].tcpIn <-- tcp.appOut++;
        }
        
        udp.ipOut --> networkLayer.udpIn;
        udp.ipIn <-- networkLayer.udpOut;
        
        networkLayer.tcpOut --> tcp.ipIn;
        tcp.ipOut --> networkLayer.tcpIn;
        

        // connections to network outside
        for i=0..sizeof(pppg)-1 {
            pppg[i] <--> ppp[i].phys;
            ppp[i].netwOut --> networkLayer.ifIn[i];
            ppp[i].netwIn <-- networkLayer.ifOut[i];
        }


        for i=0..sizeof(ethg)-1 {
            ethg[i] <--> eth[i].phys;
            eth[i].netwOut --> networkLayer.ifIn[sizeof(pppg)+i];
            eth[i].netwIn <-- networkLayer.ifOut[sizeof(pppg)+i];
        }

        
}


