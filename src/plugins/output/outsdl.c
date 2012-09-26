
/* Simple program:  Test bitmap blits */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <picviz.h>

#include "SDL.h"
#include "sge.h"

/* Call this instead of exit(), so we can clean up SDL: atexit() is evil. */
static void quit(int rc)
{
	SDL_Quit();
	exit(rc);
}

/* I used some code from Trophy to do this once http://trophy.sourceforge.net/index.php3?body=developers_corner */

//void RGBtoHSV(const int* rgb, int* hsv)
//{
//	int max = rgb[0];        // maximum RGB component
//	int whatmax = 0;    // rgb[0]=>0, rgb[1]=>1, rgb[2]=>2
//	if( rgb[1] > max )
//	{
//		max = rgb[1];
//		whatmax = 1;
//	}
//	if( rgb[2] > max )
//	{
//		max = rgb[2];
//		whatmax = 2;
//	}
//
//	int min = rgb[0];        // find minimum value
//	if( rgb[1] < min ) min = rgb[1];
//	if( rgb[2] < min ) min = rgb[2];
//
//	int delta = max-min;
//	hsv[2] = max;
//	if (max == 0)
//		hsv[1] = 0;
//	else
//		hsv[1] = (510*delta+max)/(2*max);
//
//	if( hsv[1] == 0 )
//	{
//		hsv[0] = -1;        // undefined hue
//	}
//	else
//	{
//		switch( whatmax )
//		{
//			case 0:        // red is max component
//				if( rgb[1] >= rgb[2] )
//					hsv[0] = (120*(rgb[1]-rgb[2])+delta)/(2*delta);
//				else
//					hsv[0] = (120*(rgb[1]-rgb[2]+delta)+delta)/(2*delta) + 300;
//				break;
//			case 1:        // green is max component
//				if( rgb[2] > rgb[0] )
//					hsv[0] = 120 + (120*(rgb[2]-rgb[0])+delta)/(2*delta);
//				else
//					hsv[0] = 60 + (120*(rgb[2]-rgb[0]+delta)+delta)/(2*delta);
//				break;
//			case 2:        // blue is max component
//				if ( rgb[0] > rgb[1] )
//					hsv[0] = 240 + (120*(rgb[0]-rgb[1])+delta)/(2*delta);
//				else
//					hsv[0] = 180 + (120*(rgb[0]-rgb[1]+delta)+delta)/(2*delta);
//				break;
//		}
//	}
//}
//
//void HSVtoRGB(const int* hsv, int* rgb)
//{
//	rgb[0]=rgb[1]=rgb[2] = hsv[2];
//
//	if( hsv[1] == 0 || hsv[0] == -1 )
//	{
//		;
//	}
//	else
//	{
//		if( hsv[0] >= 360 )
//		{
//			hsv[0] %= 360;
//		}
//		if( hsv[1] >= 255 )
//		{
//			hsv[1] %= 255;
//		}
//		if( hsv[2] >= 255 )
//		{
//			hsv[2] %= 255;
//		}
//		int f = hsv[0]%60;
//		hsv[0] /= 60;
//		int p = (2*hsv[2]*(255-hsv[1])+255)/510;
//		if( (hsv[0] & 1) != 0 )
//		{
//			int q = (2*hsv[2]*(15300-hsv[1]*f)+15300)/30600;
//			switch( hsv[0] )
//			{
//				case 1:
//					rgb[0]=(int)q;
//					rgb[1]=(int)hsv[2];
//					rgb[2]=(int)p;
//					break;
//				case 3:
//					rgb[0]=(int)p;
//					rgb[1]=(int)q;
//					rgb[2]=(int)hsv[2];
//					break;
//				case 5:
//					rgb[0]=(int)hsv[2];
//					rgb[1]=(int)p;
//					rgb[2]=(int)q;
//					break;
//			}
//		}
//		else
//		{
//			int t = (2*hsv[2]*(15300-(hsv[1]*(60-f)))+15300)/30600;
//			switch( hsv[0] )
//			{
//				case 0:
//					rgb[0]=(int)hsv[2];
//					rgb[1]=(int)t;
//					rgb[2]=(int)p;
//					break;
//				case 2:
//					rgb[0]=(int)p;
//					rgb[1]=(int)hsv[2];
//					rgb[2]=(int)t;
//					break;
//				case 4:
//					rgb[0]=(int)t;
//					rgb[1]=(int)p;
//					rgb[2]=(int)hsv[2];
//					break;
//			}
//		}
//	}
//}
//







void output(pcimage_t *image, char *args)
{
	SDL_Surface *screen;
	SDL_Surface *bitmap;
	Uint8  video_bpp;
	Uint32 videoflags;
	Uint8 *buffer;
	int i, j, k, done;
	SDL_Event event;
	Uint16 *buffer16;
        Uint16 color;
        Uint8  gradient;

	struct axis_t *a;
        struct line_t *l;
        struct axisplot_t *axisplot;
	unsigned int counter;
	int x;
	int y;

	int data_array[10];


	/* Initialize SDL */
	if ( SDL_Init(SDL_INIT_VIDEO) < 0 ) {
		fprintf(stderr, "Couldn't initialize SDL: %s\n",SDL_GetError());
		return;
		//return(1);
	}

	video_bpp = 0;
	videoflags = SDL_SWSURFACE;

	/* Set 770x770 video mode */
	if ( (screen=SDL_SetVideoMode(770,770,video_bpp,videoflags)) == NULL ) {
		fprintf(stderr, "Couldn't set 770x770x%d video mode: %s\n",
						video_bpp, SDL_GetError());
		quit(2);
	}


	for (i=0;i<770;i+=70) {
		sge_VLine(screen,i,0,770, SDL_MapRGB(screen->format,255,0,0));
		//sge_Line(screen,i,,0,770, SDL_MapRGB(screen->format,255,0,0));
	}

	for (i=0;i<770;i+=70) {
		sge_HLine(screen,0,770,i, SDL_MapRGB(screen->format,255,0,0));
	}

	llist_for_each_entry(l, &image->lines->list, list) {
		counter = 0;
		llist_for_each_entry(axisplot, &l->axisplot->list, list) {
/* 			counter++; */
/* 			//x = counter*(70); */
/* 			x = 30; */
/* 			//y = axisplot->y * (counter*(70)); */
/* 			//y = counter * ((axisplot->y*11)/770); */
/* 			y = axisplot->y; */
/* 			sge_PutPixel(screen, x, y, 0xffffff); */
			//printf("Put pixel at: %d, %d\n", counter*(70), axisplot->y * (counter*(70)));
		        data_array[counter] = axisplot->y;
			counter++;
		}
		for (i=0;i<11;i++) {
		        for (j=0;j<11;j++) {
			  sge_PutPixel(screen, i*70+data_array[i]/7, j*70+data_array[j]/7, 0xffffff);
			}
		}
	}

	do{
		/* Wait for user input */
		SDL_WaitEvent(&event);
		if(event.type==SDL_KEYDOWN && event.key.keysym.sym==SDLK_ESCAPE){break;}
		if(event.type==SDL_QUIT){break;}
	}while(1);



	SDL_Quit();
	return;
	//return(0);
}
