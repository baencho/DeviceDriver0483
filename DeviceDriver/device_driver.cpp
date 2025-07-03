#include "device_driver.h"
#include <stdexcept>

DeviceDriver::DeviceDriver(FlashMemoryDevice* hardware) : m_hardware(hardware)
{
}

int DeviceDriver::read(long address)
{
    // TODO: implement this method properly
    int ret = (int)(m_hardware->read(address));
    for (int i = 0; i<4; i++) {
        int temp = (int)(m_hardware->read(address));
        if (temp == ret) {
            ret = temp;
            continue;
        }
        throw std::runtime_error("Not same value for 5 times");
    }
    return ret;
}

void DeviceDriver::write(long address, int data)
{
    // TODO: implement this method
    m_hardware->write(address, (unsigned char)data);
}