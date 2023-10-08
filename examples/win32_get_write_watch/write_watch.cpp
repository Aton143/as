#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <memoryapi.h>

#include <stdint.h>
#include <stdio.h>

/*
   UINT GetWriteWatch(
  [in]      DWORD     dwFlags,
  [in]      PVOID     lpBaseAddress,
  [in]      SIZE_T    dwRegionSize,
  [out]     PVOID     *lpAddresses,
  [kin, out] ULONG_PTR *lpdwCount,
  [out]     LPDWORD   lpdwGranularity
);
*/

#define array_count(arr) (sizeof(arr) / sizeof(arr[0]))

int main()
{
  const size_t memory_size = 1ULL << 20ULL;
  uint8_t *memory_base = (uint8_t *) VirtualAlloc(NULL,
                                                  memory_size,
                                                  MEM_COMMIT | MEM_RESERVE | MEM_WRITE_WATCH,
                                                  PAGE_READWRITE);

  uint8_t **pages_touched[memory_size / 4096] = {};
  ULONG_PTR pages_touched_count = array_count(pages_touched);
  DWORD     page_size = 0;

  UINT get_write_watch_result = GetWriteWatch(WRITE_WATCH_FLAG_RESET,
                                              memory_base,
                                              memory_size,
                                              (void **) pages_touched,
                                              &pages_touched_count,
                                              &page_size);

  if (get_write_watch_result == 0)
  {
    for (UINT_PTR page_start = (UINT_PTR) memory_base;
         page_start < (UINT_PTR) memory_base + memory_size;
         page_start += page_size << 4)
    {
      *((uint8_t *) page_start) = 1;
    }

    pages_touched_count = array_count(pages_touched);
    get_write_watch_result = GetWriteWatch(0,
                                           memory_base,
                                           memory_size,
                                           (void **) pages_touched,
                                           &pages_touched_count,
                                           &page_size);

    if (get_write_watch_result == 0)
    {
      for (int pages_touched_index = 0;
           pages_touched_index < pages_touched_count;
           ++pages_touched_index)
      {
        printf("page base: 0x%p\n", pages_touched[pages_touched_index]);
      }
    }
  }

  return(0);
}
