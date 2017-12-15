#include<stdio.h>
#include"fusion.h"
void usage(char *command) {
  printf("Usage: %s <IN_PARAMETER_FILE>\n", command);
  printf("<IN_PARAMETER_FILE> is a text input file which contains:\n");
  printf("STARFM_PARAMETER_START\n");
  printf("\tNUM_IN_PAIRS = \n");
  printf("\tThe_pf_band_of_Landsat_for_calculating = \n");
  printf("\tThe_pc_band_of_MODIS_for_calculating = \n");
  printf("\tIN_PAIR_MODIS_FNAME = \n");
  printf("\tIN_PAIR_LANDSAT_FNAME = \n");
  printf("\tIN_PDAY_MODIS_FNAME = \n");
  printf("\tOUT_PDAY_LANDSAT_FNAME = \n");
  printf("\tThe_width_of_searching_window=\n");
  printf("\tAssumed_number_of_classifications = \n");
 // printf("\t =\n");
  printf("\tLandsat_sensor_error= \n");
  printf("\tMODIS_sensor_error = \n"); 
  printf("STARFM_PARAMETER_END\n");
}
int parseParameters(char *fname, CuLayer *psensor,PARAMETER *par)
{
	  int   i, k, total_pairs=0;
  char  buffer[1000] = "\0";
  char  *label =NULL;
  char  *tokenptr =NULL;
  char readpath[1000];
  string argName;
  string t;
  char  *separator = "= ,";
  FILE  *in;  
	//char  buffer[1000] = "\0";
	if((in=fopen(fname,"r"))==NULL) 
	{
    printf("Can't open input %s\n", fname);
    return -1;
    }
	fscanf(in, "%s", buffer);
	if(stricmp(buffer, "STARFM_PARAMETER_START") != 0)
	{
    printf("This is not a valid input file\n");
    return -1;
    }
	while(fgets(buffer, 1000, in) != NULL) 
	{
		if(strcmp(buffer, "STARFM_PARAMETER_END") == 0) break;
		tokenptr = strtok(buffer, separator);
		label=tokenptr;
		if(strcmp(label,"#") == 0) continue;
		while(tokenptr != NULL) 
		{
			tokenptr = strtok(NULL, separator);
			if(strcmp(label, "NUM_IN_PAIRS") == 0) 
			{
				par->NUM_PAIRS = atoi(tokenptr);
				if(par->NUM_PAIRS<=0)
				{
					cerr<<"参考影像对应大于0"<<endl;
					return -1;
				}
			}
			else if(strcmp(label, "The_pf_band_of_Landsat_for_calculating") == 0)
				par->pf=atoi(tokenptr);
			else if(strcmp(label, "The_pc_band_of_Landsat_for_calculating") == 0)
				par->pc=atoi(tokenptr);
			else if(strcmp(label, "IN_PAIR_LANDSAT_FNAME") == 0)
				for(i=0; i<par->NUM_PAIRS; i++) 
				{
					sscanf(tokenptr, "%s",readpath);
					psensor[i].Read(readpath,par->pf);
					tokenptr = strtok(NULL, separator);
				}
			else if(strcmp(label, "IN_PAIR_MODIS_FNAME") == 0)
				for(i=par->NUM_PAIRS; i<2*par->NUM_PAIRS; i++) 
				{
					sscanf(tokenptr, "%s", readpath);
					psensor[i].Read(readpath,par->pc);
					tokenptr = strtok(NULL, separator);
				}
			else if(strcmp(label, "IN_PDAY_MODIS_FNAME") == 0)
			{
				 sscanf(tokenptr, "%s", readpath);
				 psensor[2*par->NUM_PAIRS].Read(readpath,par->pc);
			}
			else if(strcmp(label, "OUT_PDAY_LANDSAT_FNAME") == 0) 
			{
				sscanf(tokenptr, "%s", par->outpath);
				psensor[2*par->NUM_PAIRS+1].resize(psensor[0].getWidth(),psensor[0].getHeight());
			}
			else if(strcmp(label, "The_width_of_searching_window") == 0)
				par->WIN_SIZE=atoi(tokenptr);
			else if(strcmp(label, "Assumed_number_of_classifications") == 0)
				par->class_num=atoi(tokenptr);
			else if(strcmp(label, "The_relative_importance_of_space_distance") == 0)
				par->A=atoi(tokenptr);
			else if(strcmp(label, "Landsat_sensor_error") == 0)
				par->L_ERR=atof(tokenptr);
			else if(strcmp(label, "MODIS_sensor_error") == 0)
				par->M_ERR=atof(tokenptr);
			label = tokenptr;
		}
		
	}
	for(i=1; i<2*par->NUM_PAIRS+1; i++) 
    {
		if(psensor[0].getHeight()!=psensor[i].getHeight()||psensor[0].getWidth()!=psensor[i].getWidth())
		{
			cerr<<"输入影像图幅范围不匹配"<<endl;
			return -1;
		}
	}
	fclose(in);
  return 0;
}