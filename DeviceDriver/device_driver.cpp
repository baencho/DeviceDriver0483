#include "device_driver.h"
#include <stdexcept>

DeviceDriver::DeviceDriver(FlashMemoryDevice* hardware) : m_hardware(hardware)
{
}

class ReadFailException : public std::exception {
private:
    const std::string message = "ReadFailException";

public:
    const char* what() const noexcept override {
        return message.c_str();
    }
};

class WriteFailException : public std::exception {
private:
    const std::string message = "WriteFailException";

public:
    const char* what() const noexcept override {
        return message.c_str();
    }
};

void DeviceDriver::checkReadPostCondition(int firstRead, long address) {
    for (int i = 0; i < READ_CREDIBILITY_COUNT - 1; i++) {
        int temp = (int)(m_hardware->read(address));
        if (temp == firstRead) continue;
        throw ReadFailException();
    }
}

void DeviceDriver::checkWritePreCondition(long address) {
    int readResult = (int)(m_hardware->read(address));
    if (readResult == PAGE_UNCLEAN) {
        throw WriteFailException();
    }
}

int DeviceDriver::read(long address) {
    int result = (int)(m_hardware->read(address));
    checkReadPostCondition(result, address);
    return result;
}

void DeviceDriver::write(long address, int data) {
    checkWritePreCondition(address);
    m_hardware->write(address, (unsigned char)data);
}