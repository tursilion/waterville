#asm
       REF VSBW,VMBW,VSBR,VMBR
       REF VWTR,DSRLNK,KSCAN
#endasm
 
#include "dsk5.grf1rf"
 
int life1, life2, scr1, scr2;
int pos, flag, forevr, x, y, r, c, k, s;
int es[6], p1s[5], p2s[5], p1sp, dist;
int rcount, mcount, cheat;
int a,b,oltype,entype;
int esp[5],br,bc,blife;
int bsr[10],bsc[10],brd;
 
#include "dsk5.random;c"
 
main()
{ init();
  while (forevr!=952)
  {
    title();
    define();
    startup();
    while (flag==0)
      game();
    if (flag==1)
      boss();
    if (flag==2)
      gamovr();
    if (flag==3)
      gamwin();
  }
}
 
init()
{
randomize();
#asm
      COPY "DSK5.WATER/S"
#endasm
}
 
define()
{
grf1();
screen(6);
chrdef(36,"000000000000000703000000000000000000000000070FFFFF07000000000000");
chrdef(40,"00000000000703000000000000000000000000000007CFFF3F07000000000000");
chrdef(44,"000000000000000000030700000000000000000000070F3FFFC7000000000000");
chrdef(60,"000060383cfffffdedeb1800000000000000000000c060fcf8c0000000000000");
chrdef(108,"000000000000010301000000000000000000000000000080000000000000000");
chrdef(92,"000000000000000000000000000000000000e070733f3f3f7370000000000000");
chrdef(96,"0000000000000000000000000000000000003030131f1f1f1310100000000000");
chrdef(100,"0000000000000000000000000000000000000030131f1f1f1310");
chrdef(104,"00303c3efffffeedeb1800000000000000000000e0b0fef8e00000000000000");
chrdef(144,"0000000007070303010100000000000000000000c0c08080000000000000000");
chrdef(148,"00070f1f1f1f1f170a090a121422120000e0f0f8f8f8f8e8505048888448280");
chrdef(152,"00000000007ebfff7900000000000000000000000002cffefc3000000000000");
chrdef(156,"00446622007fbfff78002266440000000044662200fefffe000022664400000");
chrdef(160,"00000000017fbffe780000000000000000000030fcfecf02000000000000000");
chrdef(164,"00000000007fbfff7800000000000000000000020ffefcf0000000000000000");
chrdef(168,"00000000007fbfff78000000000000000000000000f0fcfe0f0200000000000");
chrdef(172,"000222170f1d7f1b1e7f1d0f17220200004044e8f0b8feb8f8fed8f0e844400");
chrdef(176,"000000000000000202010000000000000000000080808080800000000000000");
chrdef(180,"000001010101010101030100000000000000000000000000008000000000000");
chrdef(112,"0103070f1f3f7fff80c0e0f0f8fcfeff00000000030f3fff030f3ffffffffff");
chrdef(116,"c0f0fcffffffffff00000000c0f0fcfffffefcf8f0e0c080ff7f3f1f0f07030");
chrdef(120,"fffffffffffcf0c0fffcf0c000000000ff3f0f0300000000ffffffffff3f0f");
chrdef(124,"ffffffffffffffff");
chrdef(200,"CCCCCCCCCCCCCCCC");
chrdef(204,"071820477cfefffffffffe7c43201907e0588402c2251901019965120a8658e");
chrdef(208,"07182040408080f0fc8f834040201807e01804020201010f3ff1c102020418e");
chrdef(212,"070f08074d7f4f070203070f1f1c1b1bc0e070b8d8d8d8d838d8d8b0b0d8585");
chrdef(216,"03031931393f3f1f0f00000000000000e0e0e0e0c0c0c08000");
chrdef(220,"0000050d1f3f7f7f7fffff8080ffffff000080e0f8fcfefe");
chrdef(223,"fe873379493387ff");
chrdef(224,"ff80809cb2b2f2f6e6e6ecc8c89f0000ff0101394d4d4f6f6767371313f9");
chrdef(228,"00307cff7c30");
chrdef(232,"ffffffffffffffff0103070f1f3f7fff80c0e0f0f8fcfeff");
chrdef(235,"fffefcf8f0e0c080ff7f3f1f0f070301");
chrdef(248,"0000001f376b7777f7f375310e040000000000ff90a7b981b9a790ff");
chrdef(252,"000702051f0703070e0c0703090d0f0600008040404040c0c08080808080");
color(28,2,6);
color(29,13,6);
color(14,13,6);
color(15,13,6);
color(25,12,6);
color(6,16,6);
color(7,16,6);
color(8,16,6);
color(9,16,6);
color(10,16,6);
color(11,16,6);
}
 
title()
{
  while ((k=key(1,&s))==18);
  while ((k=key(1,&s))!=18);
  text();
  puts("WATERVILLE RESCUE - A GAME BY MIKE WARD\n");
  puts("\n(C) 1990 SEAHORSE SOFTWARE\n");
  puts("\n\n'WATERVILLE GANG' CHARACTERS, NAMES,\n");
  puts("AND LIKENESS' ARE (C) 1973 [?] BY\n");
  puts("GLEN WARREN PRODUCTIONS (USED W/O\n");
  puts("PERMISSION)\n\n\n");
/*
  /* Hey, ONE person actually paid for it back in the day! ;) */
*/
  puts("PAID USERS WILL RECIEVE FREE PROGRAMS\n");
  puts("AND INFORMATION ABOUT NEW RELEASES.\n\n\n");
  puts("...PRESS FIRE BUTTON TO START...");
  while ((k=key(1,&s))!=18);
}
 
startup()
{#asm
       LI R0,SDATA
       MOV R0,@POS
#endasm
clear();
spdall();
say(1,4,"DODGER:");
say(1,18,"SHARKY:");
hchar(1,11,200,5);
hchar(1,25,200,5);
spmag(3);
sprite(1,36,5,60,25);
sprite(2,60,5,60,41);
sprite(3,92,15,125,25);
sprite(4,104,15,125,41);
es[1]=156;
es[2]=152;
es[3]=168;
es[4]=156;
es[5]=160;
es[6]=164;
p1s[1]=36;
p1s[2]=40;
p1s[3]=36;
p1s[4]=44;
p2s[1]=92;
p2s[2]=96;
p2s[3]=92;
p2s[4]=100;
p1sp=1;
dist=0;
rcount=0;
mcount=1;
scr1=0;
scr2=0;
life1=5;
life2=5;
oltype=0;
cheat=0;
k=key(0,&s);
if (k==65) cheat=1;
for (x=0; x<32; x++)
 scroll();
flag=0;
k=key(0,&s);
if (k==32) flag=1;
}
 
game()
{
scroll();
enout();
move();
}
 
boss()
{
kilall();
for (x=0; x<32; x++)
{ scroll();
  spposn(2,&r,&c);
  if (c>41) c=c-8;
  sploct(2,r,c); sploct(1,r,c-16);
  spposn(4,&r,&c);
  if (c>41) c=c-8;
  sploct(4,r,c); sploct(3,r,c-16); }
br=6; bc=17; brd=1;
bdraw();
sprite(18,204,2,93,185);
sprite(19,212,8,25,145);
sprite(20,216,8,49,145);
sprite(21,220,11,137,145);
sprite(22,224,11,161,145);
blife=20;
for (x=0; x<11; x++)
  bsr[x]=0;
say(24,3,"BOSS:");
hchar(24,8,200,20);
while (flag==1)
{ bmove();
  move(); }
}
 
gamovr()
{
say(12,12,"GAME OVER");
}
 
gamwin()
{
for (y=0; y<3000; y++)
  for (x=1; x<17; x++)
    color(29,x,6);
clear();
spdall();
say(10,8,"CONGRADULATIONS");
say(12,7,"YOU HAVE RESCUED");
say(13,7,"ANGEL AND ELOISE");
say(14,7,"FROM THE EVIL SEA");
say(15,12,"MONSTER");
wait();
#asm
       COPY "DSK5.LASTPLOAD"
#endasm
grf1();
screen(6);
say(12,7,"WATERVILLE RESCUE");
wait();
say(12,12,"STARRING");
wait();
say(14,9,"DODGER DOLPHIN");
sprite(1,36,5,89,112); sprite(2,60,5,89,128);
wait();
spdall();
say(14,10,"SHARKY SHARK");
sprite(1,92,15,89,112); sprite(2,104,15,89,128);
wait();
spdall();
say(14,11,"ANGEL FISH");
sprite(1,248,11,89,120);
wait();
spdall();
say(14,8,"ELOISE SEAHORSE");
sprite(1,252,8,89,120);
wait();
spdall();
say(17,8,"EVIL SEA MONSTER");
bc=9; br=1;
bdraw();
wait();
say(12,12,"THE END");
nsay(1,11,scr1);
nsay(1,25,scr2);
}
 
wait()
{ int q;
 for (q=0; q<30000; q++);
 clear();
}
 
die1()
{ spdel(1); spdel(2); spdel(5);
  nsay(1,11,scr1);
}
 
die2()
{ spdel(3); spdel(4); spdel(6);
  nsay(1,25,scr2);
}
 
say(r,c,x) int r,c; char x[];
{ int a;
 
a=0;
while (x[a]!=0)
  hchar(r,c+a,x[a++],1);
}
 
dlife1()
{ if (life1!=0)
{ life1--;
  hchar(1,11+life1,32,1);
  if (cheat==1) life1++;
  if (life1==0)
  {  die1();
     if (life2==0)
       flag=2;
  }
}}
 
dlife2()
{ if (life2!=0)
{ life2--;
  hchar(1,25+life2,32,1);
  if (cheat==1) life2++;
  if (life2==0)
  {  die2();
     if (life1==0)
       flag=2;
  }
}}
 
scroll()
{ int x,y;
 #asm
       COPY "DSK5.SCROLL/S"
       MOV @POS,R0
       CI R0,BUFF
       JNE PLMK
       AI R0,-24
       LI R1,1
       MOV R1,@FLAG
PLMK   MOV R0,@POS
#endasm
dist++;
if (dist==931) dist=0;
sppat(1,p1s[p1sp]);
sppat(3,p2s[p1sp++]);
if (p1sp==5)
       p1sp=1;
x=gchar(22,32);
y=gchar(21,32);
if (y==124)
       x=0;
if (x==124)
{ rcount++;
  if (rcount==mcount)
   {   #asm
       LI R0,SDATA
       MOV R0,@POS
       #endasm
       mcount++;
       rcount=0;
   }
}
}
 
move()
{
if (life1>0) {
s=joyst(1,&x,&y);
spposn(2,&r,&c);
a=(r-1)/8+1;
b=(c-1)/8+1;
s=gchar(a,b);
if (s==124)
{      x=x/2; y=y/2;
       c=c-1;
}
r=r-y;
c=c+x;
if (r>170)
       r=170;
if (r<9)
       r=9;
if (c<16)
       c=16;
if (c>200)
       c=200;
sploct(1,r,c-16); sploct(2,r,c);
shoot1();
}
if (life2>0) {
s=joyst(2,&x,&y);
spposn(4,&r,&c);
a=(r-1)/8+1;
b=(c-1)/8+1;
s=gchar(a,b);
if (s==124)
{      x=x/2; y=y/2;
       c=c-1;
}
r=r-y;
c=c+x;
if (r>170)
       r=170;
if (r<9)
       r=9;
if (c<16)
       c=16;
if (c>200)
       c=200;
sploct(3,r,c-16); sploct(4,r,c);
shoot2();
}
}
 
enout()
{
entype=5;
if (dist<745) entype=4;
if (dist<559) entype=3;
if (dist<373) entype=2;
if (dist<187) entype=1;
x=gchar(2,22);
if (x!=32) entype=6;
if (entype!=oltype)
{      kilall();
       oltype=entype;}
if (entype==1) jellyf();
if (entype==2) eel();
if (entype==3) amoeba();
if (entype==4) fishho();
if (entype==5) fishsp();
if (entype==6) spikes();
}
 
kilall()
{
a=7;
while (a<18)
       spdel(a++);
}
 
freesp()
{ int x;
x=6; r=10;
while (r<192)
  spposn(++x,&r,&c);
return(x);
}
 
jellyf()
{
if (rnd(3)==1)
{ x=freesp();
  if (x<18)
    sprite(x,148,16,rnd(192),255);
}
x=7;
while (x<18)
{ spposn(x,&r,&c);
  if (r<192) {
  r=r+(rnd(3)-1);
  c=c-5;
  sploct(x,r,c);
  if (r>190) spdel(x);
  if (r<2) spdel(x);
  if (c<5) spdel(x);
  y=spdist(x,2);
  if (y<70) dlife1();
  y=spdist(x,4);
  if (y<70) dlife2();
  y=spdist(x,5);
  if (y<70) { spdel(x); spdel(5); scr1=scr1+50; }
  y=spdist(x,6);
  if (y<70) { spdel(x); spdel(6); scr2=scr2+50; }
 }x++;
}
}
 
eel()
{if (rnd(5)==1)
{ x=freesp();
  if (x<13)
  {  sprite(x,156,11,rnd(192),255);
     esp[x-7]=1;
  }
}
x=7;
while (x<13)
{spposn(x,&r,&c);
 if (r<192) {
  r=r+(rnd(3)-1)*3;
  c=c-9;
  sppat(x,es[++esp[x-7]]);
  if (esp[x-7]==7) esp[x-7]=1;
  sploct(x,r,c);
  if (r>190) spdel(x);
  if (r<4) spdel(x);
  if (c<9) spdel(x);
  y=spdist(x,2);
  if (y<70) dlife1();
  y=spdist(x,4);
  if (y<70) dlife2();
  y=spdist(x,5);
  if (y<70) { spdel(x); spdel(5); scr1=scr1+75; }
  y=spdist(x,6);
  if (y<70) { spdel(x); spdel(6); scr2=scr2+75; }
}x++;
}}
 
amoeba()
{if (rnd(4)==1)
{ x=freesp();
  if (x<18) sprite(x,172,2,rnd(192),255);
}
x=7;
while (x<18)
{spposn(x,&r,&c);
 if (r<192) {
  r=r+(rnd(3)-1);
  c=c-4;
  sploct(x,r,c);
  if (r>190) spdel(x);
  if (r<3) spdel(x);
  if (c<5) spdel(x);
  y=spdist(x,2);
  if (y<70) dlife1();
  y=spdist(x,4);
  if (y<70) dlife2();
  y=spdist(x,5);
  if (y<70) { spdel(x); spdel(5); scr1=scr1+25; }
  y=spdist(x,6);
  if (y<70) { spdel(x); spdel(6); scr2=scr2+25; }
} x++;
}}
 
fishho()
{if (rnd(3)==1)
{ x=freesp();
   if (x<18)
     sprite(x,176,2,rnd(192),255);
}
x=7;
while (x<18) {
  spposn(x,&r,&c);
  if (r<192) {
    c=c-8;
    sploct(x,r,c);
    if (c<9) spdel(x);
    y=spdist(x,2);
    if (y<70) dlife1();
    y=spdist(x,4);
    if (y<70) dlife2(); }
  x++;
}}
 
fishsp()
{ if (rnd(4)==1)
{ x=freesp();
  if (x<18)
    sprite(x,180,2,1,rnd(240)+10); }
x=7;
while (x<18) {
 spposn(x,&r,&c);
 if (r<192) {
  r=r+6;
  c=c-8;
  sploct(x,r,c);
  if (r>190) spdel(x);
  if (c<9) spdel(x);
  y=spdist(x,2);
  if (y<70) dlife1();
  y=spdist(x,4);
  if (y<70) dlife2(); }
 x++;
}}
 
spikes()
{ if (rnd(2)==1)
{ x=freesp();
  if (x<18)
  { c=rnd(256);
    y=gchar(2,c/8+1);
    if (y!=32)
      sprite(x,144,13,10,c);
  }
} x=7;
while (x<18)
{ spposn(x,&r,&c);
  if (r<192) {
    r=r+8;
    c=c-8;
    sploct(x,r,c);
    if (r>190) spdel(x);
    if (c<9) spdel(x);
    y=spdist(x,2);
    if (y<70) dlife1();
    y=spdist(x,4);
    if (y<70) dlife2(); }
  x++;
}}
 
shoot1()
{spposn(5,&r,&c);
if (r>191) {
  k=key(1,&s);
  if (s!=0)
  { spposn(2,&r,&c);
    sprite(5,108,5,r,c);
  }
}
spposn(5,&r,&c);
if (r<192) {
  c=c+7;
  sploct(5,r,c);
  if (c>250) spdel(5);
}}
 
shoot2()
{spposn(6,&r,&c);
if (r>191) {
  k=key(2,&s);
  if (s!=0)
  { spposn(4,&r,&c);
    sprite(6,108,15,r,c);
  }
}
spposn(6,&r,&c);
if (r<192) {
  c=c+7;
  sploct(6,r,c);
  if (c>250) spdel(6);
}}
 
nsay(r,c,x) int r,c,x;
{ int y;
y=x/10000;
x=x-(y*10000);
hchar(r,c,y+48,1);
y=x/1000;
x=x-(y*1000);
hchar(r,c+1,y+48,1);
y=x/100;
x=x-(y*100);
hchar(r,c+2,y+48,1);
y=x/10;
x=x-(y*10);
hchar(r,c+3,y+48,1);
hchar(r,c+4,x+48,1);
}
 
bdraw()
{ #asm
       COPY "DSK5.DRAW/S"
#endasm
}
 
berase()
{ #asm
       COPY "DSK5.ERASE/S"
#endasm
}
 
bmove()
{
berase();
br=br+brd;
bdraw();
for (x=18; x<23; x++)
{ spposn(x,&r,&c);
  if (brd==1)
   sploct(x,r+8,c);
  else
   sploct(x,r-8,c);
}
if (br==3) {brd=1; sppat(18,204); }
if (br==9) {brd=-1; sppat(18,204); }
bshot();
}
 
bshot() {
sppat(1,p1s[p1sp]);
sppat(3,p2s[p1sp++]);
if (p1sp==5)
       p1sp=1;
y=spdist(5,18);
if (y<70) { eyehit(); scr1=scr1+100; spdel(5);}
y=spdist(5,19);
if (y<70) dlife2();
y=spdist(5,20);
if (y<70) dlife2();
y=spdist(5,21);
if (y<70) dlife1();
y=spdist(5,22);
if (y<70) dlife1();
y=spdist(6,18);
if (y<70) { eyehit(); scr2=scr2+100; spdel(6);}
y=spdist(6,19);
if (y<70) dlife2();
y=spdist(6,20);
if (y<70) dlife2();
y=spdist(6,21);
if (y<70) dlife1();
y=spdist(6,22);
if (y<70) dlife1();
for (x=0; x<10; x++)
{if (bsr[x]==0)
{ bsr[x]=br+rnd(15);
  bsc[x]=bc;
  hchar(bsr[x],bsc[x],228,1);
  x=10;
}}
for (x=0; x<10; x++)
{if (bsr[x]!=0)
{ hchar(bsr[x],bsc[x],32,1);
  bsc[x]--;
  if (bsc[x]==0) bsr[x]=0;
  if (bsr[x]!=0) hchar(bsr[x],bsc[x],228,1);
}}
spposn(2,&r,&c);
if (r<192)
{ x=gchar(r/8+1,c/8+1);
  if (x!=32) dlife1();
}
spposn(4,&r,&c);
if (r<192)
{ x=gchar(r/8+1,c/8+1);
  if (x!=32) dlife2();
}
}
 
eyehit()
{ sppat(18,208);
  blife--;
  hchar(24,8+blife,32,1);
  if (blife==0) flag=3;
}
 
