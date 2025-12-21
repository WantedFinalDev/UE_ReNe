// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

/** Main log category used across the project */
DECLARE_LOG_CATEGORY_EXTERN(Rene, Log, All);

/*
- DECLARE_LOG_CATEGORY_EXTERN(Name, DefaultVerbosity, CompileTimeVerbosity):
  UE 로그 카테고리를 다른 모듈/파일에서 참조 가능하도록 외부 선언합니다. 구현 파일에서 DEFINE_LOG_CATEGORY로 정의합니다.
  
  .cpp
- DEFINE_LOG_CATEGORY(Name): 선언된 로그 카테고리를 실제로 정의합니다.

- ANSI_TO_TCHAR(AnsiStr): `char*`(ANSI) → `TCHAR*`로 변환합니다. `__FILE__`, `__FUNCTION__`은 보통 ANSI이므로 변환이 필요합니다.

*/

// 경고 레벨 로그
#ifndef LOGWARN
#define LOGWARN() \
UE_LOG(Rene, Warning, TEXT("%s[%d]"), \
ANSI_TO_TCHAR(__FUNCTION__), \
__LINE__)
#endif

#ifndef LOGWARNF
#define LOGWARNF(Format, ...) \
UE_LOG(Rene, Warning, TEXT("%s[%d] | " Format), \
ANSI_TO_TCHAR(__FUNCTION__), \
__LINE__, \
##__VA_ARGS__)
#endif

// 오류 레벨 로그
#ifndef LOGERROR
#define LOGERROR() \
UE_LOG(Rene, Error, TEXT("%s[%d]"), \
ANSI_TO_TCHAR(__FUNCTION__), \
__LINE__)
#endif

#ifndef LOGERRORF
#define LOGERRORF(Format, ...) \
UE_LOG(Rene, Error, TEXT("%s[%d] | " Format), \
ANSI_TO_TCHAR(__FUNCTION__), \
__LINE__, \
##__VA_ARGS__)
#endif