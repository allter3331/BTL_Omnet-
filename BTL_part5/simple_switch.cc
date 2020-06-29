
#include <stdio.h>

#include <string.h>
#include <omnetpp.h>
#include <queue>
#include <algorithm>
#include "message_m.h"
#include "simplehost.h"
#include "notify_host_m.h"
#include "my_routing.h"
#include <string>
#include <simple_switch.h>

using namespace omnetpp;
using namespace std;

Define_Module(SimpleSwitch);

void SimpleSwitch::initialize()
{
    k = getParentModule()->par("k");
    index = getId()-2;

    if(getId() - 2 == k*k/4 ){
        initAdj(k);
        for(int i=0; i < k*k*k/4 + k*k + k*k/4; i++){
            buildAddress(i, k);
            buildTable(i, k);
        }
    }


    OPERATION_CYCLE_TIME = getParentModule()->par("OPERATION_CYCLE_TIME_SWITCH");
    SIMULATOR_TIME = getParentModule()->par("SIMULATOR_TIME");
    EXB_SIZE = getParentModule()->par("EXB_SIZE");
    CREDIT_DELAY = getParentModule()->par("CREDIT_DELAY");

    entranceBuffer = new queue<MessageMakeByHost*>[k];
    exitBuffer = new queue<MessageMakeByHost*>[k];

    countEmptyOFNextENB = new int[k];
    for(int i = 0; i < k; i++){
        countEmptyOFNextENB[i] = EXB_SIZE;
    }

    isBusyOnChanel = new int[k];
    for(int i = 0; i < k; i++){
            isBusyOnChanel[i] = 0;
    }

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

        int idPreviosNode = ttmsg->getPreviosNodeIndex();

        int previosPort = idDesToPort(index, idPreviosNode, k);

        // previosPort = Index of ENB


        entranceBuffer[previosPort].push(ttmsg);

    }



    if (strcmp(msg->getName(), "ENBtoEXB") == 0) {

        int isUsed[k];
        for(int i = 0; i < k; i++){
           isUsed[i] = 0;
        }
        //for each EXB


        std::vector<int> dess;
        for(int j = 0; j< k; j++){
            dess.push_back(j);
        }

        std::random_shuffle(dess.begin(), dess.end());
        for(vector<int>::iterator it = dess.begin() ; it != dess.end(); ++it){

        //for(int j = k-1; j >= 0; j--){
            int j = *it;
            int indexOfENBChoseLast = -1;
            int min = 1000000;


            // for each ENB
            for(int i=0; i < k; i++){
                if(isUsed[i] != 0) continue;

                if ( !entranceBuffer[i].empty() ){

                   MessageMakeByHost* tempMsg = entranceBuffer[i].front();
                   int src = tempMsg->getSource();
                   int des = tempMsg->getDestination();
                   int idOfNextHop = next(src, index , des, k);
                   int nextPort = idDesToPort(index, idOfNextHop, k);

                   if(tempMsg->getId() < min && nextPort == j){
                       min = tempMsg->getId();
                       indexOfENBChoseLast = i;
                   }

                }
           }

           if(indexOfENBChoseLast != -1){

               isUsed[indexOfENBChoseLast] = 1;
              // find EXB for Mess in ENB
               //cout << "idENBChoseLast: " << indexOfENBChoseLast << " Of node"<< index << "on EXB " << j <<endl;
               MessageMakeByHost* tempMsg = entranceBuffer[indexOfENBChoseLast].front();
               int src = tempMsg->getSource();
               int des = tempMsg->getDestination();
               int idOfNextHop = next(src, index , des, k);
               int nextPort = idDesToPort(index, idOfNextHop, k);

               // nextPort is Index of EXB
               //cout<< getId() - 2 << ": nextPort: " << nextPort <<endl;

              if (exitBuffer[nextPort].size() < EXB_SIZE) {

                  exitBuffer[nextPort].push(entranceBuffer[indexOfENBChoseLast].front()) ;
                  entranceBuffer[indexOfENBChoseLast].pop();
                  //notify to Host increase count
                  // Time delay to send msg to Host is 1ms so that not necessarily to delay Credit_delay

                  MessageNotify *sentMsg = new MessageNotify("NotifyPreviosNode");
                          sentMsg->setIndexOfENBChoseLast(indexOfENBChoseLast);
                          sentMsg->setIdNodeIsNotifyed(tempMsg->getPreviosNodeIndex());

                  scheduleAt(simTime() + CREDIT_DELAY, sentMsg);

              }
           }
        }
        scheduleAt(simTime() + OPERATION_CYCLE_TIME, msg);
    }

    if (strcmp(msg->getName(), "NotifyPreviosNode") == 0) {

        MessageNotify *ttmsg = check_and_cast<MessageNotify *>(msg);

        int indexOfENBChoseLast = ttmsg->getIndexOfENBChoseLast();

        EV << "indexOfENBChoseLast : " << indexOfENBChoseLast << endl;
        int indexNodeIsNotifyed = ttmsg->getIdNodeIsNotifyed();

        char nameNode[20];

        sprintf_s(nameNode,20 , "node%d", indexNodeIsNotifyed);

        if(isServer(indexNodeIsNotifyed, k)){
            cModule* hostModule = getParentModule()->getSubmodule(nameNode);
            SimpleHost *source = check_and_cast<SimpleHost*>(hostModule);
            source->incNumSpacesOfNextENB();
            //cout << "source "<< indexNodeIsNotifyed <<" ->incNumSpacesOfNextENB();"<<endl;
        }else{

            int nextPortOfPreviosNode = idDesToPort(indexNodeIsNotifyed, index, k);
            cModule* switchModule = getParentModule()->getSubmodule(nameNode);
            SimpleSwitch *source = check_and_cast<SimpleSwitch*>(switchModule);

            source->incNumSpacesOfNextENB(nextPortOfPreviosNode);
        }


        delete msg;


    }

    if (strcmp(msg->getName(), "NotifyChanelFree") == 0) {
                int ENBID = msg->par("index").longValue();
                isBusyOnChanel[ENBID] = 0;
    }

    if (strcmp(msg->getName(), "send") == 0) {

        for(int j = 0; j< k; j++){
            if (!exitBuffer[j].empty() && countEmptyOFNextENB[j] > 0 && isBusyOnChanel[j] == 0) {

                MessageMakeByHost *sentMsg = exitBuffer[j].front();
                int source = sentMsg->getSource();
                int des = sentMsg->getDestination();
                sentMsg->setPreviosNodeIndex(index);
                exitBuffer[j].pop();

                // định tuyến lại tìm cổng out
                // có current index có next index ==> index of port out

                int idOfNextHop = next(source, index , des, k);
                int nextPort = idDesToPort(index, idOfNextHop, k);

                send(sentMsg, "gate$o", nextPort); // sent msg on gate-out to Des

                if (!isServer(idOfNextHop, k)) {
                    countEmptyOFNextENB[j]--;
                }

                isBusyOnChanel[j] = 1;

                cMessage *notifMsg = new cMessage("NotifyChanelFree");
                notifMsg->addPar("index").setLongValue(j);
                scheduleAt(simTime() + CHANNEL_DELAY, notifMsg);

            }

       }

       scheduleAt(simTime() + OPERATION_CYCLE_TIME, msg);
   }


}

void SimpleSwitch::incNumSpacesOfNextENB(int idENB){
    countEmptyOFNextENB[idENB]++;
}


