#ifndef KEYBOARDREADER_H
#define KEYBOARDREADER_H

#include "core/eventreceiver/ReadEventReceiver.h"

#include <cstddef>
#include <functional>
#include <string>

class ClientContext;

class KeyboardReader : public core::eventreceiver::ReadEventReceiver {
public:
    explicit KeyboardReader(ClientContext* clientContext, const std::function<void(std::string)>& callBack);
    ~KeyboardReader();

    void stop();
    void clientContextAway();

protected:
    void readEvent() override;
    void unobservedEvent() override;

    std::function<void(std::string)> callBack;

    ClientContext* clientContext = nullptr;

    // void foo(long value);
};

#endif // KEYBOARDREADER_H
