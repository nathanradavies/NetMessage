/*
 * Copyright © 2021 Nathan R. A. Davies
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the “Software”), to deal in the
 * Software without restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the
 * Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
 * PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#ifndef NETMESSENGER_H
#define NETMESSENGER_H

#include "netmessage.h"
#include <queue>
#include <map>
#include <mutex>
#include <thread>
#include <sys/socket.h>

class NetMessenger
{
public:
    NetMessenger();
    NetMessage pop();
    bool sendBroadcast(NetMessage m);
    bool sendDirect(string clientId, NetMessage m);
    bool isMessageWaiting();
    bool addClient(string clientName, string ipAddress);
    bool dropClient(string clientName);

private:
    queue<NetMessage> messageQueue;
    map<string, string> clients;
    std::mutex messageQueueMutex;
    mutex clientsMutex;
    void networkLoop();
    thread *networkThread;
};

#endif // NETMESSENGER_H
