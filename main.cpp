/*
	1. Select a process - Probably with PID for now
	2. Loading the DLL into program memory - Know the path of DLL and having access to it
	3. Get a handle to the process - OpenProcess
	4. Allocate memory inside address - CreateRemoteThread, WriteProcessMemory
	5. Load the DLL into the process - LoadLibrary
*/

#include <iostream>
#include <Windows.h>
#include <tlhelp32.h>

int main() 
{
	DWORD processId = 0;
	LPCSTR libraryName = "";
	
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, TRUE, processId);
	HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0, NU	LL, NULL, 0);



	CloseHandle(hProcess);
	return 0;
}