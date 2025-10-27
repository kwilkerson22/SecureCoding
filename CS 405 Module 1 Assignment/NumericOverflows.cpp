// NumericOverflows.cpp : This file contains the 'main' function. Program execution begins and ends there.
// Name: Kenneth Wilkerson
// Course: CS 405

#include <iostream>  // std::cout
#include <limits>    // std::numeric_limits
#include <typeinfo>  // std::type_info
#include <utility>   // std::pair
#include <cmath>     // std::isfinite

/// <summary>
/// template function for safe addition.
/// Returns {result, successFlag}
/// successFlag = false if overflow detected.
/// </summary>
template <typename T>
std::pair<T, bool> add_numbers(T const& start, T const& increment, unsigned long int const& steps)
{
    T result = start;

    for (unsigned long int i = 0; i < steps; ++i)
    {
        // added to detect overflow before performing addition for integral types only 
        if constexpr (std::is_integral<T>::value)
        {
            if (increment > 0 && result > std::numeric_limits<T>::max() - increment)
            {
                return { result, false }; // overflow occured
            }
        }

        result += increment;

        // added for floating-point check after addition
        if constexpr (std::is_floating_point<T>::value)
        {
            if (!std::isfinite(result))
            {
                return { result, false }; // overflow occurred
            }
        }
    }

    return { result, true }; // no overflow
}

/// <summary>
/// Template function for safe subtraction.
/// Returns {result, successFlag}
/// successFlag = false if underflow detected.
/// </summary>
template <typename T>
std::pair<T, bool> subtract_numbers(T const& start, T const& decrement, unsigned long int const& steps)
{
    T result = start;

    for (unsigned long int i = 0; i < steps; ++i)
    {
        // added to detect underflow before performing subtraction for integral types only
        if constexpr (std::is_integral<T>::value)
        {
            if (std::is_signed<T>::value)
            {
                if (result - decrement > result)
                {
                    return { result, false }; // underflow occured
                }
        }
        else
        {
            if (result < decrement)
            {
                return { result, false };
            }
        }
        }

        result -= decrement;

        // added for floating-point check after subtraction
        if constexpr (std::is_floating_point<T>::value)
        {
            if (!std::isfinite(result))
            {
                return { result, false }; // underflow occurred
            }
        }
    }

    return { result, true }; // no underflow
}

template <typename T>
void test_overflow()
{
    // START DO NOT CHANGE
    // how many times will we iterate
    const unsigned long int steps = 5;
    // how much will we add each step (result should be: start + (increment * steps))
    const T increment = std::numeric_limits<T>::max() / steps;
    // what's our starting point
    const T start = 0;

    std::cout << "Overflow Test of Type = " << typeid(T).name() << std::endl;
    // END DO NOT CHANGE

    // Test 1: no overflow expected
    auto [result1, ok1] = add_numbers<T>(start, increment, steps);
    std::cout << "\tAdding Numbers Without Overflow = " << +result1
              << " | Overflow? " << (ok1 ? "false" : "true") << std::endl;

    // Test 2: overflow expected
    auto [result2, ok2] = add_numbers<T>(start, increment, steps + 1);
    std::cout << "\tAdding Numbers With Overflow = " << +result2
              << " | Overflow? " << (ok2 ? "false" : "true") << std::endl;
}

template <typename T>
void test_underflow()
{
    // START DO NOT CHANGE
    // how many times will we iterate
    const unsigned long int steps = 5;
    // how much will we subtract each step (result should be: start - (increment * steps))
    const T decrement = std::numeric_limits<T>::max() / steps;
    // what's our starting point
    const T start = std::numeric_limits<T>::max();

    std::cout << "Underflow Test of Type = " << typeid(T).name() << std::endl;
    // END DO NOT CHANGE

    // Test 1: no underflow expected
    auto [result1, ok1] = subtract_numbers<T>(start, decrement, steps);
    std::cout << "\tSubtracting Numbers Without Underflow = " << +result1
              << " | Underflow? " << (ok1 ? "false" : "true") << std::endl;

    // Test 2: underflow expected
    auto [result2, ok2] = subtract_numbers<T>(start, decrement, steps + 1);
    std::cout << "\tSubtracting Numbers With Underflow = " << +result2
              << " | Underflow? " << (ok2 ? "false" : "true") << std::endl;
}

void do_overflow_tests(const std::string& star_line)
{
    std::cout << std::endl << star_line << std::endl;
    std::cout << "*** Running Overflow Tests ***" << std::endl;
    std::cout << star_line << std::endl;

    // signed integers
    test_overflow<char>();
    test_overflow<wchar_t>();
    test_overflow<short int>();
    test_overflow<int>();
    test_overflow<long>();
    test_overflow<long long>();

    // unsigned integers
    test_overflow<unsigned char>();
    test_overflow<unsigned short int>();
    test_overflow<unsigned int>();
    test_overflow<unsigned long>();
    test_overflow<unsigned long long>();

    // real numbers
    test_overflow<float>();
    test_overflow<double>();
    test_overflow<long double>();
}

void do_underflow_tests(const std::string& star_line)
{
    std::cout << std::endl << star_line << std::endl;
    std::cout << "*** Running Underflow Tests ***" << std::endl;
    std::cout << star_line << std::endl;

    // signed integers
    test_underflow<char>();
    test_underflow<wchar_t>();
    test_underflow<short int>();
    test_underflow<int>();
    test_underflow<long>();
    test_underflow<long long>();

    // unsigned integers
    test_underflow<unsigned char>();
    test_underflow<unsigned short int>();
    test_underflow<unsigned int>();
    test_underflow<unsigned long>();
    test_underflow<unsigned long long>();

    // real numbers
    test_underflow<float>();
    test_underflow<double>();
    test_underflow<long double>();
}
/// <summary>
/// Entry point into the application
/// </summary>
/// <returns>0 when complete</returns>
int main()
{
    // create a string of "*" to use in the console
    const std::string star_line = std::string(50, '*');

    std::cout << "Starting Numeric Underflow / Overflow Tests!" << std::endl;

    // run the overflow tests
    do_overflow_tests(star_line);
    
    // run the underflow tests
    do_underflow_tests(star_line);

    std::cout << std::endl << "All Numeric Underflow / Overflow Tests Complete!" << std::endl;

    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu
