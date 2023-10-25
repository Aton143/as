.CODE

ReadCPUTimer PROC

      PUSH  RDX
      RDTSC 
      SHL   RDX, 32
      OR    RAX, RDX
      POP   RDX
      RET

ReadCPUTimer ENDP
      END
