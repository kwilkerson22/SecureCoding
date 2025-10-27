// Encryption.cpp : This file contains the 'main' function. Program execution begins and ends there.
// Name: Kenneth Wilkerson
// Course: CS 405

#include <cassert>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <ctime>


// encrypt or decrypt a source string using the provided key 
std::string encrypt_decrypt(const std::string& source, const std::string& key)
{
    const auto key_length = key.length();
    const auto source_length = source.length();

    assert(key_length > 0);
    assert(source_length > 0);

    std::string output = source;

    for (size_t i = 0; i < source_length; ++i)
    {
        // XOR each character with the key, wrapping the key using modulo
        output[i] = source[i] ^ key[i % key_length];
    }

    assert(output.length() == source_length);
    return output;
}

// read the content of a text file into a single string
std::string read_file(const std::string& filename)
{
    std::ifstream file(filename);
    if (!file)
    {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return "";
    }

    std::stringstream buffer;
    buffer << file.rdbuf();  // read the entire file
    return buffer.str();
}

// extract the student name (first line) from the file data
std::string get_student_name(const std::string& string_data)
{
    size_t pos = string_data.find('\n');
    if (pos != std::string::npos)
        return string_data.substr(0, pos);
    else
        return string_data; // fallback
}

/// <summary>
/// Save data to a file in the required format
/// Line 1: student name
/// Line 2: timestamp (yyyy-mm-dd)
/// Line 3: key used
/// Line 4+: data
/// </summary>
void save_data_file(const std::string& filename, const std::string& student_name, const std::string& key, const std::string& data)
{
    std::ofstream file(filename);
    if (!file)
    {
        std::cerr << "Error: Could not write to file " << filename << std::endl;
        return;
    }

    // get current date
    std::time_t t = std::time(nullptr);
    std::tm* tm_ptr = std::localtime(&t);
    char date_buffer[11];
    std::strftime(date_buffer, sizeof(date_buffer), "%Y-%m-%d", tm_ptr);

    // write formatted data
    file << student_name << "\n";
    file << date_buffer << "\n";
    file << key << "\n";
    file << data << "\n";

    file.close();
}

int main()
{
    std::cout << "Encryption Decryption Test!" << std::endl;

    const std::string file_name = "inputdatafile.txt";
    const std::string encrypted_file_name = "encrypteddatafile.txt";
    const std::string decrypted_file_name = "decrypteddatafile.txt";
    const std::string key = "password";

    const std::string source_string = read_file(file_name);
    if (source_string.empty())
    {
        std::cerr << "Error: Source file is empty or missing." << std::endl;
        return 1;
    }

    const std::string student_name = get_student_name(source_string);

    // encrypt
    const std::string encrypted_string = encrypt_decrypt(source_string, key);
    save_data_file(encrypted_file_name, student_name, key, encrypted_string);

    // decrypt
    const std::string decrypted_string = encrypt_decrypt(encrypted_string, key);
    save_data_file(decrypted_file_name, student_name, key, decrypted_string);

    std::cout << "Read File: " << file_name
              << " - Encrypted To: " << encrypted_file_name
              << " - Decrypted To: " << decrypted_file_name << std::endl;

    return 0;
}
