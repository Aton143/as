#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>

int main()
{
  printf("Process ID: %d\n", GetProcessId(GetCurrentProcess()));

  while (true)
  {
  }

  return(0);
}
