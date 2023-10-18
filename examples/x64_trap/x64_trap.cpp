/*
 *  Copyright (C) 2023 Antonio Martinez Arias
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 *  Most of the code is from: https://github.com/LordNoteworthy/al-khaser/blob/master/al-khaser/AntiDebug/TrapFlag.cpp 
 *  Thanks to: https://en.wikipedia.org/wiki/FLAGS_register 
 *
 *  This code also seems to only run the handler on a single step. Probably need to do more than just this :(
 *  See for a much more robust version of this: https://github.com/InsomniacGames/ig-cachesim
 */

#define WIN32_LEAN_AND_MEAN
#define _SECURE_SCL 1
#define _SECURE_SCL_THROWS 1

#include <windows.h>
#include <stdio.h>
#include "types.h"

static HANDLE ExceptionHandler;
static const __int64 SingleStepTrapFlag = 0x0100;

LONG SingleStepHandler(_EXCEPTION_POINTERS *ExceptionInfo)
{
  if (ExceptionInfo->ExceptionRecord->ExceptionCode == EXCEPTION_SINGLE_STEP)
  {
    printf("Single step!\n");
    printf("Can%s be continued\n",
           (ExceptionInfo->ExceptionRecord->ExceptionFlags == EXCEPTION_NONCONTINUABLE) ?  "not" : "");

    return(EXCEPTION_CONTINUE_EXECUTION);
  }

    return(EXCEPTION_CONTINUE_SEARCH);
}

int main()
{
  ExceptionHandler = AddVectoredExceptionHandler(1, &SingleStepHandler);

  __int64 Flags = __readeflags();
  Flags |= SingleStepTrapFlag;
  __writeeflags(Flags);

  RemoveVectoredExceptionHandler(ExceptionHandler);
  return(0);
}
