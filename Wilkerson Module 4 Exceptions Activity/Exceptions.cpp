// Exceptions.cpp : This file contains the 'main' function. Program execution begins and ends there.
// Name: Kenneth Wilkerson
// Course: CS 405

#include <iostream>
#include <stdexcept>  
#include <exception>  
#include <string>

// ==========================
// Custom Exception Class
// ==========================
// Custom exception derived from std::exception
class CustomException : public std::exception {
private:
    std::string message;
public:
    explicit CustomException(const std::string& msg) : message(msg) {}
    const char* what() const noexcept override {
        return message.c_str();
    }
};

bool do_even_more_custom_application_logic()
{
    std::cout << "Running Even More Custom Application Logic." << std::endl;

    // throwing a standard runtime_error 
    throw std::runtime_error("Standard runtime error occurred in do_even_more_custom_application_logic()");

    return true; 
}

void do_custom_application_logic()
{
    std::cout << "Running Custom Application Logic." << std::endl;

    try {
        if (do_even_more_custom_application_logic()) {
            std::cout << "Even More Custom Application Logic Succeeded." << std::endl;
        }
    }
    catch (const std::exception& e) {
        // catching standard exception and displaying the error message
        std::cerr << "Exception handled in do_custom_application_logic() Standard exception: " << e.what() << std::endl;
    }

    // throwing CustomException to be explicitly caught in main()
    throw CustomException("Custom exception occurred in do_custom_application_logic()");

    std::cout << "Leaving Custom Application Logic." << std::endl;
}

float divide(float num, float den)
{
    // throws an exception dealing with divide by zero errors
    if (den == 0.0f) {
        throw std::overflow_error("Divide by zero error in divide()");
    }
    return (num / den);
}

void do_division() noexcept
{
    float numerator = 10.0f;
    float denominator = 0.0f;

    try {
        auto result = divide(numerator, denominator);
        std::cout << "divide(" << numerator << ", " << denominator << ") = " << result << std::endl;
    }
    catch (const std::overflow_error& e) {
        // handling only the specific exception thrown by divide()
        std::cerr << "Exception handled in do_division() Overflow error: " << e.what() << std::endl;
    }
}

int main()
{
    std::cout << "Exceptions Tests!" << std::endl;

    // wrapping entire main in exception handlers
    try {
        do_division();
        do_custom_application_logic();
    }
    catch (const CustomException& e) {
        std::cerr << "Exception handled in main() CustomException: " << e.what() << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Exception handled in main() Standard exception: " << e.what() << std::endl;
    }
    catch (...) {
        // Catch-all handler for any unhandled exceptions
        std::cerr << "Exception handled in main() Unknown exception occurred." << std::endl;
    }

    std::cout << "Program continues normally after exception handling." << std::endl;
    return 0;
}
