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
#include "netmessenger.h"

/*!
 * \brief NetMessenger::NetMessenger contructs a NetMessenger message manager object.
 */
NetMessenger::NetMessenger()
{
    // Launch network loop on it's own thread.
    networkThread = new thread([this] { networkLoop(); });
}

/*!
 * \brief NetMessenger::pop returns next network message from the queue, assuming there is one.
 * \return Returns a NetMessage object containing the next network message, or an empty one if there isn't one.
 */
NetMessage NetMessenger::pop()
{
    lock_guard<mutex> guard(messageQueueMutex);
    if (messageQueue.size()) {
        NetMessage m = messageQueue.front();
        messageQueue.pop();
        return m;
    }
    else {
        return NetMessage();
    }
}

bool NetMessenger::sendBroadcast(NetMessage m)
{
    return true; // TODO: Replace with actual value
}

bool NetMessenger::sendDirect(string clientId, NetMessage m)
{
    return true; // TODO: Replace with actual value
}

bool NetMessenger::isMessageWaiting()
{
    return true; // TODO: Replace with actual check
}

void NetMessenger::networkLoop()
{

}
