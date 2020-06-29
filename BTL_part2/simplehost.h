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
#include "msg_makeby_host_m.h"

using namespace omnetpp;
using namespace std;


class SimpleSource : public cSimpleModule
{
  private:
    int count;
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

  public:
    void incNumSpacesOfNextENB();
};



#endif /* SIMPLEHOST_H_ */
