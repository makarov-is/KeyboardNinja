#include "acutest.h"

#include "FileIO.hpp"

void dummy() {};

//=============================================

void fileIO_allocTempBuffer1(void)
{
	char *result = allocTemporaryBuffer(1024);
	TEST_CHECK(result != 0);
}

void fileIO_allocTempBuffer2(void)
{
	char *result = allocTemporaryBuffer(-1024);
	TEST_CHECK(result != 0);
}

void fileIO_allocTempBuffer3(void)
{
	char *result = allocTemporaryBuffer(0);
	TEST_CHECK(result != 0);
}

//=============================================

void fileIO_checkFileSizeOK1(void)
{
	bool result = checkFileSizeOK(10, 20);
	TEST_CHECK(result);
}

void fileIO_checkFileSizeOK2(void)
{
	bool result = checkFileSizeOK(20, 10);
	TEST_CHECK(!result);
}

void fileIO_checkFileSizeOK3(void)
{
	bool result = checkFileSizeOK(5, 5);
	TEST_CHECK(!result);
}

void fileIO_checkFileSizeOK4(void)
{
	bool result = checkFileSizeOK(1, 0);
	TEST_CHECK(!result);
}

void fileIO_checkFileSizeOK5(void)
{
	bool result = checkFileSizeOK(0, 1);
	TEST_CHECK(!result);
}
void fileIO_checkFileSizeOK6(void)
{
	bool result = checkFileSizeOK(0, 0);
	TEST_CHECK(!result);
}

TEST_LIST = 
{
	{"1", dummy},
	{"allocTemporaryBuffer(1024)", fileIO_allocTempBuffer1},
	{"allocTemporaryBuffer(-1024)", fileIO_allocTempBuffer2},
	{"allocTemporaryBuffer(0)", fileIO_allocTempBuffer3},

	{"2", dummy},
	{"checkFileSizeOK(10, 20)", fileIO_checkFileSizeOK1},
	{"checkFileSizeOK(20, 10)", fileIO_checkFileSizeOK2},
	{"checkFileSizeOK(5, 5)", fileIO_checkFileSizeOK3},
	{"checkFileSizeOK(1, 0)", fileIO_checkFileSizeOK4},
	{"checkFileSizeOK(0, 1)", fileIO_checkFileSizeOK5},
	{"checkFileSizeOK(0, 0)", fileIO_checkFileSizeOK6},

	{0}
};
