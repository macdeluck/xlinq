#include <gtest/gtest.h>
#include <xlinq/xlinq_from.h>
#include <xlinq/xlinq_stl.h>
#include <memory>
#include <vector>

using namespace std;
using namespace xlinq;

TEST(XlinqStlTest, ClassicForEach)
{
	vector<int> numbers = { 1, 2, 3, 4, 5 };

	int i = 0;
	for (auto val : from(numbers) >> stl())
		ASSERT_EQ(numbers[i++], val);
}