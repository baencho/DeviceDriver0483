#include "gmock/gmock.h"
#include "device_driver.h"
using namespace testing;

class FlashMemoryDeviceMock : public FlashMemoryDevice {
public:
	MOCK_METHOD(unsigned char, read, (long address), (override));
	MOCK_METHOD(void, write, (long address, unsigned char data), (override));
};

TEST(DeviceDriver, ReadFromHW) {
	// TODO : replace hardware with a Test Double
	//FlashMemoryDevice* hardware = nullptr;
	FlashMemoryDeviceMock hardware;
	DeviceDriver driver{ &hardware };

	EXPECT_CALL(hardware, read(_))
		.Times(5)
		.WillRepeatedly(Return(static_cast<unsigned char>('5')));

	int data = driver.read(0xFF);
	EXPECT_EQ('5', data);
}

int main() {
	::testing::InitGoogleMock();
	return RUN_ALL_TESTS();
}