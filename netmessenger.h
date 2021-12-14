#ifndef NETMESSENGER_H
#define NETMESSENGER_H

#include "netmessage.h"
#include<queue>

class NetMessenger
{
public:
    NetMessenger();

private:
    queue<NetMessage> messageQueue;
};

#endif // NETMESSENGER_H
