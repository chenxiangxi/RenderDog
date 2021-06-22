#include "gtest/gtest.h"
#include "VulkanRHI.h"

int Factorial(int n)
{
	return n;
}

TEST(FactorialTest, HandlesZeroInput) {
	VulkanRHI vulkan;
	vulkan.Init();

	EXPECT_EQ(Factorial(1), 1);
}

int main(int argc, char** argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
