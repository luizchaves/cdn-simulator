//
// This file is part of an OMNeT++/OMNEST simulation example.
//
// Copyright (C) 1992-2008 Andras Varga
//
// This file is distributed WITHOUT ANY WARRANTY. See the file
// `license' for details on this and other legal matters.
//

package src.cdn.builder;

//
// Dynamically sets up a network, with the topology coming from text files
//
simple NetBuilder
{
    parameters:
        @display("i=block/cogwheel_s");
        string nodesFile;
        string connectionsFile;
        int numberRouter = default(27);
        int numberStorage = default(1);
        int numberRefletor = default(27);
        int numberIndexador = default(1);
        int numberProcessador = default(1);
        int numberClient = default(1);
}
