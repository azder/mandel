/*
	Mnozestvoto na Mandelbrot
	Goran Peoski 8583
*/
#include <iostream.h>
#include <values.h>
#include <math.h>
#include <conio.h>
#include <graphics.h>
#include <stdio.h>
#include <stdlib.h>
//Makroa
#define KEY_ESC 27
#define KEY_BS 8
#define KEY_LEFT 75
#define KEY_RIGHT 77
#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_INS 82
#define KEY_HOME 71
#define KEY_PGUP 73
#define KEY_DEL 83
#define KEY_END 79
#define KEY_PGDN 81
#define KEY_F1 59
#define KEY_F2 KEY_F1+1
#define KEY_F3 KEY_F1+2
#define KEY_F4 KEY_F1+3
#define KEY_F5 KEY_F1+4
#define KEY_F6 KEY_F1+5
#define KEY_F7 KEY_F1+6
#define KEY_F8 KEY_F1+7
#define KEY_F9 KEY_F1+8
#define KEY_F10 KEY_F1+9
#define KEY_F11 133
#define KEY_F12 134

#define CLIP_ON 1
#define STOP KEY_F3
#define MAXZOOM 100
#define START_MAXMOD 4
//Makroa kraj

//Deklaracii
struct zoom_t{long double x1,y1,x2,y2;};
void text();
int boja(long double x,long double y);
inline int errorgraph();
int regbgi();
void mandel2();
void ciklus();
void izbrisi();
void mandelmalosvezi();
void pomos();
void info();
extern "C" void _Cdecl bold_font(void);
//Deklaracii kraj

//Globalni
zoom_t zoomlist[MAXZOOM]={-2,2,-2,2};
unsigned long maxboja=1024;
unsigned long maxmod=START_MAXMOD/**32-2*/;
int zoombroj=0;
long double x1=-2,x2=2,y1=-2,y2=2;
//Globalni kraj

int boja(long double x,long double y){
	long double a=0,b=0,t;
	unsigned long c=0;
	do{
		c++;t=a;
		a=t*t-b*b+x;
		b=2.0*t*b+y;
	}while((c<maxboja) && ((a*a+b*b)<(maxmod*maxmod)));
	return abs((int)c);
}
int bojamal(long double x,long double y){
	long double a=0,b=0,t;
	int c=0;
	do{
		c++;t=a;
		a=t*t-b*b+x;
		b=2.0*t*b+y;
	}while((c<512) && ((a*a+b*b)<32766));
	return c;
}
inline int errorgraph(){
	int errorcode=graphresult();
	if (grOk==errorcode) return 0;
	printf("Graficka greska: %s\n", grapherrormsg(errorcode));;
	printf("Pritisni kopce za kraj:\n");getch();return 1;
}

int fontn;
inline int regbgi(){
	fontn=registerbgifont(bold_font);
	int errorcode = registerbgidriver(EGAVGA_driver);
	if(errorcode>=0) return 0;
	printf("Graficka greska: %s\n", grapherrormsg(errorcode));
	printf("Pritisni kopce za kraj:");getch();return 1;
}
inline void graphend(){closegraph();}
inline void pozadina(){
	setfillstyle(SOLID_FILL, BLUE);
	floodfill(3,3,WHITE);
	floodfill(637,477,WHITE);
	floodfill(637,3,WHITE);
	floodfill(3,477,WHITE);
	floodfill(3,240,WHITE);
	floodfill(637,240,WHITE);
	floodfill(405,402,WHITE);
	floodfill(637,402,WHITE);
}
inline void graphstart(){
	int gdriver = DETECT, gmode, errorcode;
	if(regbgi()) exit(0);
	initgraph(&gdriver, &gmode, "");
	if(errorgraph()) exit(0);
	atexit(graphend);
	setcolor(WHITE);
	rectangle(5,5,407,407);//Golem
	rectangle(410,5,635,204);//So podatoci
	rectangle(420,207,620,407);//Mal
	rectangle(5,410,getmaxx()-5,getmaxy()-5);//Dolen
	rectangle(1,1,getmaxx()-1,getmaxy()-1);//Rab
	pozadina();
	setfillstyle(SOLID_FILL, RED);
	floodfill(450,450,WHITE);
	info();
	setfillstyle(SOLID_FILL, BLACK);
	floodfill(50,50,WHITE);
	floodfill(450,100,WHITE);
	mandelmalosvezi();
	text();
}
inline void text(){
	int prec=cout.precision(14);
	gotoxy(53,2);cout<<"Goren lev agol:            ";
	gotoxy(53,3);cout<<"x1 =                       ";
	gotoxy(58,3);cout<<x1;
	gotoxy(53,4);cout<<"y1 =                       ";
	gotoxy(58,4);cout<<-y1;
	gotoxy(53,6);cout<<"Dolen desen agol:          ";
	gotoxy(53,7);cout<<"x2 =                       ";
	gotoxy(58,7);cout<<x2;
	gotoxy(53,8);cout<<"y2 =                       ";
	gotoxy(58,8);cout<<-y2;
	cout.precision(prec);
	gotoxy(53,10);cout<<"Max. modul =              ";
	gotoxy(66,10);cout<<maxmod;
	gotoxy(53,11);cout<<"Max. boja =               ";
	gotoxy(65,11);cout<<maxboja;
	gotoxy(53,12);cout<<"Zoom =                    ";
	gotoxy(60,12);cout<<zoombroj;
}

inline long double fw(){return fabs(x2-x1);};
inline long double fh(){return fabs(y2-y1);};

void main(){
	graphstart();
	pomos();
	ciklus();
}

inline int pogolemo(int a,int b){return a>b?a:b;}
inline int pomalo(int a,int b){return a<b?a:b;}
inline void limit(int &v,int l,int h){
	if(v<l) v=l;else if(v>h) v=h;
}
inline void napravirc(int &mx1,int &my1,int &mx2,int &my2){
	mx1=pogolemo(520+50*x1,421);
	my1=pogolemo(307+50*y1,208);
	mx2=pomalo(520+50*x2,619);
	my2=pomalo(307+50*y2,406);
	limit(mx1,421,619);
	limit(mx2,421,619);
	limit(my1,208,406);
	limit(my2,208,406);
}
void mandelmalosvezi(){
/*420,207,620,407 Koordinati Mal*/
	for(int x=421;x<620;x++)
		for(int y=208;y<407;y++)
			putpixel(x,y,bojamal((x-420)*.02-2,-((y-207)*.02-2)));
}
void mandelmal(){
	int x,y;
	static int mx1=1,mx2=1,my1=1,my2=1;
	for(x=mx1;x<=mx2;x++){
		putpixel(x,my1,bojamal((x-420)*.02-2,(my1-207)*.02-2));
		putpixel(x,my2,bojamal((x-420)*.02-2,(my2-207)*.02-2));
	}
	for(y=my1;y<=my2;y++){
		putpixel(mx1,y,bojamal((mx1-420)*.02-2,-((y-207)*.02-2)));
		putpixel(mx2,y,bojamal((mx2-420)*.02-2,-((y-207)*.02-2)));
	}
	napravirc(mx1,my1,mx2,my2);
	rectangle(mx1,my1,mx2,my2);
}

void mandel2(){
	int x,y;
	long double w=fw(),h=fh();
	for(x=6;x<407;x++)
		for(y=6;y<407;y++){
			putpixel(x,y,boja(
											((x-6)*w/400-fabs(x1)),
											-((y-6)*h/400-fabs(y1))
							));
			if(kbhit() && STOP==getch()) return;
		}
}

inline void izbrisi(){
	setviewport(6, 6, 406, 406, CLIP_ON);
	clearviewport();
	setviewport(0, 0, getmaxx(), getmaxy(), CLIP_ON);
}
inline void levo(){long double w=fw();x1-=w/2;x2-=w/2;text();}
inline void desno(){long double w=fw();x1+=w/2;x2+=w/2;text();}
inline void gore(){long double h=fh();y1-=h/2;y2-=h/2;text();}
inline void dolu(){long double h=fh();y1+=h/2;y2+=h/2;text();}

inline void zoomin(){
	if(zoombroj<MAXZOOM){
		long double w=fw(),h=fh();
		zoomlist[zoombroj].x1=x1;
		zoomlist[zoombroj].x2=x2;
		zoomlist[zoombroj].y1=y1;
		zoomlist[zoombroj].y2=y2;
		x1+=w/4;y1+=h/4;
		x2-=w/4;y2-=h/4;
		zoombroj++;
		text();
	}
//	mendel2();
}
inline void zoomout(){
	if(zoombroj>0){
		zoombroj--;
		x1=zoomlist[zoombroj].x1;
		x2=zoomlist[zoombroj].x2;
		y1=zoomlist[zoombroj].y1;
		y2=zoomlist[zoombroj].y2;
		text();
	}
}
inline void bojaplus(){if(maxboja<MAXLONG>>1) maxboja*=2;text();}
inline void bojaminus(){if(maxboja>1) maxboja/=2;text();}
inline void modulplus(){if(maxmod*maxmod<MAXLONG>>1) maxmod*=2;text();}
inline void modulminus(){if(maxmod>1) maxmod/=2;text();}
inline void bojaplus1(){if(maxboja<MAXLONG)maxboja++;text();}
inline void bojaminus1(){if(maxboja>1)maxboja--;text();}
inline void modulplus1(){if(maxmod<MAXLONG)maxmod++;text();}
inline void modulminus1(){if(maxmod>1)maxmod--;text();}
inline void reset(){
	zoombroj=0;x1=y1=-2;x2=y2=2;maxboja=1024;
	maxmod=START_MAXMOD;
	text();
	pomos();
	mandelmalosvezi();
}

void ciklus(){
	char c,mal=1;
	for(;;){
		if(mal)mandelmal();
		mal=0;
		while(!kbhit());
		switch(getch()){/*c!=0*/
			case KEY_BS:izbrisi();mal=0;break;
			case KEY_ESC:exit(0);break;
			case '\0':
				switch(getch()){/*c==0*/
					case KEY_F12:bojaplus();mal=0;break;
					case KEY_F11:bojaminus();mal=0;break;
					case KEY_F10:bojaplus1();mal=0;break;
					case KEY_F9:bojaminus1();mal=0;break;
					case KEY_F8:modulplus();mal=0;break;
					case KEY_F7:modulminus();mal=0;break;
					case KEY_F6:modulplus1();mal=0;break;
					case KEY_F5:modulminus1();mal=0;break;
					case KEY_F4:reset();mal=1;break;
					case KEY_F2:mandel2();mal=0;break;
					case KEY_F1:pomos();mal=0;break;
					case KEY_INS:zoomin();mal=1;break;
					case KEY_DEL:zoomout();mal=1;break;
					case KEY_LEFT:levo();mal=1;break;
					case KEY_RIGHT:desno();mal=1;break;
					case KEY_UP:gore();mal=1;break;
					case KEY_DOWN:dolu();mal=1;break;
					case KEY_HOME:gore();levo();mal=1;break;
					case KEY_PGUP:gore();desno();mal=1;break;
					case KEY_END:dolu();levo();mal=1;break;
					case KEY_PGDN:dolu();desno();mal=1;break;
				}/*switch(getch()) - c==0*/
			}/*switch(getch()) - c!=0*/
	}/*for(;;)*/
}

void pomos(){
	izbrisi();
	gotoxy(3,2);
	cout<<"F1  = Pomos     F2  = Iscrtaj   F3  = Stop      ";
	gotoxy(3,3);
	cout<<"F4  = Pocetok   F5  = Modul-1   F6  = Modul+1   ";
	gotoxy(3,4);
	cout<<"F7  = Modul/2   F8  = Modul*2   F9  = Boja-1    ";
	gotoxy(3,5);
	cout<<"F10 = Boja+1    F11 = Boja/2    F12 = Boja*2    ";
	gotoxy(3,6);
	cout<<"LEV = Levo      DES = Desno     GOR = Gore      ";
	gotoxy(3,7);
	cout<<"DOL = Dolu      INS = Zoom+     DEL = Zoom-     ";
	gotoxy(3,8);
	cout<<"HOM = GOR+LEV   END = DOL+LEV   PGU = GOR+DES   ";
	gotoxy(3,9);
	cout<<"PGD = DOL+DES   BS  = Izbrisi   ESC = Izlez     ";
}
inline void info(){
	if(fontn>=0)settextstyle(fontn, HORIZ_DIR,0);
	outtextxy(25,400,"Mnozestvoto Mandelbrot");
	settextstyle(DEFAULT_FONT,HORIZ_DIR,0);
	outtextxy(385,465,"Napraveno od Goran Peoski 8583");
}
