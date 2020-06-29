/*
 * simplehost.h
 *
 *  Created on: Jun 10, 2020
 *      Author: Dell
 */

#ifndef SIMPLEHOST_H_
#define SIMPLEHOST_H_

#include <stdio.h>
#include <string.h>
#include <omnetpp.h>
#include <queue>
#include <message_m.h>

using namespace omnetpp;
using namespace std;


class SimpleHost : public cSimpleModule
{
  private:
    int count;
    int EXB_SIZE;
    double SIMULATOR_TIME;
    double MSG_GEN_INTERVAL;
    double CHANNEL_DELAY = 0.0001;
    queue<int> sourceQueue;
    queue<int> exitBuffer;
    int lastMsgId = -1;
    void SQtoEXB();
    void sendMsg();
    int k;


    double INTERVAL_TIME;
    int countMsg = 0;

    int *receivedMsgCount;
    int intervalIndex = 0;
    int receivedMsgCountLength;


  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void finish() override;

  public:
    void incNumSpacesOfNextENB();
};



#endif /* SIMPLEHOST_H_ */
