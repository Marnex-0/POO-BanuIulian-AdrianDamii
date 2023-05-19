#include "pch.h";
#define write(x) Console::WriteLine(x)

using namespace System::IO;
using namespace System;

void VerifyCreateFile(String^ fileName)
{
    String^ filePath = fileName;

    try
    {
        if (!File::Exists(filePath))
        {
            throw gcnew FileNotFoundException("The file does not exist or cannot be found, make sure the file is in the same directory as the program");
        }
    }
    catch (FileNotFoundException^ ex)
    {
        write("Do you want to create a new TASKSFILE ? (Y/N)");

        String^ response = Console::ReadLine();
        if (response->ToUpper() == "Y")
        {
            try
            {
                FileStream^ fileStream = File::Create(filePath);
                fileStream->Close();
                write("File created successfully.");
            }
            catch (Exception^ ex)
            {
                write("An error occurred while creating the file: ", ex->Message);
            }
        }
        else write("The file has not been created.");
    }

    catch (Exception^ ex)
    {
        write("An error occurred: ", ex->Message);
    }
}

void AddLine(String^ filePath, String^ line)
{
    try
    {

        StreamWriter^ writer = gcnew StreamWriter(filePath, true);

        writer->WriteLine(line);

        writer->Close();

        write("Task added to TASKSFILE");
    }
    catch (Exception^ ex)
    {
        write("Error: ", ex->Message);
    }
}

void DeleteLine(String^ filePath, int lineNumber)
{
    try
    {
        array<String^>^ lines = File::ReadAllLines(filePath);

        if (lineNumber > 0 && lineNumber - 1 <= lines->Length)
        {
            lines->SetValue("", lineNumber - 1);

            File::WriteAllLines(filePath, lines);

            write("Task "+ lineNumber +" deleted from file.");
        }
        else
        {
            write("Invalid line number.");
        }
    }
    catch (Exception^ ex)
    {
        write("Deletion Error: ", ex->Message);
    }
}
