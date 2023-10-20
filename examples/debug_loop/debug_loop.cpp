#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include "types.h"

void DoDebug(void)
{
}

int main(i32 ArgumentCount, char **Arguments)
{
  if (ArgumentCount < 2)
  {
    fprintf(stderr, "Error: requires a process id\n");
    return(1);
  }

  u32 ProcessID = (u32) _atoi64(Arguments[1]);
  if (DebugActiveProcess(ProcessID))
  {
    fprintf(stderr, "Successfully attached to process id: %d\n", ProcessID);
    DoDebug();

    if (DebugActiveProcessStop(ProcessID))
    {
      return(0);
    }
    else
    {
      return(1);
    }
  }
  else
  {
    fprintf(stderr, "Failed to attach to process id: %d\n%d\n", ProcessID, GetLastError());
    return(1);
  }
}
