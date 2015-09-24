// QFP/SOIC-style drawing tool for EAGLE
// Revision log:
// Version 1.0    shabaz
// Version 1.1    30 June 2012
// version 1.2    uploaded to github September 2015
//
#include <stdio.h>

//#define PWIDTH 0.29
//#define PHEIGHT 1.35 

//#define AX 21.45
//#define e_val 0.5
//#define pins 144

#define wwidth 0.127

FILE* f;

int
main(void)
{
  int i, j;
  double x, y;
  double p, np;
  double ww, pw;
  int ti;
  double ax2, nax2;
  int sc;
  int iit;
  int sides;
  int pinnum=1;
  double ix0, ix1, iy0, iy1;
  
  double PWIDTH, PHEIGHT, e_val;
  double AX=0.0;
  int pins;
  char fname[1000];
  
  printf("** SMD Tool ***\n");
  printf("All dimensions in mm\n");
  printf("Pad width\n");
  printf("---------\n");
  printf("TQFP:0.29 TSSOP:0.40\n");
  printf(">");
  scanf("%lf", &PWIDTH);
  printf("Pad height\n");
  printf("----------\n");
  printf("TQFP:1.35 TSSOP:1.30\n");
  printf(">");
  scanf("%lf", &PHEIGHT);
  printf("Style (number of sides)\n");
  printf("-----------------------\n");
  printf("TQFP-like:4 TSOP-like:2 SIL-like:1\n");
  printf(">");
  scanf("%d", &sides);
  if (sides>=2)
  {
    printf("Distance to opposite side pad centers (Ax or Ay)\n");
  	printf("------------------------------------------------\n");
  	printf("TQFP48:8.45 TSSOP:6.10\n");
  	printf(">");
  	scanf("%lf", &AX);
  }
  printf("Pin spacing/pitch (e)\n");
  printf("---------------------\n");
  printf("TQFP:0.5 TSSOP:0.65\n");
  printf(">");
  scanf("%lf", &e_val);
  printf("Number of pins\n");
  printf("--------------\n");
  printf("TQFP48:48 TSSOP16:16\n");
  printf(">");
  scanf("%d", &pins);
  if (sides<=2)
  {
  	printf("Overall package width\n");
    printf("---------------------\n");
    printf("TSSOP16:5.0\n");
    printf(">");
    scanf("%lf", &pw);
  }
  
  printf("Filename\n");
  printf("--------\n");
  printf("example: my_tssop.scr\n");
  printf(">");
  scanf("%s", &fname);
  
  f=fopen(fname, "w");
  if (!f)
  {
  	printf("Error opening file\n");
  	return(1);
  }

  fprintf(f, "grid mm\n");
  fprintf(f, "grid 1\n");
  	
  p=pins/sides;
  p=p/2;
  ti=(int)p;
  p=(double) ti;
  p=((p-1)*e_val)+(e_val/2);
  
  iit=pins/sides;
  if (iit % 2 == 0)
  {
  	// even number, nothing else to do
  }
  else
  {
  	p=p+e_val/2;
  }  
    
  np=0-p;
 
  ax2=AX/2;
  nax2=0-ax2;
  
  sc=pins/sides;
  
  // draw outline
  fprintf(f, "set wire_bend 2\n");
  fprintf(f, "layer tPlace \n");
  if (sides==4)
  {
    ix0=nax2+(wwidth*3)+(PHEIGHT/2);
  }
  else
  {
  	//ix0=np-(PWIDTH);
  	ix0=0-(pw/2);
  }
  ix1=0-ix0;
  iy0=nax2+(wwidth*3)+(PHEIGHT/2);
  iy1=0-iy0;
  ww=wwidth;
  fprintf(f, "wire %f (%f %f) (%f %f)\n", ww, ix0, iy0, ix1, iy0);
  fprintf(f, "wire %f (%f %f) (%f %f)\n", ww, ix1, iy0, ix1, iy1);
  fprintf(f, "wire %f (%f %f) (%f %f)\n", ww, ix1, iy1, ix0, iy1);
  fprintf(f, "wire %f (%f %f) (%f %f)\n", ww, ix0, iy1, ix0, iy0);
  if (sides>=2)
  {
  	fprintf(f, "circle 0 (%f %f) (%f %f)\n", ix0+0.5, iy0+0.5, ix0+0.8, iy0+0.5);
  }
  else
  {
  	fprintf(f, "circle 0 (%f %f) (%f %f)\n", ix0+0.5, iy0+(PHEIGHT/2)+0.5, ix0+0.8, iy0+(PHEIGHT/2)+0.5);
  }
  fprintf(f, "set size 1.27\n");
  fprintf(f, "layer tnames\n");
  fprintf(f, "text >NAME (%f %f)\n", ix0, nax2-(PHEIGHT/2)-1-1.27);
  fprintf(f, "layer tvalues\n");
  fprintf(f, "text >VALUE (%f %f)\n", ix0, 0-nax2+(PHEIGHT/2)+1);
  fprintf(f, "layer top\n");
  
  
  // place pads
  x=np; 
  y=nax2;
  for (i=0; i<sc; i++)
  {
    fprintf(f, "smd %f %f '%d' (%f %f)\n", PWIDTH, PHEIGHT, pinnum, x, y);
    x=x+e_val;
    pinnum++;
  }

  if (sides==4)
  {
    x=ax2; 
    y=np;
    for (i=0; i<sc; i++)
    {
      fprintf(f, "smd %f %f '%d' (%f %f)\n", PHEIGHT, PWIDTH, pinnum, x, y);
      y=y+e_val;
      pinnum++;
    }
  }  
  
  if (sides>1)
  {
  	x=p; 
  	y=ax2;
  	for (i=0; i<sc; i++)
  	{
    	fprintf(f, "smd %f %f '%d' (%f %f)\n", PWIDTH, PHEIGHT, pinnum, x, y);
    	x=x-e_val;
    	pinnum++;
  	}
  }

	if (sides==4)
	{
    x=nax2; 
    y=p;
    for (i=0; i<sc; i++)
    {
      fprintf(f, "smd %f %f '%d' (%f %f)\n", PHEIGHT, PWIDTH, pinnum, x, y);
      y=y-e_val;
      pinnum++;
    }  
  }
  
  fclose(f);
  return(0);
  
}
