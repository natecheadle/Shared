#pragma once

#include <functional>
#include <numeric>

namespace Shared {
    class Calculus {
      public:
        /// <summary>
        /// Given function y = f(x) Computes f'(x0)
        /// </summary>
        /// <typeparam name="INPUT_IT">iterator to container of numeric values</typeparam>
        /// <param name="fx0">iterator pointing to f(x0), fx0 - 1 must be valid</param>
        /// <param name="h">step size between input values a.k.a. x1 - x0</param>
        /// <param name="f_GetValue">functor to get value to get f(x0), by default returns *fx0</param>
        /// <returns>value of f'(x0)</returns>
        template <class INPUT_IT>
        static double TwoPointBackwardDerivative(
            INPUT_IT                        fx0,
            double                          h,
            std::function<double(INPUT_IT)> f_GetValue = [](INPUT_IT it) { return *it; })
        {
            return (f_GetValue(fx0) - f_GetValue(fx0 - 1)) / h;
        }

        /// <summary>
        /// Given function y = f(x) Computes f'(x0)
        /// </summary>
        /// <typeparam name="INPUT_IT">iterator to container of numeric values</typeparam>
        /// <param name="fx0">iterator pointing to f(x0), fx0 + 1 must be valid</param>
        /// <param name="h">step size between input values a.k.a. x1 - x0</param>
        /// <param name="f_GetValue">functor to get value to get f(x0), by default returns *fx0</param>
        /// <returns></returns>
        template <class INPUT_IT>
        static double TwoPointForwardDerivative(
            INPUT_IT                        fx0,
            double                          h,
            std::function<double(INPUT_IT)> f_GetValue = [](INPUT_IT it) { return *it; })
        {
            return (f_GetValue(fx0 + 1) - f_GetValue(fx0)) / h;
        }

        /// <summary>
        /// Given function y = f(x) Computes f'(x0).
        /// It is generally preferred to use the MidPointDerivative,
        /// it is more accurate and less computationally intensive.
        /// </summary>
        /// <typeparam name="INPUT_IT">iterator to container of numeric values</typeparam>
        /// <param name="fx0">iterator pointing to f(x0), f(x0) + 2 must be valid</param>
        /// <param name="h">step size between input values a.k.a. x1 - x0</param>
        /// <param name="f_GetValue">functor to get value to get f(x0), by default returns *fx0</param>
        /// <returns>value of f'(x0)</returns>
        template <class INPUT_IT>
        static double ThreePointForwardEndPointDerivative(
            INPUT_IT                        fx0,
            double                          h,
            std::function<double(INPUT_IT)> f_GetValue = [](INPUT_IT it) { return *it; })
        {
            return (1 / (2 * h)) * (-3 * f_GetValue(fx0) + 4 * f_GetValue(fx0 + 1) - f_GetValue(fx0 + 2));
        }

        /// <summary>
        /// Given function y = f(x) Computes f'(x0).
        /// It is generally preferred to use the MidPointDerivative,
        /// it is more accurate and less computationally intensive.
        /// </summary>
        /// <typeparam name="INPUT_IT">iterator to container of numeric values</typeparam>
        /// <param name="fx0">iterator pointing to f(x0), f(x0) - 2 must be valid</param>
        /// <param name="h">step size between input values a.k.a. x1 - x0</param>
        /// <param name="f_GetValue">functor to get value to get f(x0), by default returns *fx0</param>
        /// <returns>value of f'(x0)</returns>
        template <class INPUT_IT>
        static double ThreePointBackwardEndPointDerivative(
            INPUT_IT                        fx0,
            double                          h,
            std::function<double(INPUT_IT)> f_GetValue = [](INPUT_IT it) { return *it; })
        {
            return (1 / (2 * h)) * (3 * f_GetValue(fx0) - 4 * f_GetValue(fx0 - 1) + f_GetValue(fx0 - 2));
        }

        /// <summary>
        /// Given function y = f(x) Computes f'(x0)
        /// </summary>
        /// <typeparam name="INPUT_IT">iterator to container of numeric values</typeparam>
        /// <param name="fx0">iterator pointing to f(x0), fx0 + 1 and fx0 - 1 must be valid</param>
        /// <param name="h">step size between input values a.k.a. x1 - x0</param>
        /// <param name="f_GetValue">functor to get value to get f(x0), by default returns *fx0</param>
        /// <returns>value of f'(x1)</returns>
        template <class INPUT_IT>
        static double ThreePointMidPointDerivative(
            INPUT_IT                        fx0,
            double                          h,
            std::function<double(INPUT_IT)> f_GetValue = [](INPUT_IT it) { return *it; })
        {
            return (1 / (2 * h)) * (f_GetValue(fx0 + 1) - f_GetValue(fx0 - 1));
        }

        /// <summary>
        /// Given function y = f(x) Computes f'(x0)
        /// It is generally preferred to use the MidPointDerivative,
        /// it is more accurate and less computationally intensive.
        /// </summary>
        /// <typeparam name="INPUT_IT">iterator to container of numeric values</typeparam>
        /// <param name="fx0">iterator pointing to f(x0), f(x0) + 4 must be valid</param>
        /// <param name="h">step size between input values a.k.a. x1 - x0</param>
        /// <param name="f_GetValue">functor to get value to get f(x0), by default returns *fx0</param>
        /// <returns>value of f'(x0)</returns>
        template <class INPUT_IT>
        static double FivePointForwardEndPointDerivative(
            INPUT_IT                        fx0,
            double                          h,
            std::function<double(INPUT_IT)> f_GetValue = [](INPUT_IT it) { return *it; })
        {
            return (1 / (12 * h)) * (-25 * f_GetValue(fx0) + 48 * f_GetValue(fx0 + 1) - 36 * f_GetValue(fx0 + 2) +
                                     16 * f_GetValue(fx0 + 3) - 3 * f_GetValue(fx0 + 4));
        }

        /// <summary>
        /// Given function y = f(x) Computes f'(x0)
        /// It is generally preferred to use the MidPointDerivative,
        /// it is more accurate and less computationally intensive.
        /// </summary>
        /// <typeparam name="INPUT_IT">iterator to container of numeric values</typeparam>
        /// <param name="fx0">iterator pointing to f(x0), fx0 - 4 must be valid</param>
        /// <param name="h">step size between input values a.k.a. x1 - x0</param>
        /// <param name="f_GetValue">functor to get value to get f(x0), by default returns *fx0</param>
        /// <returns>value of f'(x0)</returns>
        template <class INPUT_IT>
        static double FivePointBackwardEndPointDerivative(
            INPUT_IT                        fx0,
            double                          h,
            std::function<double(INPUT_IT)> f_GetValue = [](INPUT_IT it) { return *it; })
        {
            return (1 / (12 * h)) * (25 * f_GetValue(fx0) - 48 * f_GetValue(fx0 - 1) + 36 * f_GetValue(fx0 - 2) -
                                     16 * f_GetValue(fx0 - 3) + 3 * f_GetValue(fx0 - 4));
        }

        /// <summary>
        /// Given function y = f(x) Computes f'(x0)
        /// </summary>
        /// <typeparam name="INPUT_IT">iterator to container of numeric values</typeparam>
        /// <param name="fx0">iterator pointing to f(x0), fx0 + 2 and fx0 - 2 must be valid</param>
        /// <param name="h">step size between input values a.k.a. x1 - x0</param>
        /// <param name="f_GetValue">functor to get value to get f(x0), by default returns *fx0</param>
        /// <returns>value of f'(x3)</returns>
        template <class INPUT_IT>
        static double FivePointMidPointDerivative(
            INPUT_IT                        fx0,
            double                          h,
            std::function<double(INPUT_IT)> f_GetValue = [](INPUT_IT it) { return *it; })
        {
            return (1 / (12 * h)) *
                   (f_GetValue(fx0 - 2) - 8 * f_GetValue(fx0 - 1) + 8 * f_GetValue(fx0 + 1) - f_GetValue(fx0 + 2));
        }
    };
} // namespace Shared