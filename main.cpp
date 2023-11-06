/*
	1. Select a process - Probably with PID for now
	2. Loading the DLL into program memory - Know the path of DLL and having access to it
	3. Get a handle to the process - OpenProcess
	4. Allocate memory inside address - CreateRemoteThread, WriteProcessMemory
	5. Load the DLL into the process - LoadLibrary
*/


#include <iostream>
#include <windows.h>


int main() 
{	
	DWORD processId{ 12852 }; //temporary id
	LPCSTR dllPath = "lib//unabomber.dll";
	SIZE_T dllSize = strlen(dllPath) + 1;

	HANDLE hProcess = OpenProcess( PROCESS_ALL_ACCESS , FALSE, processId );
	LPVOID lpDllPath = VirtualAllocEx( hProcess, NULL, dllSize, MEM_COMMIT , PAGE_READWRITE);

	WriteProcessMemory( hProcess, lpDllPath, reinterpret_cast<LPCVOID>( dllPath ), dllSize, NULL );

	HANDLE hLoadThread = CreateRemoteThread(
		hProcess, 
		NULL, 
		NULL, 
		reinterpret_cast<LPTHREAD_START_ROUTINE>( GetProcAddress( GetModuleHandleA( "Kernel32.dll" ), "LoadLibraryA" ) ),
		lpDllPath,
		NULL,
		NULL
	);
	std::cout << lpDllPath << std::endl;
	std::cin.get();

	VirtualFreeEx( hProcess, NULL, dllSize, MEM_RELEASE );
	CloseHandle( hProcess );
	return EXIT_SUCCESS;
}