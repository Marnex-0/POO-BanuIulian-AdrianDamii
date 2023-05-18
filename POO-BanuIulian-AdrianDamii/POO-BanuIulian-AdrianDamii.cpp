#include "pch.h"

using namespace System;
ref class Task {
public:
	String^ TaskText;
	bool CompletionStatus;
	System::DateTime^ CreationTime;
	System::DateTime^ CompletionTime;
};
int main(array<System::String ^> ^args)
{
	Console::WriteLine("Hello World!");
}
