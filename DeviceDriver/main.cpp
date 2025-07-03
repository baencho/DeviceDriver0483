#include "gmock/gmock.h"
#include "device_driver.h"
#include <string>
using namespace testing;
class FlashMemoryDeviceMock : public FlashMemoryDevice {
public:
	MOCK_METHOD(unsigned char, read, (long address), (override));
	MOCK_METHOD(void, write, (long address, unsigned char data), (override));
};

class DeviceDriverFixture : public Test {
public:
	FlashMemoryDeviceMock hardware;
	DeviceDriver driver{ &hardware };

	const long RANDOM_ADDRESS = 0xAB;
	const unsigned char RANDOM_CHAR = '5';
	const int PAGE_UNCLEAN = 0xFF;
};

TEST_F(DeviceDriverFixture, ReadFromHW) {
	EXPECT_CALL(hardware, read(_))
		.Times(5)
		.WillRepeatedly(Return(RANDOM_CHAR));

	int data = driver.read(RANDOM_ADDRESS);
	EXPECT_EQ(RANDOM_CHAR, data);
}

TEST_F(DeviceDriverFixture, ReadFromHWFail) {
	const unsigned char DIFFERENT_CHAR = '6';

	EXPECT_CALL(hardware, read(_))
		.WillOnce(Return(RANDOM_CHAR))
		.WillOnce(Return(RANDOM_CHAR))
		.WillRepeatedly(Return(DIFFERENT_CHAR));

	try {
		int data = driver.read(RANDOM_ADDRESS);
		FAIL();
	}
	catch (std::exception& e) {
		EXPECT_EQ(std::string{ e.what() }, std::string{ "ReadFailException" });
	}
}

TEST_F(DeviceDriverFixture, WriteFromHW) {
	EXPECT_CALL(hardware, read(_))
		.Times(1);

	EXPECT_CALL(hardware, write(_, _))
		.Times(1);

	driver.write(RANDOM_ADDRESS, RANDOM_CHAR);
}

TEST_F(DeviceDriverFixture, WriteFromHWFail) {
	EXPECT_CALL(hardware, read(_))
		.Times(1)
		.WillRepeatedly(Return(PAGE_UNCLEAN));

	EXPECT_CALL(hardware, write(_, _))
		.Times(0);

	try {
		driver.write(RANDOM_ADDRESS, RANDOM_CHAR);
	}
	catch (std::exception& e) {
		EXPECT_EQ(std::string{ e.what() }, std::string{ "WriteFailException" });
	}
}

int main() {
	::testing::InitGoogleMock();
	return RUN_ALL_TESTS();
}