//
// Created by TekuConcept on October 18, 2021
//
// FWinConsole provides a convinient way to setup classical stdio.
// (meant for quick debugging pruposes only)
//

#pragma once

#include <cstdio>

class FWinConsole {
public:
	FWinConsole(const FWinConsole&) = delete;
	FWinConsole& operator=(const FWinConsole&) = delete;

	static FWinConsole* Instance();
	~FWinConsole();

	void Destroy();
	bool IsDestroyed() const { return M_IsDestroyed; }

	int Write(const char* Message, ...);

private:
	FWinConsole();
	bool M_IsDestroyed;
	static FILE* M_NewStdin;
	static FILE* M_NewStdout;
	static FILE* M_NewStderr;

	void Create();

	static bool M_CreateConsole();
	static bool M_DestroyConsole();
	static void M_ResizeConsole();
	static void M_RedirectStdout();
	static void M_RedirectStdin();
	static void M_RedirectStderr();
	static void M_LinkStdIos();
};
