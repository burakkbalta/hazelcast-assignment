#ifndef MESSAGE_FACTORY_H
#define MESSAGE_FACTORY_H

#include "messages.hh"
#include <memory>
#include <map>

class MessageFactory {
public:
    MessageFactory();
    virtual ~MessageFactory() {}
    virtual std::shared_ptr<BaseMessage> getMessage(const std::string& messageType, const std::string& messageParameter);

private:
    std::map<std::string, std::shared_ptr<BaseMessage>> messageMap;
};

#endif