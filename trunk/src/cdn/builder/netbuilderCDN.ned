package src.cdn.builder;

//
// Dynamically sets up a network, with the topology coming from text files
//
simple NetBuilderCDN
{
    parameters:
        @display("i=block/cogwheel_s");
        string routersFile;
        string elementsFile;
        string clientsFile;
}
