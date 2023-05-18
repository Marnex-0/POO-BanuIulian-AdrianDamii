#include "pch.h";

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace System::IO;
using namespace System;

void DeleteLineInFile(const std::string& fileName, int lineNumber, std::string& deletedContent) {
    // Open the original file for reading
    std::ifstream inputFile(fileName);
    if (!inputFile) {
        std::cout << "Failed to open the file." << std::endl;
        return;
    }

    std::string line;
    int currentLine = 1;
    std::vector<std::string> lines;

    // Read each line from the original file
    while (std::getline(inputFile, line)) {
        // Store the line content except the line to be deleted
        if (currentLine != lineNumber) {
            lines.push_back(line);
        }
        else {
            deletedContent = line;
        }
        currentLine++;
    }

    // Close the file
    inputFile.close();

    // Open the file for writing
    std::ofstream outputFile(fileName);
    if (!outputFile) {
        std::cout << "Failed to open the file for writing." << std::endl;
        return;
    }

    // Write the modified content to the file
    for (const auto& line : lines)
    {
        outputFile << line << std::endl;
    }

    // Close the file
    outputFile.close();
    std::cout << "Line " << lineNumber << " deleted from the file." << std::endl;
}


void VerifyAndCreateFile()
{
    String^ fileName = "file.txt";
    String^ filePath = Path::Combine(Environment::CurrentDirectory, fileName);

    try
    {
        // Check if the file exists
        if (File::Exists(filePath))
        {
            Console::WriteLine("File already exists.");
        }
        else
        {
            throw gcnew FileNotFoundException("The file is deleted or is in another directory!");
        }
    }
    catch (FileNotFoundException^ ex)
    {
        Console::WriteLine("An error occurred: {0}", ex->Message);
        Console::WriteLine("Do you want to create a new file with the same name? (Y/N)");

        String^ response = Console::ReadLine();
        if (response->ToUpper() == "Y")
        {
            try
            {
                // Create a new file
                FileStream^ fileStream = File::Create(filePath);
                fileStream->Close();
                Console::WriteLine("File created successfully.");
            }
            catch (Exception^ ex)
            {
                Console::WriteLine("An error occurred while creating the file: {0}", ex->Message);
            }
        }
    }
    catch (Exception^ ex)
    {
        Console::WriteLine("An error occurred: {0}", ex->Message);
    }
}