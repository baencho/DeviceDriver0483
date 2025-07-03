#include "gmock/gmock.h"
#include "device_driver.h"
#include <string>
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

TEST(DeviceDriver, ReadFromHWFail) {
	// TODO : replace hardware with a Test Double
	//FlashMemoryDevice* hardware = nullptr;
	FlashMemoryDeviceMock hardware;
	DeviceDriver driver{ &hardware };

	EXPECT_CALL(hardware, read(_))
		.WillOnce(Return(static_cast<unsigned char>('5')))
		.WillOnce(Return(static_cast<unsigned char>('5')))
		.WillRepeatedly(Return(static_cast<unsigned char>('A')));

	try {
		int data = driver.read(0xFF);
		FAIL();
	}
	catch (std::exception& e) {
		EXPECT_EQ(std::string{ e.what() }, std::string{ "ReadFailException" });
	}
}

TEST(DeviceDriver, WriteFromHW) {
	//FlashMemoryDevice* hardware = nullptr;
	FlashMemoryDeviceMock hardware;
	DeviceDriver driver{ &hardware };

	EXPECT_CALL(hardware, read(_))
		.Times(1);

	EXPECT_CALL(hardware, write(_, _))
		.Times(1);

	driver.write(0xA, 0xB);
}

TEST(DeviceDriver, WriteFromHWFail) {
	FlashMemoryDeviceMock hardware;
	DeviceDriver driver{ &hardware };

	EXPECT_CALL(hardware, read(_))
		.Times(1)
		.WillRepeatedly(Return(0xFF));

	EXPECT_CALL(hardware, write(_, _))
		.Times(0);

	try {
		driver.write(0xA, 0xB);
	}
	catch (std::exception& e) {
		EXPECT_EQ(std::string{ e.what() }, std::string{ "WriteFailException" });
	}
}

int main() {
	::testing::InitGoogleMock();
	return RUN_ALL_TESTS();
}