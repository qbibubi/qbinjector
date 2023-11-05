/*
	1. Select a process - Probably with PID for now
	2. Loading the DLL into program memory - Know the path of DLL and having access to it
	3. Get a handle to the process - OpenProcess
	4. Allocate memory inside address - CreateRemoteThread, WriteProcessMemory
	5. Load the DLL into the process - LoadLibrary
*/

#include <iostream>
#include <Windows.h>
#include <tchar.h>

int main() 
{
	DWORD processId = 0;
	DWORD dwSize;
	const wchar_t* dllPath = L"testdll.dll";
	
	HANDLE hProcess = OpenProcess(PROCESS_CREATE_THREAD | PROCESS_QUERY_INFORMATION | PROCESS_VM_OPERATION | PROCESS_VM_WRITE | PROCESS_VM_READ, FALSE, processId);
	if (hProcess == NULL) 
	{
		CloseHandle(hProcess);
		return 1;
	}

	LPVOID lpRemoteThreadProcess = VirtualAllocEx(hProcess, NULL, _tcslen(dllPath) + 1, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	if (lpRemoteThreadProcess == NULL)
	{
		CloseHandle(hProcess);
		return 1;
	}


	HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0, NULL, NULL, 0);



	CloseHandle(hProcess);
	return 0;
}