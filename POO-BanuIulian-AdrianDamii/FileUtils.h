#include "pch.h";

using namespace System::IO;
using namespace System;

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