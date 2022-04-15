       JMP LOADIT
PAB1   DATA >0500,>0000,>0000,>1800,>000C
       TEXT 'DSK1.TITLE_P'
PAB2   DATA >0500,>1F00,>0000,>17E0,>000C
       TEXT 'DSK1.TITLE_C'
TEMP1  BSS >80
LOADIT LI R0,>01A0
       BLWP @VWTR
       LI R0,>0002
       BLWP @VWTR
       LI R0,>0206
       BLWP @VWTR
       LI R0,>03FF
       BLWP @VWTR
       LI R0,>0403
       BLWP @VWTR
       LI R0,>0536
       BLWP @VWTR
       LI R0,>1B00
       LI R1,>D000
       BLWP @VSBW
       LI R0,>1800
       CLR R2
NQ     CLR R1
LP     BLWP @VSBW
       INC R0
       AI R1,>0100
       CI R1,>0000
       JNE LP
       INC R2
       CI R2,3
       JNE NQ
       LI R0,>3780
       LI R1,TEMP1
       LI R2,>80
       BLWP @VMBR
       LI R0,>1B02
       LI R1,PAB1
       LI R2,23
       BLWP @VMBW
       LI R6,>1B0B
       MOV R6,@>8356
       BLWP @DSRLNK
       DATA 8
       LI R0,>1B02
       LI R1,PAB2
       LI R2,23
       BLWP @VMBW
       LI R6,>1B0B
       MOV R6,@>8356
       BLWP @DSRLNK
       DATA 8
       LI R3,>36FF
       LI R4,>37FF
FIX    MOV R3,R0
       BLWP @VSBR
       MOV R4,R0
       BLWP @VSBW
       DEC R4
       DEC R3
       CI R4,>1FFF
       JNE FIX
       LI R0,>01E2
       BLWP @VWTR
