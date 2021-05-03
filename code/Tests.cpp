#include "acutest.h"

#include "FileIO.hpp"

void fileIO_allocTempBuffer(void)
{
	char *result;
	result = allocTemporaryBuffer(1024);
	TEST_CHECK(result != 0);

	result = allocTemporaryBuffer(-1024);
	TEST_CHECK(result != 0);

	result = allocTemporaryBuffer(0);
	TEST_CHECK(result != 0);
}

void fileIO_checkFileSizeOK(void)
{
	bool result;

	result = checkFileSizeOK(10, 20);
	TEST_CHECK(result);

	result = checkFileSizeOK(20, 10);
	TEST_CHECK(!result);

	result = checkFileSizeOK(5, 5);
	TEST_CHECK(!result);

	result = checkFileSizeOK(-15, 5);
	TEST_CHECK(!result);

	result = checkFileSizeOK(15, -5);
	TEST_CHECK(!result);

	result = checkFileSizeOK(-10, -5);
	TEST_CHECK(!result);

	result = checkFileSizeOK(1, 0);
	TEST_CHECK(!result);

	result = checkFileSizeOK(0, 1);
	TEST_CHECK(!result);

	result = checkFileSizeOK(0, 0);
	TEST_CHECK(!result);
}

TEST_LIST = 
{
	{"char *allocTemporaryBuffer(int bufferSize)", fileIO_allocTempBuffer},
	{"bool checkFileSizeOK(bytesRead ,outBufferSize)", fileIO_checkFileSizeOK},
	{0}
};
