#include <iostream>
#include <windows.h>


int main()
{
	HWND hWindow = FindWindowA(NULL, "AssaultCube");
	DWORD processId;
	GetWindowThreadProcessId(hWindow, &processId);
	
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId);

	// Inject DLL into the suspended process
	LPCSTR dllPath = "D:\\dev\\assaultqb\\assaultqb\\bin\\Win32\\Debug\\assaultqb.dll";
	SIZE_T dllSize = strlen(dllPath) + 1;
	LPVOID lpDllPath = VirtualAllocEx( hProcess, 0, dllSize, MEM_COMMIT, PAGE_READWRITE );

	if (lpDllPath == NULL)
	{
		std::cout << "Dll path is empty" << std::endl;
		return EXIT_FAILURE;
	}

	if (!WriteProcessMemory(
		hProcess,
		lpDllPath,
		dllPath,
		dllSize,
		0)
		) 
	{
		std::cout << "Failed to write process memoery" << std::endl;
		return EXIT_FAILURE;
	}

	HMODULE hKernel32 = GetModuleHandleA("Kernel32.dll");
	if (hKernel32 == NULL) 
	{
		std::cout << "Handle error, unable to get a handle to Kernel32" << std::endl;
		return EXIT_FAILURE;
	}

	FARPROC pfnLoadLibraryA = GetProcAddress(hKernel32, "LoadLibraryA");
	if (pfnLoadLibraryA == NULL) 
	{
		std::cout << "Handle error, unable to get the address of LoadLibraryA" << std::endl;
		return EXIT_FAILURE;
	}

	HANDLE hLoadThread = CreateRemoteThread(
		hProcess,
		0,
		0,
		reinterpret_cast<LPTHREAD_START_ROUTINE>(pfnLoadLibraryA),
		lpDllPath,
		NULL,
		NULL
	);
	std::cout << lpDllPath << std::endl;

	VirtualFreeEx(hProcess, lpDllPath, 0, MEM_RELEASE);

	// Wait for child process to exit
	WaitForSingleObject(hProcess, INFINITE);
	CloseHandle(hProcess);
	
	return EXIT_SUCCESS;
}