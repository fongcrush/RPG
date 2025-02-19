// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

DECLARE_LOG_CATEGORY_EXTERN(RPG, Log, All);

#define LOG(Format, ...) UE_LOG(RPG, Log, TEXT(Format), ##__VA_ARGS__)
#define LOG_WARNING(Format, ...) UE_LOG(RPG, Warning, TEXT(Format), ##__VA_ARGS__)
#define LOG_ERROR(Format, ...) UE_LOG(RPG, Error, TEXT(Format), ##__VA_ARGS__)

#define CALLINFO_S (FString(__FUNCTION__) + TEXT("(") + FString::FromInt(__LINE__) + TEXT(")"))
#define CALLINFO UE_LOG(RPG, Log, TEXT("%s"), *CALLINFO_S)
#define CALLINFO_WARNING UE_LOG(RPG, Warning, TEXT("%s"), *CALLINFO_S)
#define CALLINFO_ERROR UE_LOG(RPG, Error, TEXT("%s"), *CALLINFO_S)

#define LOG_CALLINFO(Format, ...) UE_LOG(RPG, Log, TEXT("%s %s"), *CALLINFO_S, *FString::Printf(TEXT(Format), ##__VA_ARGS__))
#define LOG_CALLINFO_WARNING(Format, ...) UE_LOG(RPG, Warning, TEXT("%s %s"), *CALLINFO_S, *FString::Printf(TEXT(Format), ##__VA_ARGS__))
#define LOG_CALLINFO_ERROR(Format, ...) UE_LOG(RPG, Error, TEXT("%s %s"), *CALLINFO_S, *FString::Printf(TEXT(Format), ##__VA_ARGS__))

#define	LOG_SCREEN(Format, ...) GEngine->AddOnScreenDebugMessage(1, 10.f, FColor::Red, FString::Printf(TEXT(Format), ##__VA_ARGS__));

#define LOG_CHECK(Expr, ...) { if((Expr) { LOG_ERROR(TEXT("ASSERTION : %s"), TEXT("'"#Expr"'")); return __VA_ARGS__; } }

#define VERIFY(Expr) verifyf(Expr != nullptr, TEXT("%s ASSERTION : %s"), *CALLINFO_S, TEXT("'"#Expr"'"))
#define VERIFYF(Expr, Format, ...) verifyf(Expr != nullptr, TEXT("%s ASSERTION : %s, %s"), *CALLINFO_S, TEXT("'"#Expr"'"), *FString::Printf(TEXT(Format), ##__VA_ARGS__))
#define VERIFY_EQUAL(Val1, Val2) verifyf(Val1 == Val2, TEXT("%s ASSERTION : %s != %s"), *CALLINFO_S, TEXT("'"#Val1"'"), TEXT("'"#Val2"'"))
#define VERIFYF_EQUAL(Val1, Val2, Format, ...) verifyf(Val1 == Val2, TEXT("%s ASSERTION : %s != %s, %s"), *CALLINFO_S, TEXT("'"#Val1"'"), TEXT("'"#Val2"'"), *FString::Printf(TEXT(Format), ##__VA_ARGS__))

#define CALLSTACK \
	static constexpr SIZE_T StackTraceSize = 65535;\
	ANSICHAR StackTrace[StackTraceSize];\
	FPlatformStackWalk::StackWalkAndDump(StackTrace, StackTraceSize, 0);\
	FString OutCallstack = ANSI_TO_TCHAR(StackTrace);\
	UE_LOG(LogTemp, Warning, TEXT("Call Stack:\n%s"), *OutCallstack)

#define LOG_CALLSTACK(Format, ...) UE_LOG(RPG, Log, TEXT("%s"), *FString::Printf(TEXT(Format), ##__VA_ARGS__)); CALLSTACK

template <typename T, typename U>
T* StructCast(U* Source)
{
	return (Source && std::is_base_of_v<T, U>) ? static_cast<T*>(Source) : nullptr;
}
