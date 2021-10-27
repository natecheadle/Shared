#include <Math/Calculus.hpp>

#include <gtest/gtest.h>

#include <array>
#include <functional>

namespace Shared {
    struct CartesionPoint
    {
        double X;
        double Y;
    };

    constexpr double DelX = 1e-3;

    constexpr double fx(double x) { return 3 * x * x + 2 * x + 1; }

    constexpr double fPrimeX(double x) { return 6 * x + 2; }

    constexpr std::array<CartesionPoint, 100> GetInputData(double delX)
    {
        std::array<CartesionPoint, 100> arrayOfValues;
        double                          x{0};
        for (auto& value : arrayOfValues)
        {
            value.X = x;
            value.Y = fx(x);
            x += delX;
        }

        return arrayOfValues;
    }

    constexpr std::array<CartesionPoint, 100> GetOutputData(double delX)
    {
        std::array<CartesionPoint, 100> arrayOfValues;
        double                          x{0};
        for (auto& value : arrayOfValues)
        {
            value.X = x;
            value.Y = fPrimeX(x);
            x += delX;
        }

        return arrayOfValues;
    }

    constexpr std::array<CartesionPoint, 100> InputData      = GetInputData(DelX);
    constexpr std::array<CartesionPoint, 100> ExpectedOutput = GetOutputData(DelX);

    TEST(Calculus_Tests, ValidateTwoPointForwardDerivative)
    {
        std::function<double(decltype(InputData.begin()))> GetFx = [](decltype(InputData.begin()) it) { return it->Y; };

        size_t index{0};
        for (auto it = InputData.begin(); it != (InputData.end() - 1); ++it)
        {
            double nextValue = Calculus::TwoPointForwardDerivative(it, DelX, GetFx);

            EXPECT_TRUE(std::abs(ExpectedOutput.at(index).Y - nextValue) < 1e-2);
            ++index;
        }
    }

    TEST(Calculus_Tests, ValidateTwoPointReverseDerivative)
    {
        std::function<double(decltype(InputData.begin()))> GetFx = [](decltype(InputData.begin()) it) { return it->Y; };

        size_t index{1};
        for (auto it = InputData.begin() + 1; it != InputData.end(); ++it)
        {
            double nextValue = Calculus::TwoPointBackwardDerivative(it, DelX, GetFx);

            EXPECT_TRUE(std::abs(ExpectedOutput.at(index).Y - nextValue) < 1e-2);
            ++index;
        }
    }

    TEST(Calculus_Tests, ValidateThreePointForwardEndPointDerivative)
    {
        std::function<double(decltype(InputData.begin()))> GetFx = [](decltype(InputData.begin()) it) { return it->Y; };

        size_t index{0};
        for (auto it = InputData.begin(); it != InputData.end() - 2; ++it)
        {
            double nextValue = Calculus::ThreePointForwardEndPointDerivative(it, DelX, GetFx);

            EXPECT_TRUE(std::abs(ExpectedOutput.at(index).Y - nextValue) < 1e-5);
            ++index;
        }
    }

    TEST(Calculus_Tests, ValidateThreePointBackwardEndPointDerivative)
    {
        std::function<double(decltype(InputData.begin()))> GetFx = [](decltype(InputData.begin()) it) { return it->Y; };

        size_t index{2};
        for (auto it = InputData.begin() + 2; it != InputData.end(); ++it)
        {
            double nextValue = Calculus::ThreePointBackwardEndPointDerivative(it, DelX, GetFx);

            EXPECT_TRUE(std::abs(ExpectedOutput.at(index).Y - nextValue) < 1e-5);
            ++index;
        }
    }

    TEST(Calculus_Tests, ValidateThreePointMidPointDerivative)
    {
        std::function<double(decltype(InputData.begin()))> GetFx = [](decltype(InputData.begin()) it) { return it->Y; };

        size_t index{1};
        for (auto it = InputData.begin() + 1; it != InputData.end() - 1; ++it)
        {
            double nextValue = Calculus::ThreePointMidPointDerivative(it, DelX, GetFx);

            EXPECT_TRUE(std::abs(ExpectedOutput.at(index).Y - nextValue) < 1e-5);
            ++index;
        }
    }

    TEST(Calculus_Tests, ValidateFivePointForwardEndPointDerivative)
    {
        std::function<double(decltype(InputData.begin()))> GetFx = [](decltype(InputData.begin()) it) { return it->Y; };

        size_t index{0};
        for (auto it = InputData.begin(); it != InputData.end() - 4; ++it)
        {
            double nextValue = Calculus::FivePointForwardEndPointDerivative(it, DelX, GetFx);

            EXPECT_TRUE(std::abs(ExpectedOutput.at(index).Y - nextValue) < 1e-5);
            ++index;
        }
    }

    TEST(Calculus_Tests, ValidateFivePointBackwardEndPointDerivative)
    {
        std::function<double(decltype(InputData.begin()))> GetFx = [](decltype(InputData.begin()) it) { return it->Y; };

        size_t index{4};
        for (auto it = InputData.begin() + 4; it != InputData.end(); ++it)
        {
            double nextValue = Calculus::FivePointBackwardEndPointDerivative(it, DelX, GetFx);

            EXPECT_TRUE(std::abs(ExpectedOutput.at(index).Y - nextValue) < 1e-5);
            ++index;
        }
    }

    TEST(Calculus_Tests, ValidateFivePointMidPointDerivative)
    {
        std::function<double(decltype(InputData.begin()))> GetFx = [](decltype(InputData.begin()) it) { return it->Y; };

        size_t index{2};
        for (auto it = InputData.begin() + 2; it != InputData.end() - 2; ++it)
        {
            double nextValue = Calculus::FivePointMidPointDerivative(it, DelX, GetFx);

            EXPECT_TRUE(std::abs(ExpectedOutput.at(index).Y - nextValue) < 1e-5);
            ++index;
        }
    }
} // namespace Shared