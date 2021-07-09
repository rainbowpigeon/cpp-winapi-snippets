#include <Windows.h>
#include <wchar.h>
#include <Shlwapi.h>
#pragma comment(lib, "shlwapi.lib")

constexpr auto CONSOLE_INPUT_BUF_LEN = 128;

LPWSTR get_user_input(LPCWSTR prompt);

LPWSTR get_user_input(LPCWSTR prompt) {
	HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
	if (hStdin != INVALID_HANDLE_VALUE) {
		wprintf(prompt);
		wchar_t* input_buffer = (wchar_t*)calloc(1, CONSOLE_INPUT_BUF_LEN * sizeof(TCHAR) + 2);
		if (input_buffer != NULL) {
			DWORD nchars_read;
			ReadConsoleW(
				hStdin,
				input_buffer,
				CONSOLE_INPUT_BUF_LEN,
				&nchars_read,
				NULL
			);
			input_buffer[StrCSpnW(input_buffer, L"\r\n")] = 0;
			return input_buffer; // caller will free buffer
		}
	}
	return NULL;
}
