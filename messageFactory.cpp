#include "messageFactory.hh"


MessageFactory::MessageFactory() {
    messageMap = { {"--int-message", std::make_shared<message1>()},
                   {"--double-message", std::make_shared<message2>()},
                   {"--string-message", std::make_shared<message3>()}};
}

std::shared_ptr<BaseMessage> MessageFactory::getMessage(const std::string& messageType, const std::string& messageParameter) {
    auto it = messageMap.find(messageType);

    if(it != messageMap.end()) {
        auto message = it->second;
        message->setMessageParameter(messageParameter);
        return message;
    } else {
        return nullptr;
    }
}