/*
 * simple_destination.cc
 *
 *  Created on: May 16, 2020
 *      Author: Dell
 */

#include <stdio.h>
#include <string.h>
#include <omnetpp.h>
#include <queue>
#include "msg_makeby_host_m.h"

using namespace omnetpp;
using namespace std;

class SimpleDestination : public cSimpleModule
{
  private:
    double SIMULATOR_TIME;
    double INTERVAL_TIME;
    int count = 0;

    int *receivedMsgCount;
    int intervalIndex = 0;
    int receivedMsgCountLength;

  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void finish() override;
};

Define_Module(SimpleDestination);

void SimpleDestination::initialize()
{

    INTERVAL_TIME = getParentModule()->par("MSG_GEN_INTERVAL");
    SIMULATOR_TIME = getParentModule()->par("SIMULATOR_TIME");

    receivedMsgCountLength = SIMULATOR_TIME / INTERVAL_TIME;
    receivedMsgCount = new int[receivedMsgCountLength];

    memset(receivedMsgCount, 0, receivedMsgCountLength * sizeof(int));
    scheduleAt(0.000, new cMessage("nextInterval"));

}

void SimpleDestination::handleMessage(cMessage *msg)
{
    if (simTime() >= SIMULATOR_TIME){
         return;
    }

    char temp[15];
    strncpy(temp, msg->getName(), 11);
    temp[11] = 0;

    if (strcmp(temp, "MsgFromHost") == 0) {
                EV << "Received Msg from Host" << endl;
                send(new cMessage("NotifyReceived"), "out");
                count++;
                receivedMsgCount[intervalIndex]++;
                delete(msg);
    }
    if (strcmp(msg->getName(), "nextInterval") == 0) {
         intervalIndex++;
         scheduleAt(simTime() + INTERVAL_TIME, msg);
    }

}
void SimpleDestination::finish(){

    for(int i=0; i < receivedMsgCountLength; i++){
        EV<< "Count in "<< i <<":"<< receivedMsgCount[i] << endl;
    }

    EV<< count;

}


