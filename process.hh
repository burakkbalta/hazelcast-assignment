#pragma once

#include "messageFactory.hh"
#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/bind.hpp>
#include <atomic>

class Process {
public:
    Process(const std::shared_ptr<MessageFactory>& mesFactory, const CommandLineArguments& commandLineArgs);
    ~Process();
    Process(const Process&) = delete;
    Process(Process&&) = delete;
    Process& operator==(const Process&) = delete;
    Process& operator==(Process&&) = delete;

private:
    static constexpr size_t numberOfThreads{40};
    void createAndStartThreads(const std::shared_ptr<BaseMessage>& sendingMessage);
    void createLocalSocketAndRemoteEndpoints(const std::string& localIp, unsigned int localPort, 
            std::vector<std::pair<std::string, unsigned int>>& remoteAddresses);
    void populateSendingMessage(const std::string& messageType, const std::string& messageParameter);
    void receiveMessage();
    void sender(const boost::asio::ip::udp::socket& socket, 
            const std::vector<boost::asio::ip::udp::endpoint>& remoteEndpoints;
            const std::shared_ptr<BaseMessage>& message);


    boost::asio::io_service io_service;
    boost::asio::ip::udp::socket socket;
    std::vector<boost::asio::ip::udp::endpoint> remoteEndpoints;
    std::shared_ptr<BaseMessage> message;
    std::mutex messageSenderMutex;
    std::condition_variable cv;
    bool ready{true};
    std::shared_ptr<MessageFactory> messageFactory{nullptr};
    std::vector<std::thread> threads;


};