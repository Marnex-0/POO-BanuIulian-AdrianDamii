#include "pch.h"
#include "FileUtils.h"

// Definitions for common code
#define write(x) Console::WriteLine(x)

#define HLPMSG "List of options:\n help\tshows this page\n list\tlists all the tasks\n add \tadds a task to the TASKSFILE \n delete [Task Number]\tdeletes a task from the TASKSFILE, make sure the [Task Number] is a natural integer different from 0\n complete [Task Number]\tmarks a task as complete, make sure the [Task Number] is a natural integer different from 0\n"

using namespace System;
using namespace System::IO;
using namespace System::Collections::Generic;
using namespace System::Text::RegularExpressions;

// Structure that holds timestamped tasks
ref struct Task
{
    String^ TaskText;
    DateTime CreationTime;
    DateTime CompletionTime;
    String^ CompletionStatus;

};

// Function to parse timestamped tasks from a file
List<Task^>^ ParseTasks(String^ filename)
{
    List<Task^>^ text = gcnew List<Task^>();

    try
    {
        IO::StreamReader^ reader = IO::File::OpenText(filename);
        String^ line;

        // Regex patterns to match timestamps and tasks
        String^ pattern = "(.+) (\\d{1,2}/\\d{1,2}/\\d{4} \\d{1,2}:\\d{1,2}:\\d{1,2} [AP]M) (Pending Completion)";
        String^ pattern2 = "(.+) (\\d{1,2}/\\d{1,2}/\\d{4} \\d{1,2}:\\d{1,2}:\\d{1,2} [AP]M) -> (\\d{1,2}/\\d{1,2}/\\d{4} \\d{1,2}:\\d{1,2}:\\d{1,2} [AP]M) (Completed)";
        Regex^ regex = gcnew Regex(pattern);
        Regex^ regex2 = gcnew Regex(pattern2);
        

        while ((line = reader->ReadLine()) != nullptr)
        {
            // First regex matching function
            Match^ match = regex->Match(line);
            if (match->Success && match->Groups->Count == 4)
            {
                String^ TaskText = match->Groups[1]->Value;
                String^ CreationTimeStr = match->Groups[2]->Value;
                String^ CompletionStatus = match->Groups[3]->Value;

                DateTime timestamp = DateTime::Parse(CreationTimeStr);
                Task^ txt = gcnew Task();
                txt->CreationTime = timestamp;
                txt->TaskText = TaskText;
                txt->CompletionStatus = CompletionStatus;

                text->Add(txt);
            }

            // Second regex matching function
            Match^ match2 = regex2->Match(line);
            if (match2->Success && match2->Groups->Count == 5)
            {
                String^ TaskText2 = match2->Groups[1]->Value;
                String^ CreationTimeStr2 = match2->Groups[2]->Value;
                String^ CompletionTimeStr = match2->Groups[3]->Value;
                String^ CompletionStatus2 = match2->Groups[4]->Value;

                DateTime timestamp2 = DateTime::Parse(CreationTimeStr2);
                DateTime timestamp3 = DateTime::Parse(CompletionTimeStr);
                Task^ txt = gcnew Task();
                txt->CreationTime = timestamp2;
                txt->CompletionTime = timestamp3;
                txt->TaskText = TaskText2;
                txt->CompletionStatus = CompletionStatus2;

                text->Add(txt);
            }
        }

        reader->Close();
    }
    catch (Exception^ ex)
    {
        write("Error: " + ex->Message);
    }

    return text;
}


void CompleteTask(String^ filename, int linenr) {

    String^ linetxt;

        try {
            array<String^>^ alllines = File::ReadAllLines(filename);

            if (linenr > 0 && linenr <= alllines->Length) {
                String^ pattern = "(.+) (\\d{1,2}/\\d{1,2}/\\d{4} \\d{1,2}:\\d{1,2}:\\d{1,2} [AP]M)";
                Regex^ regex = gcnew Regex(pattern);

                Match^ match = regex->Match(alllines[linenr - 1]);
                if (match->Success && match->Groups->Count == 3) {
                    String^ TaskText = match->Groups[1]->Value;
                    String^ CreationTime = match->Groups[2]->Value;
                    DateTime timestamp = DateTime::Parse(CreationTime);
                    Task^ txt = gcnew Task();
                    txt->CreationTime = timestamp;
                    txt->CompletionTime = DateTime::Now;
                    txt->TaskText = TaskText;
                    txt->CompletionStatus = "Completed";
                    linetxt = txt->TaskText + " " + txt->CreationTime + " -> " + txt->CompletionTime + " " + txt->CompletionStatus;
                    alllines->SetValue(linetxt, linenr - 1);
                    File::WriteAllLines(filename, alllines);
                }
            }
        }
    catch (Exception^ ex) {
        write("Error: " + ex);
    }
}

void Arguments(array<System::String^>^ args) {
    String^ fileName = "TASKSFILE";
    if (args[0] == "help") {
        write(HLPMSG);
    }

    else if (args[0] == "list") {
        List<Task^>^ task = ParseTasks(fileName);
        for each (Task ^ txt in task)
        {
            DateTime CDate = txt->CreationTime;
            DateTime CDate2 = txt->CompletionTime;
            if (txt->CompletionStatus == "Completed")
                write("Task : \" " + txt->TaskText + "\" \n" + "Timestamp: " + txt->CreationTime + " -> " + txt->CompletionTime + ", Completion Status: " + txt->CompletionStatus);
            else write("Task : \" " + txt->TaskText+ "\" \n" + "Timestamp: " + txt->CreationTime + ", Completion Status: " + txt->CompletionStatus);
        }
    }
    else if (args[0] == "add") {
        String^ TaskText;
        write("Describe the task:");
        TaskText = Console::ReadLine();
        AddLine("TASKSFILE", TaskText + " " + System::DateTime::Now +" Pending Completion");
    }

    else if (args[0] == "delete") {
        Int32 num = Convert::ToInt32(args[1]);
        DeleteLine(fileName, num);
    }

    else if (args[0] == "complete") {
        Int32 num = Convert::ToInt32(args[1]);
        CompleteTask(fileName, num);
    }

    else write(HLPMSG);

}   

int main(array<System::String^>^ args)
{
    VerifyCreateFile("TASKSFILE");
   
    if (args->Length > 0 && args->Length < 3) Arguments(args); else write(HLPMSG);
}
