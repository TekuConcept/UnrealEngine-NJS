//
// Created by TekuConcept on October 17, 2021
//

#include "NodeJSModule.h"

#include <cstdio>
#include <iostream>

#include "Core.h"
#include "Modules/ModuleManager.h"
#include "Interfaces/IPluginManager.h"

#define LOCTEXT_NAMESPACE "FNodeJSModule"


FNodeJSModule::FNodeJSModule():
	M_LibnodeHandle(nullptr),
	M_IsReady(false),
	M_IsEol(false) {}


inline int FNodeJSModule::StartupNodejs()
{
	// register built-in modules _before_ initializing node
	// x_register_test();

	constexpr int Argc       = 3;
	char*         Argv[Argc] = {
		"unreal-scripting",
		"-e",
		// this script runs a simple REPL server on port 25562
		"var repl=require(\"repl\"),net=require(\"net\");"
		"process.stdin.resume(),net.createServer(function(e)"
		"{var r=repl.start({prompt:\"> \",input:e,output:e});"
		"r.on&&r.on(\"exit\",()=>{e.end()}),e.on(\"error\","
		"()=>r.close())}).listen(25562);setInterval(()=>{},1)"
	};
	const int Result = node::XInitialize(Argc, Argv, M_Context);
	if (Result != 0) {
		M_IsReady = false;
		UE_LOG(LogTemp, Warning, TEXT("Failed to initialize the nodejs runtime: %d"), Result);
		return Result;
	}
	if (M_Context == nullptr) {
		M_IsReady = false;
		UE_LOG(LogTemp, Warning, TEXT("Failed to initialize context: nullptr"));
		return 1;
	}

	// initialize built-in modules _after_ initializing node
	// node::XScopedInvoke(context, [&]() {    
	// 	x_test_init(context);
	// });

	M_IsReady = true;
	return Result;
}


void FNodeJSModule::StartupModule()
{
	const FString BaseDir = IPluginManager::Get().FindPlugin("EmbeddedNodeJS")->GetBaseDir();

	FString LibraryPath;
	#if PLATFORM_WINDOWS
	LibraryPath = FPaths::Combine(*BaseDir, TEXT("Binaries/ThirdParty/NodeJS/Win64/libnode.dll"));
	#elif PLATFORM_MAC
	LibraryPath = FPaths::Combine(*BaseDir, TEXT("Binaries/ThirdParty/NodeJS/Win64/libnode.so"));
	#elif PLATFORM_LINUX
	LibraryPath = FPaths::Combine(*BaseDir, TEXT("Binaries/ThirdParty/NodeJS/Win64/libnode.dylib"));
	#else
	#error "Unsupported Platform"
	#endif // PLATFORM_WINDOWS

	if (!LibraryPath.IsEmpty()) M_LibnodeHandle = FPlatformProcess::GetDllHandle(*LibraryPath);

	if (M_LibnodeHandle == nullptr) {
		M_IsReady = false;
		UE_LOG(LogTemp, Warning, TEXT("Failed to load libnode from %ls\n"), *LibraryPath);
		return;
	}

	StartupNodejs();
}


void FNodeJSModule::Tick(float /*Duration*/)
{
	if (!M_IsReady) {
		UE_LOG(LogTemp, Display, TEXT("engine attempted tick before ready"));
		return;
	}

	const int Result = node::XStep(M_Context, M_IsEol);
	if (Result != 0) {
		M_IsReady = false;
		UE_LOG(LogTemp, Warning, TEXT("nodejs return with error: %d"), Result);
	}
	else if (M_IsEol) {
		M_IsReady = false;
		UE_LOG(LogTemp, Warning, TEXT("nodejs gracefully exited with: %d"), Result);
	}
}


inline void FNodeJSModule::ShutdownNodejs()
{
	node::XExit(M_Context);
}


void FNodeJSModule::ShutdownModule()
{
	M_IsReady = false;
	ShutdownNodejs();
	FPlatformProcess::FreeDllHandle(M_LibnodeHandle);
	M_LibnodeHandle = nullptr;
}


#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FNodeJSModule, JScripting)
