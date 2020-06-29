/*
 * simple_switch.h
 *
 *  Created on: Jun 21, 2020
 *      Author: Dell
 */

#ifndef SIMPLE_SWITCH_H_
#define SIMPLE_SWITCH_H_

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

using namespace omnetpp;
using namespace std;

class SimpleSwitch:public cSimpleModule
{
  private:
    int EXB_SIZE;
    double SIMULATOR_TIME;
    double OPERATION_CYCLE_TIME;
    double CHANNEL_DELAY = 0.0001; // = 100Kb / 1Gbps
    double CREDIT_DELAY ;
    int* countEmptyOFNextENB;
    int* isBusyOnChanel;
    int k;
    int index;

    queue<MessageMakeByHost*> *entranceBuffer;
    queue<MessageMakeByHost*> *exitBuffer;



    void ENBtoEXB();

  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;

  public:
      void incNumSpacesOfNextENB(int idENB);
};



#endif /* SIMPLE_SWITCH_H_ */
