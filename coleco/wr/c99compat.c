#include "cv.h"
#include "cvu.h"
#include "c99compat.h"

struct cv_controller_state cs;
cv_vmemp pMus = 0;
cv_vmemp scrnpos = 0;
int nMusCnt=0;
volatile sfr at 0xff audport;

unsigned char tmpbuf[8];
volatile unsigned char nFrame;
volatile int8 step=99;		// magic flag

#include "chara1.c"

// enables and disables the vblank interrupt so we can run safely without all
// the flags and crap.
void cv_vdpout(const unsigned char reg, const unsigned char data1);
extern uint8_t cv_vdpreg[2];	// in CV lib
void cv_set_vint_active(bool active) {
	cv_vdpout(0x1, cv_vdpreg[1] = (active ? 0x20 : 0x0) | (cv_vdpreg[1] & ~0x20));
}

void nmi(void)
{
	// release any waiting code
	nFrame++;
	step = true;
}

// replacement for LIMI, but also delays a frame
void waitforstep() {
	if (step == 99) {
		// NMI not set up yet, so install it
		cv_set_vint_handler(nmi);
		step=false;
	}

	// enable vblank
	cv_set_vint_active(true);

	// wait for one to occur
	while (!step);

	// disable vblank again
	cv_set_vint_active(false);

	// clear the flag (we do it last only to 
	// minimize time wasted before running doVBlank
	step=false;
}

#if 0
// one interrupt of music
void doMusic() {
	int i,size;

	if (0 == pMus) {
		return;
	}
	nMusCnt--;
	if (nMusCnt > 0) {
		return;
	}

	cv_set_read_vram_address(pMus);

	i=cv_vinb();
	size=i;

	if (i == 0) {
		pMus=0;
		return;
	}

	while ((i--) > 0) {
		audport=cv_vinb();
	}

	nMusCnt=cv_vinb();
	pMus+=size+2;
}

void StartMusic(int p) {
	nMusCnt=1;
	pMus=p;
}
#endif

// loads a default ASCII character set from ROM
void loadcharset() {
	// this loads the ColecoVision ROM font
	cvu_memtovmemcpy(gPATTERN+(32*8), 0x15a3, 96*8);
}

void chrdef(int n, const unsigned char *sz) {
	unsigned char a,b;
	unsigned char cnt;
	cv_set_write_vram_address(gPATTERN+(n<<3));

	cnt=8;
	while ((*sz)&&(*(sz+1))) {
		a=*(sz++)-'0';
		b=*(sz++)-'0';
		if (a>9) a-=7;
		if (b>9) b-=7;
		// Strange.. this masking was needed (only for b) ...
		// without it, b was always setting bit 0x20...
		b&=0x0f;
		a&=0x0f;
		cv_voutb((a<<4)|b);
		cnt--;
		if (cnt == 0) cnt=8;
	}
	// handle rest of a partial pattern
	if (cnt != 8) {
		while (cnt--) {
			cv_voutb(0);
		}
	}
}

// copies a VDP character, pattern and color tables
void patcpy(int8 from, int8 to) {
	cvu_vmemtomemcpy(tmpbuf, (from<<3)+gPATTERN, 8);
	cvu_memtovmemcpy((to<<3)+gPATTERN, tmpbuf, 8);
}

//void screen(int x) {
//	cv_set_colors(0,x-1);
//}

//void hchar(int r, int c, int ch, int cnt) {
//	cvu_vmemset(gIMAGE+((r-1)<<5)+c-1, ch, cnt);
//}

//int gchar(int r, int c) {
//	return cvu_vinb(gIMAGE+((r-1)<<5)+c-1);
//}

void spmag(int x) {
	cv_set_sprite_big(((x-1)>>1)&1);
	cv_set_sprite_magnification((x-1)&1);
}

// setup a sprite
void sprite(int n, int chr, int col, int y, int x) {
	cv_set_write_vram_address(gSPRITES+(n<<2));
	cv_voutb(y);
	cv_voutb(x);
	cv_voutb(chr);
	cv_voutb(col-1);
}

// get sprite position
void spposn(int n, int *r, int *c) {
	cv_set_read_vram_address(gSPRITES+(n<<2));
	*r=cv_vinb();
	*c=cv_vinb();
}

// set sprite location
void sploct(int n, int r, int c) {
	cv_set_write_vram_address(gSPRITES+(n<<2));
	cv_voutb(r);
	cv_voutb(c);
}

// set sprite color
void spcolr(int n, int col) {
	cvu_voutb(col-1, gSPRITES+(n<<2)+3);
}

// set sprite pattern
//void sppat(int n, int chr) {
//	cvu_voutb(chr, gSPRITES+(n<<2)+2);
//}

// delete a sprite
//void spdel(int n) {
//	cvu_voutb(0xd1,gSPRITES+(n<<2));
//}

void spdall() {
	unsigned char i;
	for (i=0; i<32; i++) {
		spdel(i);
	}
}

// set main color
//void color(int nSet, int nFore, int nBack) {
//	cvu_voutb(((nFore-1)<<4)|(nBack-1),gCOLOR+nSet);
//}

// returns c^2, clamped near max_int
int  spdist(int s1, int s2) {
	int r1,r2,c1,c2;

	spposn(s1, &r1, &c1);
	spposn(s2, &r2, &c2);

	// keep positive value for easier trimming
	if (r1 > r2) {
		r1=r1-r2;
	} else {
		r1=r2-r1;
	}
	if (c1>c2) {
		c1=c1-c2;
	} else {
		c1=c2-c1;
	}

	// avoid overflow by checking before we square
	// max value for either is 16383, which the 
	// closest whole square to is 127, so clamp to that
	if (r1 > 127) r1=127;
	if (c1 > 127) c1=127;
	return ((r1*r1 + c1*c1));
}

/*
** based on c99 random number functions
** by Clint Pulley, based on TI-FORTH
*/
int seed;
/* generate a 16-bit random number
*/
int rndnum()
{
	int z;

//	seed*=0x6fe5;	// this multiply just scrambles the numbers, really, right? Can we XOR instead (for performance)?
	seed^=0x6fe5;	// (seems to be okay!)
	seed+=0x7ab9;
	z=seed&1;
	seed>>=1;
	if (z) seed|=0x8000;

	return seed;
}

/* generate a random number between
** 0 and n-1
*/
int rnd(int n)
{
	int res = rndnum();
	if (res < 0) res=-res;
	res%=n;

	return res;
}

void intpic() {
	unsigned char i,j;

	// initialize graphics mode so we can load a picture
	// we're probably not in said mode, so go ahead
	// just display some bitmap pictures
	cv_set_screen_active(false);			// Switch screen off.
	cv_set_screen_mode(CV_SCREENMODE_BITMAP);
	cv_set_sprite_big(1);
	cv_set_sprite_magnification(0);

	// disable interrupts (since the Coleco uses an NMI we can disable it at the video chip)
	// this modifies VDP register 1, screen mode functions mess with that
	cv_set_vint_active(false);		// no interrupts!

	// set the mode bits ourselves - does NOT set to the same as the globals!
	cv_set_read_vram_address(0x8206);		// SIT at >1800
	cv_set_read_vram_address(0x83ff);		// CT at >2000
	cv_set_read_vram_address(0x8403);		// PDT at >0000
	cv_set_read_vram_address(0x8536);		// SAL at >1B00
	cv_set_read_vram_address(0x8607);		// SDT at >3800

	// fill the image table with three sets of 0-255
	cv_set_write_vram_address(0x1800);
	for (i=0; i<3; i++) {
		j=0;
		do {
			cv_voutb(j++);
			cv_voutb(j++);
			cv_voutb(j++);
			cv_voutb(j++);
		} while (j != 0);
	}
}

void RLEUnpack(cv_vmemp p, unsigned char *buf) {
	int cnt;
	unsigned char z;

	cv_set_write_vram_address(p);
	cnt=6144;
	while (cnt > 0) {
		z=*buf;
		if (z&0x80) {
			// run of bytes
			buf++;
			cv_vmemset_slow(*buf, z&0x7f);
			buf++;
		} else {
			// sequence of data
			buf++;
			cv_memtovmemcpy_slow(buf, z);
			buf+=z;
		}
		cnt-=(z&0x7f);
	}
}

// this is for bitmap mode - it does NOT use the globals,
// but uses fixed offsets
void ldpic(const char *p, const char *c) {
	// loads a picture - we aren't actually loading from disk
	// we are un-RLE-ing the image from ROM
	RLEUnpack(0x0000, p);
	RLEUnpack(0x2000, c);
	cv_set_screen_active(true);
}

// read controller, return key number for specified unit
// 5 = keyboard
// 1 = joystick 1 (fire=18)
// 2 = joystick 2 (fire=18)
// stat is 0 if nothing, 1 if new key, -1 if key held (-1 not implemented)
int key(int num, int *stat) {
	if ((num == 5)||(num == 0)) {
		// keyboard sim
		cv_get_controller_state(&cs, 0);	// Read the controller (0 only)
		if (cs.keypad == 0) {
			*stat=1;
			return 48;		// 0
		}
		if (cs.keypad == 1) {
			*stat=1;
			return 65;		// a
		}
		if (cs.keypad == 2) {
			*stat=1;
			return 83;		// s
		}
		if (cs.keypad == 3) {
			*stat=1;
			return 76;		// l
		}
		if (cs.keypad == 4) {
			*stat=1;
			return 83;		// s
		}
		if (cs.keypad == 5) {
			*stat=1;
			return 87;		// w
		}
		if (cs.keypad == 6) {
			*stat=1;
			return 80;		// p
		}
		if (cs.keypad == 7) {
			*stat=1;
			return 66;		// b
		}
		if (cs.keypad == 8) {
			*stat=1;
			return 79;		// n
		}
		if (cs.keypad == 9) {
			*stat=1;
			return 32;		// space
		}
		*stat=0;
		return -1;
	} else {
		cv_get_controller_state(&cs, num-1);	// Read the controller.
		if (cs.joystick & CV_FIRE) {
			*stat=1;
			return 18;
		} else {
			*stat=0;
			return -1;
		}
	}
}

int joyst(int num, int *x, int *y) {
	// x and y are always by 4
	// return non zero if moved
	int ret=0;
		
	*x=0;
	*y=0;

	cv_get_controller_state(&cs, num-1);	// Read the controller.
	if (cs.joystick & CV_LEFT) {
		*x=-4;
		ret=1;
	}
	if (cs.joystick & CV_RIGHT) {
		*x=4;
		ret=1;
	}
	if (cs.joystick & CV_UP) {
		*y=4;
		ret=1;
	}
	if (cs.joystick & CV_DOWN) {
		*y=-4;
		ret=1;
	}

	return ret;
}

void grf1() {
	cv_set_screen_active(false);			// Switch screen off.

	// set graphics mode 1, clear screen
	cv_set_screen_mode(CV_SCREENMODE_STANDARD);

	// disable interrupts (since the Coleco uses an NMI we can disable it at the video chip)
	// this modifies VDP register 1, screen mode functions mess with that
	cv_set_vint_active(false);		// no interrupts!

	// set the mode bits ourselves - same as the globals!
	cv_set_read_vram_address(0x8200);		// SIT at >0000
	cv_set_read_vram_address(0x830e);		// CT at >0380
	cv_set_read_vram_address(0x8401);		// PDT at >0800
	cv_set_read_vram_address(0x8506);		// SAL at >0300
	cv_set_read_vram_address(0x8601);		// SDT at >0800
	cv_set_read_vram_address(0x87f4);		// white on blue

	// copy in the font
	loadchars();

	// clear the screen
	clear();

	// clear the sprite attribute table
	spdall();

	cv_set_screen_active(true);
}

void text() {
	cv_set_screen_active(false);			// Switch screen off.

	// set text mode, clear screen
	cv_set_screen_mode(CV_SCREENMODE_TEXT);

	// disable interrupts (since the Coleco uses an NMI we can disable it at the video chip)
	// this modifies VDP register 1, screen mode functions mess with that
	cv_set_vint_active(false);		// no interrupts!

	// set the mode bits ourselves - same as the globals!
	cv_set_read_vram_address(0x8200);		// SIT at >0000
	cv_set_read_vram_address(0x830e);		// CT at >0380
	cv_set_read_vram_address(0x8401);		// PDT at >0800
	cv_set_read_vram_address(0x8506);		// SAL at >0300	(no sprites in text mode, so no conflict)
	cv_set_read_vram_address(0x8601);		// SDT at >0800
	cv_set_read_vram_address(0x87f4);		// white on blue

	// copy in the font
	loadchars();

	// clear the screen
	cvu_vmemset(gIMAGE, 32, 960);			// more characters than grf1 mode

	cv_set_screen_active(true);
}

void clear() {
	if (cv_vdpreg[1] & 0x10) {
		cvu_vmemset(gIMAGE, 32, 960);	// text mode
	} else {
		cvu_vmemset(gIMAGE, 32, 768);	// graphics 1
	}
	scrnpos=gIMAGE;
}

void puts(const char *p) {
	unsigned char step;
	int nLimit;
	unsigned char r=0,c=0;

	if (cv_vdpreg[1] & 0x10) {
		step=40;
		nLimit=gIMAGE+960;
	} else {
		step=32;
		nLimit=gIMAGE+768;
	}

	cv_set_write_vram_address(scrnpos);
	while (*p) {
		if (*p >= ' ') {
			cv_voutb(*(p++));
			scrnpos++;
			if (scrnpos >= nLimit) {
				// wraparound ;)
				scrnpos=gIMAGE;
				cv_set_write_vram_address(scrnpos);
			}
		} else {
			switch(*(p++)) {
				case '\r':
					r=(scrnpos-gIMAGE)/step;
					c=0;
					break;

				case '\n':
					r=(scrnpos-gIMAGE)/step+1;
					c=0;
					break;

				case '\b':
					r=(scrnpos-gIMAGE)/step;
					c=(scrnpos-gIMAGE)%step-1;
					break;
			}
			scrnpos=gIMAGE+(r*step)+c;
			while (scrnpos >= nLimit) {
				// wraparound ;)
				scrnpos-=step*24;
			}
			cv_set_write_vram_address(scrnpos);
		}
	}
}

void loadchars() {
	// copies in a character set - the Coleco does not have
	// one by default. You can save ROM space by reducing how
	// much of it you include! This code should adapt
	cvu_memtovmemcpy(gPATTERN+(32*8), CHARA1, sizeof(CHARA1));
}
