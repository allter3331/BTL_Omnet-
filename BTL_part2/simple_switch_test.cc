#include <stdio.h>
#include <string.h>
#include <omnetpp.h>
#include <queue>
#include <algorithm>
#include "msg_makeby_host_m.h"
#include "simplehost.h"


using namespace omnetpp;
using namespace std;




class SimpleSwitch:public cSimpleModule
{
  private:
    int EXB_SIZE;
    double SIMULATOR_TIME;
    double OPERATION_CYCLE_TIME;
    double CHANNEL_DELAY = 0.015;
    int isBusy;

    queue<MessageMakeByHost*> *entranceBuffer;
    queue<MessageMakeByHost*> *exitBuffer;
    int numberOfSource;

    int indexOfENBChoseLast = -1;

    void ENBtoEXB();
    void sendMsg();

  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
};

Define_Module(SimpleSwitch);

void SimpleSwitch::initialize()
{
    OPERATION_CYCLE_TIME = getParentModule()->par("MSG_GEN_INTERVAL");
    SIMULATOR_TIME = getParentModule()->par("SIMULATOR_TIME");
    EXB_SIZE = getParentModule()->par("EXB_SIZE");

    numberOfSource = getParentModule()->par("numberOfSource");
    int numberOfDes = getParentModule()->par("numberOfDestination");
    isBusy = 0;

    entranceBuffer = new queue<MessageMakeByHost*>[numberOfSource];
    exitBuffer = new queue<MessageMakeByHost*>[numberOfDes];

    scheduleAt(0, new cMessage("ENBtoEXB"));

    scheduleAt(0, new cMessage("send"));
}

void SimpleSwitch::handleMessage(cMessage *msg)
{
    if (simTime() >= SIMULATOR_TIME){
        return;
    }

    char temp[12];
    strncpy(temp, msg->getName(), 11);
    temp[11] = 0;

    if (strcmp(temp, "MsgFromHost") == 0){
        MessageMakeByHost *ttmsg = check_and_cast<MessageMakeByHost *>(msg);
        entranceBuffer[ttmsg->getSource()].push(ttmsg);
    }

    if (strcmp(msg->getName(), "ENBtoEXB") == 0) {

        int min = 10000;
        // do có 1 des nên ta so sánh index của từng msg đầu tiên của ENB
        for(int i=0; i < numberOfSource; i++){
            if ( !entranceBuffer[i].empty() ){
                MessageMakeByHost* tempMsg = entranceBuffer[i].front();
                if(tempMsg->getId() < min ){
                    min = tempMsg->getId();
                    indexOfENBChoseLast = i;
                }

            }
        }

        if(indexOfENBChoseLast != -1){
            if (exitBuffer[0].size() < EXB_SIZE) {
                exitBuffer[0].push(entranceBuffer[indexOfENBChoseLast].front()) ;
                entranceBuffer[indexOfENBChoseLast].pop();
                //notify to Host increase count
                // Time delay to send msg to Host is 1ms so that not necessarily to delay Credit_delay

                scheduleAt(simTime() + 0.001, new cMessage("NotifyHost"));

            }
        }

        scheduleAt(simTime() + OPERATION_CYCLE_TIME, new cMessage("ENBtoEXB"));
    }

    if (strcmp(msg->getName(), "NotifyHost") == 0) {

        EV << "indexOfENBChoseLast : " << indexOfENBChoseLast << endl;
        cModule* switchModule = getParentModule()->getSubmodule("source", indexOfENBChoseLast);
        SimpleSource *source = check_and_cast<SimpleSource*>(switchModule);
        source->incNumSpacesOfNextENB();

        delete msg;


       }

    if (strcmp(msg->getName(), "NotifyReceived") == 0) {
        isBusy = 0;
    }

    if (strcmp(msg->getName(), "send") == 0) {
        if (!exitBuffer[0].empty() && isBusy == 0) {
                isBusy = 1;
                MessageMakeByHost *sentMsg = exitBuffer[0].front();
                exitBuffer[0].pop();
                send(sentMsg, "out", numberOfSource); // sent msg on gate-out to Des
                scheduleAt(simTime() + CHANNEL_DELAY, msg);
        }else
            scheduleAt(simTime() + OPERATION_CYCLE_TIME, msg);
   }
}


void SimpleSwitch::sendMsg(){
    if (!exitBuffer[0].empty() && isBusy == 0) {
        isBusy = 1;
        MessageMakeByHost *sentMsg = exitBuffer[0].front();
        exitBuffer[0].pop();

        send(sentMsg, "out", numberOfSource); // sent msg on gate-out to Des
    }


}


