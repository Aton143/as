/*
 * NOTE(antoniom):
 * A basic VectoredExceptionHandler that u page faults
 *
 * V2: Using page faults to track memory accesses
 *
 * See below for more information:
 * https://github.com/InsomniacGames/ig-cachesim
 * https://github.com/microsoft/Detours
 * https://www.codeproject.com/Articles/3161814/x64-Memory-Access-Monitor
 * https://github.com/electronicarts/EAThread/blob/master/source/pc/eathread_callstack_win64.cpp
 */

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <stdio.h>
#include <stdint.h>

#define VEH_CALL_FIRST 1
#define VEH_CALL_LAST  0

#define ExceptionCode_FindHandlers        0x10001
#define ExceptionCode_FindHandlersContext 0x10002
static const int32_t MaxCallStackDepth = 128;

// NOTE(antoniom):
// ig-cachesim and eathread_callstack
static int32_t Backtrace(uintptr_t CallStack[], const PCONTEXT Context)
{
  ULONGLONG ImageBase;

  UNWIND_HISTORY_TABLE UnwindHistory;
  ZeroMemory(&UnwindHistory, sizeof(UnwindHistory));

  CONTEXT ContextCopy = *Context;

  int32_t CallStackDepth = 0;
  while (CallStackDepth < MaxCallStackDepth)
  {
    CallStack[CallStackDepth++] = ContextCopy.Rip;

    PRUNTIME_FUNCTION FunctionPointer = RtlLookupFunctionEntry(ContextCopy.Rip, &ImageBase, &UnwindHistory);

    if (FunctionPointer == NULL)
    {
      // NOTE(IG): This is a leaf function:
      // https://learn.microsoft.com/en-us/cpp/build/exception-handling-x64
      // "Table-based exception handling requires a table entry for all
      // functions that allocate stack space or call another function (for example, nonleaf functions)"

      // TODO(antoniom): Why is RIP supposed to be a copy of the value that RSP points to?
      ContextCopy.Rip  = *((uintptr_t *) ContextCopy.Rsp);
      ContextCopy.Rsp += 8;
    }
    else
    {
      PVOID   PCLocation       = 0;
      DWORD64 EstablisherFrame = 0;

      RtlVirtualUnwind(UNW_FLAG_NHANDLER,
                       ImageBase,
                       ContextCopy.Rip,
                       FunctionPointer,
                       &ContextCopy,
                       &PCLocation,
                       &EstablisherFrame,
                       NULL);
    }
  }

  return(CallStackDepth);
}

static LONG WINAPI FindCallHandlers(EXCEPTION_POINTERS *ExceptionInfo)
{
  if (ExceptionInfo->ExceptionRecord->ExceptionCode == ExceptionCode_FindHandlers)
  {
    // NOTE(IG): Raise another exception to get a context record that has RtlpCallVectoredHandlers in the stack
    RaiseException(ExceptionCode_FindHandlersContext, 0, 1,
                   ExceptionInfo->ExceptionRecord->ExceptionInformation);

    return(EXCEPTION_CONTINUE_EXECUTION);
  }
  else if (ExceptionInfo->ExceptionRecord->ExceptionCode == ExceptionCode_FindHandlersContext)
  {
    // Grab a backtrace from the exception we raised above:
    uintptr_t CallStack[MaxCallStackDepth] = {};

    int32_t FrameCount = Backtrace(CallStack, ExceptionInfo->ContextRecord);
    if ((0 == FrameCount) || (MaxCallStackDepth == FrameCount))
    {
      ExitProcess(1);
    }
  }

  return(EXCEPTION_CONTINUE_SEARCH);
}

int main()
{
  void *PageFaultHandlerHandle = AddVectoredExceptionHandler(VEH_CALL_FIRST, FindCallHandlers);
  if (PageFaultHandlerHandle == NULL)
  {
    ExitProcess(1);
  }

  RemoveVectoredExceptionHandler(PageFaultHandlerHandle);
  return(0);
}
