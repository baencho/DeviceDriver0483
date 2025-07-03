#pragma once
#include "flash_memory_device.h"

class DeviceDriver
{
public:
    DeviceDriver(FlashMemoryDevice* hardware);
    int read(long address);
    void write(long address, int data);

protected:
    FlashMemoryDevice* m_hardware;

private:
    void checkReadPostCondition(int firstRead, long address);
    void checkWritePreCondition(long address);

    const int READ_CREDIBILITY_COUNT = 5;
    const int PAGE_CLEAN = 0xFF;
};