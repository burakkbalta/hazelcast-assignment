#pragma once

#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <utility>

struct CommandLineArguments {
    std::string localIp;
    unsigned int localPort;
    std::vector<std::pair<std::string, unsigned int>> remoteAddresses;
    std::string messageType;
    std::string messageParameter;
};

struct BaseMessage {
    BaseMessage(const std::string& t_messageName) : messageName{t_messageName} {}
    std::string messageName{};
    std::string thread_id;

    virtual void setMessageParameter(const std::string& messageParameter) = 0;

    std::shared_ptr<BaseMessage> clone() {
        return std::make_shared<BaseMessage>(*this);
    }
    virtual std::string serializeMessage() {
        return "Base Message";
    }

    virtual std::shared_ptr<BaseMessage> deserializeMessage() {
        return nullptr;
    }

    template<typename T>
    T setParameter(const std::string& mesParameter) {
        std::stringstream ss << mesParameter;
        T value{};
        ss >> value;
        return value;
    }
};

struct message1 : public BaseMessage {
    message1() : BaseMessage("--int-message") {}
    int i;
    friend std::ostream &operator<<(std::ostream &os, const message1 &message1) {
        os << "Message1 : " << message1.i << " thread_id: " <<
        message1.thread_id;
        return os;
    }

    void setMessageParameter(const std::string& messageParameter) override {
        i = setParameter<int>(messageParameter);
    }
};

struct message2 : public BaseMessage {
    message2() : BaseMessage("--double-message")
    {
    }
    
    double d;
    friend std::ostream &operator<<(std::ostream &os, const message2 &message2) {
        os << "Message2 d: " << message2.d << " thread_id: " <<
        message2.thread_id;
        return os;
    }

    void setMessageParameter(const std::string& messageParameter) override {
        d = setParameter<double>(messageParameter);
    }
};

struct message3 :public BaseMessage {
    message3() : BaseMessage("--string-message") {}
    std::string s;
    
    friend std::ostream &operator<<(std::ostream &os, const message3 &message3) {
        os << "Message3 s: " << message3.s << " thread_id: " <<
        message3.thread_id;
        return os;
    }

    void setMessageParameter(const std::string& messageParameter) override {
        s = std::move(setParameter<int>(messageParameter));
    }
};