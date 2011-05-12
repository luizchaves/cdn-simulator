package src.cdn.builder.old;

//
// Dynamically sets up a network, with the topology coming from text files
//
simple NetBuilder
{
    parameters:
        @display("i=block/cogwheel_s");
        string connectionsFile;
        int numberRouter = default(27);
        int numberStorage = default(1);
        int numberRefletor = numberRouter;
        int numberIndexador = default(1);
        int numberProcessador = default(1);
        int numberClient = default(1);
}
