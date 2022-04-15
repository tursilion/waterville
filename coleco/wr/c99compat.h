#define gIMAGE 0x0000
#define gPATTERN 0x0800
#define gCOLOR 0x0380
#define gSPRITES 0x0300
#define gSPRITE_PATTERNS 0x0800
#define VOL_OFF 30

typedef unsigned char uint8;
typedef char int8;

extern struct cv_controller_state cs;
extern cv_vmemp pMus;
extern int seed;
extern volatile unsigned char nFrame;
extern volatile int8 step;

// any fire button
#define CV_FIRE (CV_FIRE_0 | CV_FIRE_1 | CV_FIRE_2 | CV_FIRE_3)

#define screen(x) cv_set_colors(0,x-1)
#define hchar(r, c, ch, cnt) cvu_vmemset(gIMAGE+((r-1)<<5)+c-1, ch, cnt)
#define gchar(r, c) cvu_vinb(gIMAGE+((r-1)<<5)+c-1)
#define sppat(n, chr) cvu_voutb(chr, gSPRITES+(n<<2)+2)
#define spdel(n) cvu_voutb(0xd1,gSPRITES+(n<<2))
#define color(nSet, nFore, nBack) cvu_voutb(((nFore-1)<<4)|(nBack-1),gCOLOR+nSet)

void nmi();
void cv_set_vint_active(bool active);
void waitforstep();
void doMusic();
void StartMusic(int p);
void loadcharset();
void chrdef(int n, const unsigned char *sz);
void patcpy(int8 from, int8 to);
void spmag(int x);
void sprite(int n, int chr, int col, int y, int x);
void spposn(int n, int *r, int *c);
void sploct(int n, int r, int c);
void spcolr(int n, int col);
void spdall();
int spdist(int s1, int s2);

void clear();
void puts(const char *p);

int rndnum();
int rnd(int n);
void intpic();
void RLEUnpack(cv_vmemp p, unsigned char *buf);
void ldpic(const char *p, const char *c);
int key(int num, int *stat);
int joyst(int num, int *x, int *y);
void grf1();
void text();
void loadchars();


