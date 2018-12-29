// BGA-style drawing tool for EAGLE
// Revision log:
// Version 1.0    shabaz 29 dec 2018
// Not very tidy but seems to work
// my EAGLE version has a bug, it cannot accept Layer tStop command.
// so this BGA tool draws on layer bottom, and it is up to the user to
// deselect all other layers, then select all on layer bottom, and then change to tStop.

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
  double ww, pw, ph;
  int ti;
  double ax2, nax2;
  int sc;
  int iit;
  int skipi;
  int sides;
  int pinnum=1;
  char name[6];
  int aindex=0;
  double ix0, ix1, iy0, iy1;
  
  double PWIDTH, PHEIGHT, STOPWIDTH, CREAMWIDTH, e_val;
  double AX=0.0;
  int hpins, vpins, PROUND;
  char fname[1000];
  
  printf("** BGA Tool ***\n");
  printf("All dimensions in mm\n");
  printf("Top-left pad will be numbered A1, next row begins B1, etc\n");
  printf("Pad copper diameter/width (L)\n");
  printf("---------\n");
  printf("example BGA: 0.4\n");
  printf(">");
  scanf("%lf", &PWIDTH);
  PHEIGHT=PWIDTH;
  printf("Pad paste/cream diameter/width (L)\n");
  printf("---------\n");
  printf("example BGA: 0.4, or 0.0 for no paste holes in stencil\n");
  printf(">");
  scanf("%lf", &CREAMWIDTH);
  printf("Pad stop mask diameter/width (M)\n");
  printf("---------\n");
  printf("example BGA: 0.5\n");
  printf(">");
  scanf("%lf", &STOPWIDTH);
  printf("Pad roundness (100 for round pads)\n");
  printf("----------\n");
  printf("BGA:100 Non-standard:0\n");
  printf(">");
  scanf("%d", &PROUND);
  printf("Pad spacing/pitch (e)\n");
  printf("---------------------\n");
  printf("FTGB:1.0\n");
  printf(">");
  scanf("%lf", &e_val);
  printf("Skip row I\n");
  printf("--------------\n");
  printf("Type 1 to skip row I, otherwise type 0 to retain it\n");
  printf(">");
  scanf("%d", &skipi);
  printf("Number of vertical pins (1-26)\n");
  printf("--------------\n");
  printf("FTGB196:14\n");
  printf(">");
  scanf("%d", &vpins);
  printf("Number of horizontal pins\n");
  printf("--------------\n");
  printf("FTGB196:14\n");
  printf(">");
  scanf("%d", &hpins);
  	printf("Overall package width\n");
    printf("---------------------\n");
    printf("FTGB196:15.0\n");
    printf(">");
    scanf("%lf", &pw);
  printf("Overall package height\n");
    printf("---------------------\n");
    printf("FTGB196:15.0\n");
    printf(">");
    scanf("%lf", &ph);
  
  printf("Filename\n");
  printf("--------\n");
  printf("example: my_bga.scr\n");
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
  	
  
  // draw outline
  fprintf(f, "set wire_bend 2\n");
  fprintf(f, "layer tPlace \n");

  ix1=(pw/2)+(wwidth/2);
  ix0=0-ix1;
  iy0=(ph/2)+(wwidth/2);
  iy1=0-iy0;

  ww=wwidth;
  fprintf(f, "wire %f (%f %f) (%f %f)\n", ww, ix0, iy0, ix1, iy0);
  fprintf(f, "wire %f (%f %f) (%f %f)\n", ww, ix1, iy0, ix1, iy1);
  fprintf(f, "wire %f (%f %f) (%f %f)\n", ww, ix1, iy1, ix0, iy1);
  fprintf(f, "wire %f (%f %f) (%f %f)\n", ww, ix0, iy1, ix0, iy0);
  	fprintf(f, "circle 0 (%f %f) (%f %f)\n", ix0-0.5, iy0+0.5, ix0-0.8, iy0+0.8);
  fprintf(f, "set size 1.27\n");
  fprintf(f, "layer tnames\n");
  fprintf(f, "text >NAME (%f %f)\n", ix0, iy0+2.0);
  fprintf(f, "layer tvalues\n");
  fprintf(f, "text >VALUE (%f %f)\n", ix0, iy1-2.0);
  fprintf(f, "layer top\n");
  
  PROUND=0-PROUND;  
  // place pads
    y=(((double)(vpins-1))*e_val)/2;

  for (i=0; i<vpins; i++)
  {
    name[0]='A'+i;
    if(name[0]>='I')
    {
      if (skipi)
        name[0]++;
    }
    x=(((double)(hpins-1))*e_val)/2;
    x=0-x;
    for (j=0; j<hpins; j++)
    {
      sprintf(name+1, "%d", j+1);
      fprintf(f, "Layer top\n");
      fprintf(f, "smd '%s' %f %f %d R0 NOCREAM NOSTOP (%f %f)\n", name, PWIDTH, PHEIGHT, PROUND, x, y);
      fprintf(f, "Layer bottom\n");
      fprintf(f, "circle 0 (%f %f) (%f %f)\n", x, y, x+(STOPWIDTH/2), y);
      if (CREAMWIDTH>0.0)
      {
        fprintf(f, "Layer tCream\n");
        fprintf(f, "circle 0 (%f %f) (%f %f)\n", x, y, x+(CREAMWIDTH/2), y);
      }


      x=x+e_val;
      pinnum++;
    }
    y=y-e_val;
  }

  fprintf(f, "text Change bottom to tStop! (%f %f)\n", ix0, iy1-6.0);

  
  fclose(f);
  return(0);
  
}
