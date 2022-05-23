#include <MaxFlow.cpp>
#include <stdexcept>
#include <gtest/gtest.h>

TEST(MaxFlow, set)
{
    MaxFlow myflow("set.txt");
    EXPECT_EQ(myflow.get_maxflow(), 5);
}

TEST(MaxFlow, set2)
{
    MaxFlow myflow("set2.txt");
    EXPECT_EQ(myflow.get_maxflow(), 6);
}

TEST(MaxFlow, set3)
{
    MaxFlow myflow("set3.txt");
    EXPECT_EQ(myflow.get_maxflow(), 2);
}
