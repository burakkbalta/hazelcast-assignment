#include <iostream>
#include <iterator>
#include <algorithm>
#include <vector>
#include <utility>

#include "messages.hh"

std::pair<std::string, unsigned int> splitIpPortString(const std::string& ipPortStr, const std::string& ipPortDelimeter) {
    size_t pos{ipPortStr.find(ipPortDelimeter)};

    std::string ip{};
    unsigned int port{};
    if(pos != std::string::npos) {
        ip = ipPortStr.substr(0,pos);
        port =  static_cast<unsigned int>(stoi(ipPortStr.substr(pos + ipPortDelimeter.size(), ipPortStr.size())));
    }

    return std::make_pair(ip,port);
}

int main(int argc, char* argv[])
{
    const std::string argumentNameLocalAdress{"--local_address"};
    const std::string argumentNameRemoteAdresses{"--remote_addresses"};

    std::vector<std::string> args(argv + 1, argv + argc);
    std::string localAddressPort, remoteAddressesAndPorts;
    std::string message, messageParameter;
    CommandLineArguments cmArguments;

    for (auto i = args.begin(); i != args.end(); ++i) {
        if (*i == argumentNameLocalAdress) {
            localAddressPort = *++i;
        } else if (*i == argumentNameRemoteAdresses) {
            remoteAddressesAndPorts = *++i;
        } else if ((*i).find("-message") != std::string::npos) {
            message = *i++;
            messageParameter = *i++;
        }
    }

    std::string ipPortDelimeter{":"};
    std::string remoteAddressDelimeter{","};

    auto splittedStringPair{std::move(splitIpPortString(localAddressPort, ipPortDelimeter))};
    cmArguments.localIp = splittedStringPair.first;
    cmArguments.localPort = splittedStringPair.second;

    size_t pos{0};
    while((pos = remoteAddressesAndPorts.find(remoteAddressDelimeter)) != std::string::npos) {
        std::string remoteAddressAndPort{remoteAddressesAndPorts.substr(0, pos)};
        splittedStringPair = std::move(splitIpPortString(remoteAddressAndPort, ipPortDelimeter));
        cmArguments.remoteAddresses.push_back(splittedStringPair);
        remoteAddressesAndPorts.erase(0, pos + remoteAddressDelimeter.length());
    }

    cmArguments.messageType = message;
    cmArguments.messageParameter = messageParameter;

    return 0;
}
