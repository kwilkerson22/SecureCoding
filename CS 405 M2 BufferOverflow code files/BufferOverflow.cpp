// BufferOverflow.cpp : This file contains the 'main' function. Program execution begins and ends there.
// Name: Kenneth Wilkerson
// Course: CS 405

#include <iostream>
#include <string>
#include <cstdio>

int main() {
  std::cout << "Buffer Overflow Example" << std::endl;
  
  
  const std::string account_number = "CharlieBrown42";
  char user_input[20];
  
  // set maximum characters that can be stored - left room for null terminator.
  constexpr std::size_t BUF_SIZE = sizeof(user_input);
  constexpr std::size_t MAX_STORABLE = BUF_SIZE - 1;

  /*
  1) Read the entire line into a std:string.
  2) If the input is longer than the buffer, reduce it and notify the user.
  3) Use std::snprintf to copy into the fixed-size buffer and ensures null-termination.
  */
  std::string line;
  while (true) {
    std::cout << "Enter a Value: ";
    std::getline(std::cin, line); // safer full-line read that handles spaces.
    // empty entry not allowed
    if (line.empty()) {
      std::cout << "Input cannot be empty — try again.\n";
      continue;
    }
    if (line.size() > MAX_STORABLE) {
        // notify users that input will be reduced to fit buffer.
      std::cerr << "Input too long (" << line.size() 
                << " chars). It will be reduced to " << MAX_STORABLE << ".\n";
      
    }
    // copy safely into user_input and ensure null termination.
    std::snprintf(user_input, BUF_SIZE, "%s", line.c_str());
    break;
  }

  std::cout << "You entered: " << user_input << std::endl;
  std::cout << "Account Number = " << account_number << std::endl;
}
