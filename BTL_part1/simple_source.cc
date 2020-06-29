#include <stdio.h>
#include <string.h>
#include <omnetpp.h>
#include <queue>
#include "msg_part1_m.h"

using namespace omnetpp;
using namespace std;


class SimpleSource : public cSimpleModule
{
  private:
    int EXB_SIZE;
    double SIMULATOR_TIME;
    double MSG_GEN_INTERVAL;
    double CHANNEL_DELAY = 0.015;
    queue<int> sourceQueue;
    queue<int> exitBuffer;
    int lastMsgId = -1;
    void SQtoEXB();
    void sendMsg();

  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
};

Define_Module(SimpleSource);

void SimpleSource::initialize()
{
    MSG_GEN_INTERVAL = getParentModule()->par("MSG_GEN_INTERVAL");
    SIMULATOR_TIME = getParentModule()->par("SIMULATOR_TIME");
    EXB_SIZE = par("EXB_SIZE");

    scheduleAt(0, new cMessage("generate"));

    scheduleAt(0, new cMessage("send"));
}

void SimpleSource::handleMessage(cMessage *msg)
{
    if (simTime() >= SIMULATOR_TIME){
        delete(msg);
        return;
    }

    if (strcmp(msg->getName(), "generate") == 0) {

            sourceQueue.push(++lastMsgId);
            EV<< "Message generated" << endl;
            scheduleAt(simTime() + MSG_GEN_INTERVAL, msg);

            if (exitBuffer.size() < EXB_SIZE) {
                SQtoEXB();
            }


    }
    if (strcmp(msg->getName(), "send") == 0) {

            sendMsg();
            SQtoEXB();

            scheduleAt(simTime() + CHANNEL_DELAY, msg);
    }
}

void SimpleSource::SQtoEXB ()
{
    if (!sourceQueue.empty()){
            int msgId = sourceQueue.front();
            sourceQueue.pop();
            exitBuffer.push(msgId);

    }
}

void SimpleSource::sendMsg(){
    if (!exitBuffer.empty()) {

        int sentMsgId = exitBuffer.front();
        exitBuffer.pop();

        SimpleMessage *sentMsg = new SimpleMessage("testMsg");
        sentMsg->setId(sentMsgId);
        send(sentMsg, "out");
    }
}


