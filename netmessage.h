#ifndef NETMESSAGE_H
#define NETMESSAGE_H

#include<string>
#include<vector>

using namespace std;


class NetMessage
{
public:
    NetMessage();

private:
    vector<byte> messageData;
    vector<byte> magicData;
};

#endif // NETMESSAGE_H
