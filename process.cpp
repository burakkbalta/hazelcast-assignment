#include "process.hh"

#include <sstream>

Process::Process(const std::shared_ptr<MessageFactory>& mesFactory, const CommandLineArguments& commandLineArgs) : 
socket{io_service},
messageFactory{mesFactory}
{
    socket.open(boost::asio::ip::udp::v4());
    createLocalSocketAndRemoteEndpoints(commandLineArgs.localIp, commandLineArgs.localPort, 
            commandLineArgs.remoteAddresses);
    populateSendingMessage(commandLineArgs.messageType, commandLineArgs.messageParameter);        
}

Process::~Process() {
    socket.close();
}

void Process::createAndStartThreads(const std::shared_ptr<BaseMessage>& sendingMessage) {
    for(size_t i=0; i<numberOfThreads; ++i) {
        threads.push_back(std::thread(sender, std::ref(socket), std::ref(remoteEndpoints), std::ref(message)));
    }

    for(const auto& th : threads) {
        th.join();
    }
}

void Process::createLocalSocketAndRemoteEndpoints(const std::string& localIp, unsigned int localPort, 
            std::vector<std::pair<std::string, unsigned int>>& remoteAddresses) {
    socket.bind(boost::asio::ip::udp::endpoint(address::from_string(localIp), localPort));

    for(const auto& elem : remoteAddresses) {
        boost::asio::ip::udp::endpoint remoteEndpoint(address::from_string(localIp), localPort);
        remoteEndpoints.push_back(remoteEndpoint);
    }
}

void Process::populateSendingMessage(const std::string& messageType, const std::string& messageParameter) {
    message = std::move(messageFactory->getMessage(messageType, messageParameter));
}

void Process::receiveMessage() {

}

void Process::sender(const boost::asio::ip::udp::socket& socket, 
            const std::vector<boost::asio::ip::udp::endpoint>& remoteEndpoints;
            const std::shared_ptr<BaseMessage>& message) {

    std::unique_lock lk(messageSenderMutex);
    cv.wait(lk, []{return ready;});
    
    auto mes = message->clone();
    auto myid = std::this_thread::get_id();
    std::stringstream ss;
    ss << myid;
    std::string mystring = ss.str();
    mes->thread_id = mystring;

    ready = false;
    boost::system::error_code err;
    for(const auto& remoteEndp : remoteEndpoints) {
        auto sent = socket.send_to(boost::asio::buffer(message->serializeMessage()), remoteEndp, 0, err);
    }

    lk.unlock();
    cv.notify_one();
}