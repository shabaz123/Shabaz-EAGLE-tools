// toroid drawing tool for EAGLE
// Revision log:
// Version 1.0    shabaz  2011
// Version 1.1    shabaz  June 2012
// Version 1.2    shabaz uploaded to github September 2015
//

#include <stdio.h>
#include <math.h>

#define PI 3.141592654

#define DEG90   1.5707963
#define DEG180  3.141592654
#define DEG270  4.71238898
#define DEG360  6.28318531

// *******************************************************
// *************** CONFIG ********************************

// style 0=vertical mount, 1=horizontal (flat mount)
//#define STYLE    1

// drawing thickness (0.6mm should be always fine)
#define LTHICK  0.6
// toroid dimensions
//#define T_ID  13.3
//#define T_OD  23.6
//#define T_WIDTH 8.38

// wire details
//#define W_THICK   1.626
//#define TURNS     18

// wire minimum bend radius
//#define MINB   1
// spacing of turns from each other (in mm)
//#define GAP    0
// *******************************************************
// *******************************************************

void vect_to_cart(double angle, double length, double *x, double *y);

int
main(void)
{
	FILE* f;
	char fname[1000];
  int i;
  double wire_outerrad, wire_innerrad, in_circum, out_circum;
  double wound_in_circum, wound_deg, sa;
  double ang_inc, ax, ay, bx, by;
  int STYLE;
  double T_ID, T_OD, T_WIDTH, W_THICK, TURNS, MINB, GAP;
  
  printf("** Toroid Tool ***\n");
  printf("All dimensions in mm\n");
  printf("Style\n");
  printf("-----\n");
  printf("vertical(perpendicular):0 horizontal(flat):1\n");
  printf(">");
  scanf("%d", &STYLE);
  printf("Toroid inner diameter\n");
  printf("---------------------\n");
  printf("T37:5.207 T44:5.8166 T50:7.62\n");
  printf(">");
  scanf("%lf", &T_ID);
  printf("Toroid outer diameter\n");
  printf("---------------------\n");
  printf("T37:9.398 T44:11.176 T50:12.7\n");
  printf(">");
  scanf("%lf", &T_OD);
  printf("Toroid width(height)\n");
  printf("--------------------\n");
  printf("T37:3.2512 T44:4.0386 T50:4.826\n");
  printf(">");
  scanf("%lf", &T_WIDTH);
  printf("Wire thickness mm\n");
  printf("-----------------\n");
  printf("SWG16:1.6256 SWG22:0.7112 SWG30:0.315 SWG32:0.2743\n");
  printf("AWG14:1.6281 AWG24:0.5105 AWG26:0.4039 AWG28:0.32\n");
  printf(">");
  scanf("%lf", &W_THICK);  
  printf("Number of turns\n");
  printf("---------------\n");
  printf(">");
  scanf("%lf", &TURNS);
  printf("Wire min bend radius mm\n");
  printf("-----------------------\n");
  printf("e.g. 1\n");
  printf(">");
  scanf("%lf", &MINB);  
  printf("Turns spacing mm\n");
  printf("----------------\n");
  printf("e.g. 0 for close-wound\n");
  printf(">");
  scanf("%lf", &GAP);  
  printf("Filename\n");
  printf("--------\n");
  printf("example: my_toroid.scr\n");
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
  
  // print text
  fprintf(f, "set size 1.27\n");
  fprintf(f, "layer tnames\n");
  if (STYLE==1)
  {
    fprintf(f, "text >NAME (%f %f)\n", 0.0-(T_OD/2), 0.0-(T_OD/2)-2.27);
  }
  else
  {
  	fprintf(f, "text >NAME (%f %f)\n", 0.0-(T_OD/2), 0.0-(T_WIDTH/2)-2.27);
  }
  fprintf(f, "layer tvalues\n");
  if (STYLE==1)
  {
    fprintf(f, "text >VALUE (%f %f)\n", 0.0-(T_OD/2), 0.0-(T_OD/2)-4.04);
  }
  else
  {
  	fprintf(f, "text >VALUE (%f %f)\n", 0.0-(T_OD/2), 0.0-(T_WIDTH/2)-4.04);
  }
  fprintf(f, "layer top\n");
  
  fprintf(f, "layer 21\n");
  fprintf(f, "set wire_bend 2\n");
  fprintf(f, "wire %.2f\n", LTHICK);

  if (STYLE) // horizontal mount
  {
    // draw circles
    fprintf(f, "circle (0 0) (%.2f 0)\n", T_ID/2);
    fprintf(f, "circle (0 0) (%.2f 0)\n", T_OD/2);
  }
  else
  {
    // vertical mount, draw square
    fprintf(f, "wire (-%.2f %.2f) (%.2f %.2f)\n", T_OD/2, T_WIDTH/2, T_OD/2, T_WIDTH/2);
    fprintf(f, "wire (%.2f %.2f) (%.2f -%.2f)\n", T_OD/2, T_WIDTH/2, T_OD/2, T_WIDTH/2);
    fprintf(f, "wire (%.2f -%.2f) (-%.2f -%.2f)\n", T_OD/2, T_WIDTH/2, T_OD/2, T_WIDTH/2);
    fprintf(f, "wire (-%.2f -%.2f) (-%.2f %.2f)\n", T_OD/2, T_WIDTH/2, T_OD/2, T_WIDTH/2);

  }

  fprintf(f, "wire %.2f\n", W_THICK);
  fprintf(f, "change drill %.2f\n", W_THICK+0.2);
  fprintf(f, "pad %.2f ROUND\n", W_THICK+2);

  wire_outerrad=(T_OD/2)+MINB+(W_THICK/2);
  wire_innerrad=(T_ID/2)-MINB-(W_THICK/2);
  in_circum=2*PI*wire_innerrad;
  out_circum=2*PI*wire_outerrad;
  wound_in_circum=TURNS*(W_THICK+GAP);
  wound_deg=(wound_in_circum/in_circum)*PI*2;
  //sa=((PI*2)-wound_deg)/2;
  sa=((PI*2)-(((((TURNS-1)*(W_THICK+GAP))-GAP)/in_circum)*PI*2))/2;
  ang_inc=wound_deg/TURNS;

  for (i=0; i<TURNS; i++)
  {
    vect_to_cart(sa+(ang_inc*i), wire_innerrad, &ax, &ay);
    vect_to_cart(sa+(ang_inc*i), wire_outerrad, &bx, &by);

    if (STYLE) // horizontal
    {
      fprintf(f, "wire (%.2f %.2f) (%.2f %.2f)\n", ax, ay, bx, by);
      if (i==0)
      {
        fprintf(f, "pad (%.2f %.2f)\n", ax, ay);
      }
    }
    else // vertical
    {
      if (i==0)
      {
        fprintf(f, "pad (%.2f %.2f)\n", ax, (T_WIDTH/2)+MINB);
      }

    }


  }
  if (STYLE) // horizontal
  {
    fprintf(f, "pad (%.2f %.2f)\n", bx, by);
  }
  else // vertical
  {
    fprintf(f, "pad (%.2f -%.2f)\n", ax, (T_WIDTH/2)+MINB);
  }

  fclose(f);
  return(0);
}

void
vect_to_cart(double angle, double length, double *x, double *y)
{
  double nx, ny;
  ny=0-cos(angle); nx=sin(angle);
  ny=ny*length; nx=nx*length;

 *y=ny; *x=nx;

}
