/********************************************************************************
*  HPGL to EAGLE SCR converter
*
*  Ver   Author                Description
*  ----  ----------------      ---------------------------------
*  1.0   Shabaz                Initial version
*  1.1   Shabaz                October 2015 - Uploaded to GitHub
*  1.2   greigs                Added scale parameter. Build fix for g++ on windows
*********************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h>

using std::string;

// HPGL scaling of 1016 points to an inch means that the
// millimetre scale is 1016/25.4 = 40 points per millimetre
#define DEFAULTSCALE 40


// global vars
int pval;

// func prototypes
void process_cmd(char* hbuf, string& sline, char *xchar, char* ychar);

// main prog
int
main(int argc, char *argv[])
{
  
  FILE *infile;
  FILE *outfile;
  string in_name;
  string out_name;
  int not_finished=1;
  string sline;
  char hbuf[32];
  char xchar[32];
  char ychar[32];
  int idx;
  int state;
  int ret;
  unsigned int fromint[2];
  unsigned int toint[2];
  unsigned int oldint[2];
  int processed;
  double scale=DEFAULTSCALE;
  double fromf[2];
  double tof[2];
  
  if (argc>1)
  {
  	in_name=argv[1];
  }
  else
  {
  	printf("Error - no input file specified\n");
  	printf("Usage: hpgl2eagle.exe inputfile outputfile.scr scale\n");
  	printf("       where scale is points per millimetre (e.g. 20.1) - Default is 40.0");
  	return(1);
  }
  
  if (argc>2)
  {
  	out_name=argv[2];
  }
  else
  {
  	out_name=string("output.scr");
  }
  if (argc>3)
  {
  	scale=atof(argv[3]);
  }

  infile=fopen(in_name.c_str(), "rb");
  if (infile==NULL)
  {
  	printf("Error opening the input file\n");
  	return(1);
  }
  
  outfile=fopen(out_name.c_str(), "w");
  if (outfile==NULL)
  {
  	printf("Error opening the output file\n");
  	return(1);
  }
  
  printf("Scale set to %f points per millimetre\n", scale);

  // initial file content:
  fprintf(outfile, "# File generated using hpgl2eagle\n");
  fprintf(outfile, "grid mm\n");
  fprintf(outfile, "grid 1\n");
  fprintf(outfile, "set wire_bend 2\n");
  fprintf(outfile, "change width 0.2\n");
  
  
  pval=0;
  idx=0;
  state=0;


  do
  {
  	do
  	{
  	  ret=fread((hbuf+idx), 1, 1, infile);
  	  if (ret==0)
  	  {
  		  not_finished=0;
  		  break;
  	  }
  	  else
  	  {
  	  	if (hbuf[idx]==';')
  	  	{
  	  		break;	
  	  	}
  	  }
  	  idx++;
    } // this loops until a semi-colon is found
    while(1);
    hbuf[idx+1]='\0';
  	//printf("command collected: '%s'\n", hbuf);
  	
  	idx=0;
  	
  	if (strlen(hbuf)<4) continue;
  	
  	sline=string(hbuf);
  	
  	processed=0;
  	if ((hbuf[0]=='P') && (hbuf[1]=='U') && (sline.find(",") != string::npos))
  	{
  		state=1;
  		processed=1;
  		process_cmd(hbuf, sline, xchar, ychar);
  	}
  	if ((hbuf[0]=='P') && (hbuf[1]=='D') && (sline.find(",") != string::npos))
  	{
  		if (state==2)
  		{
  			// remain in same state
  		}
  		else
  		{
  			state++;
  		}
  		processed=1;
  		process_cmd(hbuf, sline, xchar, ychar);
  	}
  	
  	if (processed)
  	{
  		if (state==1)
  		{
  			sscanf(xchar, "%u", &fromint[0]);
  			sscanf(ychar, "%u", &fromint[1]);
  			continue;
  		}
  		if (state==2)
  		{
  			sscanf(xchar, "%u", &toint[0]);
  			sscanf(ychar, "%u", &toint[1]);
  			oldint[0]=toint[0];
  			oldint[1]=toint[1];
  			
  			// scale the co-ords:
  			fromf[0]=((double)fromint[0])/scale;
  			fromf[1]=((double)fromint[1])/scale;
  			tof[0]=((double)toint[0])/scale;
  			tof[1]=((double)toint[1])/scale;
  			
  			fprintf(outfile, "wire (%.3f %.3f) (%.3f %.3f)\n", 
  			                           fromf[0], fromf[1],
  			                           tof[0], tof[1]);
  			fromint[0]=oldint[0];
  			fromint[1]=oldint[1];
  			pval++;
  		}
  	}
  		
  	
  }while(not_finished);
  
  printf("%d segments processed, output file='%s'\n", pval, out_name.c_str());
  fclose(infile);
  fclose(outfile);
  
  return(0);
}



void
process_cmd(char* hbuf, string& sline, char *xchar, char* ychar)
{
	int com, sem;
	
	com=sline.find(",");
	sem=sline.find(";");
	strncpy(xchar, hbuf+2, com-2);
	xchar[com-2]='\0';
	//printf("xchar='%s'\n", xchar);
	strncpy(ychar, hbuf+com+1, sem-com-1);
	ychar[sem-com-1]='\0';
	//printf("ychar='%s'\n'", ychar);

	
	return;
}


