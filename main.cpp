#include <iostream>
#include <windows.h>


int main() 
{	
	// Create suspended process of Assault Cube
	LPCWSTR assaultCube = L"C:\\Program Files (x86)\\AssaultCube 1.3.0.2\\assaultcube.bat";
	STARTUPINFO startupInfo;
	PROCESS_INFORMATION processInfo;

	// Set the memory of startupInfo to zero
	ZeroMemory(&startupInfo, sizeof(startupInfo));
	startupInfo.cb = sizeof(startupInfo);

	BOOL createProcess = CreateProcessW(
		assaultCube, 
		NULL, 
		NULL, 
		NULL, 
		NULL, 
		CREATE_SUSPENDED, 
		NULL, 
		L"C:\\Program Files (x86)\\AssaultCube 1.3.0.2",
		&startupInfo, 
		&processInfo
	);

	if (!createProcess) {
		MessageBoxW(NULL, L"Failed to create process", L"Error", MB_ICONERROR);
		return EXIT_FAILURE;
	}

	// Inject DLL into the suspended process
	LPCSTR dllPath = "D:\\dev\\qbinjector\\lib\\assaultqb.dll";
	SIZE_T dllSize = strlen(dllPath) + 1;
	LPVOID lpDllPath = VirtualAllocEx( processInfo.hProcess, NULL, dllSize, MEM_COMMIT , PAGE_READWRITE);

	WriteProcessMemory(
		processInfo.hProcess, 
		lpDllPath, 
		reinterpret_cast<LPCVOID>( dllPath ), 
		dllSize, 
		NULL
	);

	HANDLE hLoadThread = CreateRemoteThread(
		processInfo.hProcess,
		NULL, 
		NULL, 
		reinterpret_cast<LPTHREAD_START_ROUTINE>( GetProcAddress( GetModuleHandleA( "Kernel32.dll" ), "LoadLibraryA" ) ),
		lpDllPath,
		NULL,
		NULL
	);
	std::cout << lpDllPath << " " << processInfo.hProcess << std::endl;
	MessageBoxW(NULL , L"Sucesfully injected at ", L"qbinjector", MB_ICONINFORMATION);
	ResumeThread(processInfo.hThread);

	VirtualFreeEx( processInfo.hProcess, NULL, dllSize, MEM_RELEASE );
	CloseHandle( processInfo.hProcess );
	return EXIT_SUCCESS;
}