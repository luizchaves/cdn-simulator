package src.cdn.nodeApp;

import inet.applications.udpapp.UDPApp;


simple Processor like UDPApp
{
    parameters:
        //int serverPort; // port to listen on
        //volatile double waitInterval @unit("s"); // interval between sending video stream packets
        //volatile int packetLen @unit("B");
        @display("i=block/app");

    gates:
        input udpIn @labels(UDPControlInfo/up);
        output udpOut @labels(UDPControlInfo/down);
}
