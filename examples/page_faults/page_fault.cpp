/*
 * NOTE(antoniom):
 * A basic VectoredExceptionHandler that recovers from a page fault
 */

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <stdio.h>
#include <stdint.h>

#define VEH_CALL_FIRST 1
#define VEH_CALL_LAST  0

/*
 * ExceptionInfo->ExceptionRecord->ExceptionAddress gets RIP value
 */

LONG PageFaultHandler(EXCEPTION_POINTERS *ExceptionInfo)
{
  if (ExceptionInfo->ExceptionRecord->ExceptionCode == EXCEPTION_ACCESS_VIOLATION)
  {
    void *FaultingAddress = (void *) ExceptionInfo->ExceptionRecord->ExceptionInformation[1];
    printf("PAGE FAULT: (%s) 0x%p\n", ExceptionInfo->ExceptionRecord->ExceptionInformation[0] == 0 ? "Read" : "Write", FaultingAddress);

    MEMORY_BASIC_INFORMATION PageInfo;
    VirtualQuery((void *) ((uint64_t) FaultingAddress & ~4095), &PageInfo, sizeof(PageInfo));
    VirtualAlloc(FaultingAddress, 4096, MEM_COMMIT, PAGE_READWRITE);
  }

  return(EXCEPTION_CONTINUE_EXECUTION);
}

int main()
{
  void *PageFaultHandlerHandle = AddVectoredExceptionHandler(VEH_CALL_FIRST, PageFaultHandler);
  if (PageFaultHandlerHandle == NULL)
  {
    ExitProcess(1);
  }

  SYSTEM_INFO SysInfo;
  GetSystemInfo(&SysInfo);

  DWORD PageSize = SysInfo.dwPageSize;

  void *Memory = VirtualAlloc(NULL, PageSize * 3, MEM_RESERVE, PAGE_NOACCESS);
  VirtualAlloc(Memory, PageSize, MEM_COMMIT, PAGE_READWRITE);

  uint8_t *NextPage = ((uint8_t *) Memory) + PageSize;
  printf("Next Page: 0x%p\n", NextPage);
  *NextPage = 0;
  *(NextPage + 1) = 1;

  NextPage = ((uint8_t *) Memory) + (2 * PageSize);
  printf("Next Page: 0x%p\n", NextPage);
  *NextPage = 2;
  *(NextPage + 1) = 3;

  RemoveVectoredExceptionHandler(PageFaultHandlerHandle);
  return(0);
}
