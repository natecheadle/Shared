#include <Container/StaticQueue.hpp>

#include <gtest/gtest.h>

namespace Shared {
    static constexpr size_t SIZE = 10;
    TEST(StaticQueue_Tests, ValidateConstruction)
    {
        StaticQueue<int, SIZE> queue;
        EXPECT_TRUE(queue.Empty());
        EXPECT_EQ(0, queue.Size());
        EXPECT_EQ(SIZE, queue.MaxSize());
        EXPECT_FALSE(queue.Full());
    }

    TEST(StaticQueue_Tests, ValidateFillEmpty)
    {
        StaticQueue<int, SIZE> queue;
        ASSERT_TRUE(queue.Empty());
        ASSERT_FALSE(queue.Pop());
        ASSERT_EQ(0, queue.Size());
        ASSERT_FALSE(queue.Full());
        for (int y = 0; y < 5; ++y)
        {
            int i{0};
            while (!queue.Full())
            {
                ASSERT_TRUE(queue.Push(i++));
                ASSERT_EQ(i, queue.Size());
                ASSERT_EQ(i - 1, queue.Back());
            }
            ASSERT_EQ(SIZE, i);
            ASSERT_FALSE(queue.Push(i + 1));
            i = 0;
            while (!queue.Empty())
            {
                ASSERT_EQ(i++, queue.Front());
                ASSERT_TRUE(queue.Pop());
            }
        }
    }

    TEST(StaticQueue_Test, ValidateIterator)
    {
        StaticQueue<int, SIZE> queue;

        int i{0};
        while (!queue.Full())
        {
            ASSERT_TRUE(queue.Push(i++));
            ASSERT_EQ(i, queue.Size());
        }

        i = 0;
        for (auto it = queue.Begin(); it != queue.End(); ++it)
        {
            ASSERT_EQ(i++, *it);
        }

        for (int j = 1; j < SIZE; ++j)
        {
            queue.Pop();

            i = j;
            for (auto it = queue.Begin(); it != queue.End(); ++it)
            {
                ASSERT_EQ(i++, *it);
            }
        }
    }

} // namespace Shared
