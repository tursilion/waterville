CC = sdcc
CFLAGS = -mz80 -c "-I../../../include" --std-sdcc99 --opt-code-speed --vc
AS = as-z80
AFLAGS = -plosgff

.PHONY: all clean

objs = c99compat.o final_c.o final_p.o title_c.o title_p.o wr.o

all: build
	/cygdrive/c/work/coleco/tursi/makemegacart/debug/makemegacart.exe crt0.ihx wr.rom

build: $(objs)
	$(CC) -mz80 --no-std-crt0 --code-loc 0x8100 --data-loc 0x7000 "../../../bin/libcvu.lib" "../../../bin/libcv.lib" "../../../bin/crt0.o" *.o

wr.o: wr.c c99compat.h allpics.h
	$(CC) $(CFLAGS) wr.c

c99compat.c: c99compat.h chara1.c
	$(CC) $(CFLAGS) c99compat.c
	
# extract useful information from the bin2inc files
allpics.h: final_c.c final_p.c title_c.c title_p.c
	grep -h "#define SIZE_OF" >allpics.h final_c.c final_p.c title_c.c title_p.c
	grep -h "const unsigned char" final_c.c final_p.c title_c.c title_p.c | sed "s/\(.*\)=.*/extern \1 ;/" >>allpics.h

.o.c: $<.c *.h
	$(CC) $(CFLAGS) $<

clean:
	rm -f *.rel *.map *.lst *.lnk *.sym *.asm *~ *.o *.ihx *.sprite.* *.rom allpics.h
