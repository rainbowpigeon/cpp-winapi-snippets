#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <string>

HMODULE g_hDll = NULL;

extern "C" __declspec(dllexport) void init();

BOOL APIENTRY DllMain(
	HMODULE hModule,
	DWORD ul_reason_for_call,
	LPVOID lpReserved
) {
	switch (ul_reason_for_call) {
	case DLL_PROCESS_ATTACH:
		g_hDll = hModule;
		GetModuleHandleExW(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS, (LPCWSTR)DllMain, &hModule);
		CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)init, NULL, 0, NULL);

	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
}

extern "C" __declspec(dllexport) void init() {
	std::wstring arg = L"";
	PROCESS_INFORMATION piProcInfo;
	STARTUPINFO siStartInfo;
	BOOL bSuccess = FALSE;

	ZeroMemory(&piProcInfo, sizeof(PROCESS_INFORMATION));
	ZeroMemory(&siStartInfo, sizeof(STARTUPINFO));
	siStartInfo.cb = sizeof(STARTUPINFO);

	bSuccess = CreateProcessW(
		NULL,
		&arg[0],
		NULL,
		NULL,
		FALSE,
		0,
		NULL,
		NULL,
		&siStartInfo,
		&piProcInfo
	);

	if (bSuccess) {
		WaitForSingleObject(piProcInfo.hProcess, INFINITE);
		CloseHandle(piProcInfo.hThread);
		CloseHandle(piProcInfo.hProcess);
	}
	FreeLibraryAndExitThread(g_hDll, 0);
}
