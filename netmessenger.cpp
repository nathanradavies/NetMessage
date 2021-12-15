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

/**
 * @brief Construct a new Net Messenger:: Net Messenger object
 * 
 */
NetMessenger::NetMessenger()
{
    // Launch network loop on it's own thread.
    networkThread = new thread([this]
                               { networkLoop(); });
}

/**
 * @brief Get the oldest message from the queue.
 * 
 * @return NetMessage containing the oldest message in the queue.
 */
NetMessage NetMessenger::pop()
{
    lock_guard<mutex> guard(messageQueueMutex);
    if (messageQueue.size())
    {
        NetMessage m = messageQueue.front();
        messageQueue.pop();
        return m;
    }
    else
    {
        return NetMessage();
    }
}

/**
 * @brief Sends a NetMessage to all connected clients.
 * 
 * @param m The NetMessage to broadcast to all clients
 * @return true indicates that the message was broadcast to all clients successfully.
 * @return false indicates that the message failed to send to one or more clients
 */
bool NetMessenger::sendBroadcast(NetMessage m)
{
    return true; // TODO: Replace with actual value
}

/**
 * @brief Sends a NetMessage directly to a named client.
 * 
 * @param clientId The name of the client to send the message to.
 * @param m The NetMessage to send to the client.
 * @return true indicates that the message was sent correctly.
 * @return false indicates that the message was not sent, was send incorrectly, or otherwise failed.
 */
bool NetMessenger::sendDirect(string clientId, NetMessage m)
{
    return true; // TODO: Replace with actual value
}

/**
 * @brief Checks if there is a message waiting in the queue.
 * 
 * @return true indicates that there is a message in the queue
 * @return false indicates that there are no messages waiting in the queue
 */
bool NetMessenger::isMessageWaiting()
{
    lock_guard<mutex> quard(messageQueueMutex); // Make sure a commit is not in progress
    if (messageQueue.size())
    {
        return true;
    }
    else
    {
        return false;
    }
}

/**
 * @brief Private method containing the logic loop for the network connections.
 * 
 */
void NetMessenger::networkLoop()
{
}

/**
 * @brief Adds a client to the network manager
 * 
 * @param clientName A unique name for the client for convenience.
 * @param ipAddress The IPv4 address of the client in string format.
 * @return true indicates that the connection was added successfully.
 * @return false indicates that the connection failed to be added.
 */
bool NetMessenger::addClient(string clientName, string ipAddress)
{
    lock_guard<mutex> guard(clientsMutex);
    //clients.insert(clientName, ipAddress);
    // TODO: add client to tcp code
    return true;
}

/**
 * @brief Drops a client from the network manager
 * 
 * @param clientName The unique name of the client to be removed.
 * @return true indicates that the client was found and removed.
 * @return false indicates that the client was not found, or could not be removed.
 */
bool NetMessenger::dropClient(string clientName)
{
    lock_guard<mutex> guard(clientsMutex);
    clients.erase(clientName);
    // TODO: remove client from tcp code
    return true;
}