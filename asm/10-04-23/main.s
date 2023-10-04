ExitProcess PROTO ;; external proc

.CODE
arithmetic_asm PROC

     MOV  RAX, 1000b
     ADD  RAX, 14o
     MOV  RDX, [RCX]
     MUL  RDX
     RET

arithmetic_asm ENDP

     END
