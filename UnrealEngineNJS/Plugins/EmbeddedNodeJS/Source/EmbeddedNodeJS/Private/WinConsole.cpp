//
// Created by TekuConcept on October 18, 2021
//

#if PLATFORM_WINDOWS

#include "WinConsole.h"
#include "Logging/LogMacros.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <fcntl.h>
#include <io.h>
#include <iostream>


FILE* FWinConsole::M_NewStdin  = nullptr;
FILE* FWinConsole::M_NewStdout = nullptr;
FILE* FWinConsole::M_NewStderr = nullptr;


FWinConsole* FWinConsole::Instance()
{
	static FWinConsole Instance;
	Instance.Create();
	return &Instance;
}


FWinConsole::FWinConsole() : M_IsDestroyed(true) { }


FWinConsole::~FWinConsole()
{
	Destroy();
}


void FWinConsole::Create()
{
	const auto Result = M_CreateConsole();
	UE_LOG(LogTemp, Display, TEXT("CreatedConsole: %d"), Result);
	M_ResizeConsole();
	M_RedirectStdout();
	M_RedirectStdin();
	M_RedirectStderr();
	M_LinkStdIos();
	M_IsDestroyed = false;
}


void FWinConsole::Destroy()
{
	if (M_IsDestroyed) return;
	M_DestroyConsole();
	M_IsDestroyed = true;
}


int FWinConsole::Write(const char* Message, ...)
{
	va_list Args;
	va_start(Args, Message);
	const int Result = vfprintf(M_NewStdout, Message, Args);
	fflush(M_NewStdout);
	UE_LOG(LogTemp, Warning, TEXT("Wrote %d chars"), Result);
	va_end(Args);
	return Result;
}


bool FWinConsole::M_CreateConsole() { return AllocConsole(); }


bool FWinConsole::M_DestroyConsole() { return FreeConsole(); }


void FWinConsole::M_ResizeConsole()
{
	constexpr WORD             MaxConsoleLines = 1024;
	CONSOLE_SCREEN_BUFFER_INFO ConsoleInfo;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &ConsoleInfo);
	ConsoleInfo.dwSize.Y = MaxConsoleLines;
	SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), ConsoleInfo.dwSize);
}


void FWinConsole::M_RedirectStdout()
{
	const intptr_t LStdHandle = reinterpret_cast<intptr_t>(GetStdHandle(STD_OUTPUT_HANDLE));
	const int      HConHandle = _open_osfhandle(LStdHandle, _O_WRONLY/*_O_TEXT*/);
	if (HConHandle <= 0) {
		UE_LOG(LogTemp, Warning, TEXT("Failed to get stdout handle"));
		return;
	}
	freopen_s(&M_NewStdout, "CONOUT$", "a", stdout);
	if (!M_NewStdout) {
		UE_LOG(LogTemp, Warning, TEXT("Failed to open stdout handle"));
		return;
	}
	setvbuf(stdout, nullptr, _IONBF, 0);
}


void FWinConsole::M_RedirectStdin()
{
	const intptr_t LStdHandle = reinterpret_cast<intptr_t>(GetStdHandle(STD_INPUT_HANDLE));
	const int      HConHandle = _open_osfhandle(LStdHandle, _O_TEXT);
	if (HConHandle <= 0) {
		UE_LOG(LogTemp, Warning, TEXT("Failed to get stdin handle"));
		return;
	}
	freopen_s(&M_NewStdin, "CONIN$", "r", stdin);
	if (!M_NewStdin) {
		UE_LOG(LogTemp, Warning, TEXT("Failed to open stdin handle"));
		return;
	}
	setvbuf(stdin, nullptr, _IOLBF, BUFSIZ);
}


void FWinConsole::M_RedirectStderr()
{
	const intptr_t LStdHandle = reinterpret_cast<intptr_t>(GetStdHandle(STD_ERROR_HANDLE));
	const int      HConHandle = _open_osfhandle(LStdHandle, _O_TEXT);
	if (HConHandle <= 0) {
		UE_LOG(LogTemp, Warning, TEXT("Failed to get stderr handle"));
		return;
	}
	freopen_s(&M_NewStderr, "CONERR$", "a", stderr);
	if (!M_NewStderr) {
		UE_LOG(LogTemp, Warning, TEXT("Failed to open stderr handle"));
		return;
	}
	setvbuf(stderr, nullptr, _IOLBF, 0);
}


void FWinConsole::M_LinkStdIos()
{
	std::ios::sync_with_stdio();
}


#endif /* PLATFORM_WINDOWS */
