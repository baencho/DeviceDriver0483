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

int DeviceDriver::read(long address)
{
    // TODO: implement this method properly
    int ret = (int)(m_hardware->read(address));
    for (int i = 0; i < 4; i++) {
        int temp = (int)(m_hardware->read(address));
        if (temp == ret) {;
            continue;
        }
        ReadFailException readFailException;
        throw readFailException;
    }
    return ret;
}

void DeviceDriver::write(long address, int data)
{
    // TODO: implement this method
    m_hardware->write(address, (unsigned char)data);
}