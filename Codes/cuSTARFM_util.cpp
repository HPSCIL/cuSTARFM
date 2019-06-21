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
  printf("\tThe relative importance of space distance =\n");
  printf("\tAssumed_number_of_classifications = \n");
 // printf("\t =\n");
  printf("\tLandsat_sensor_error= \n");
  printf("\tMODIS_sensor_error = \n"); 
  printf("STARFM_PARAMETER_END\n");
}
int parseParameters(char *fname, CuLayer *psensor,PARAMETER *par)
{
	  int   i, k, total_pairs=0;
	  par->NUM_PREDICTIONS = 0;
  char  buffer[1000] = "\0";
  char  *label =NULL;
  char  *tokenptr =NULL;
  char readpath[1000];
  string argName;
  string t;
  char  *separator = "= ,";
  FILE  *in;  
  char *token = NULL;
	//char  buffer[1000] = "\0";
	if((fopen_s(&in,fname,"r"))!=0) 
	{
    printf("Can't open input %s\n", fname);
    return -1;
    }
	fscanf_s(in, "%s", buffer,1000);
	if(strcmp(buffer, "STARFM_PARAMETER_START") != 0)
	{
    printf("This is not a valid input file\n");
    return -1;
    }
	int nn=0;
	while(1) 
	{
		nn++;
		if(nn>1000)
		{
			cerr<<"配置文件应该用STARFM_PARAMETER_END结尾"<<endl;
				break;
		}
		memset(buffer,0,1000);
		if(fgets(buffer, 1000, in)==NULL)
			continue;
		if(strcmp(buffer, "STARFM_PARAMETER_END") == 0) 
			break;
		tokenptr = strtok_s(buffer, separator,&token);
		label=tokenptr;
		if(strcmp(label,"#") == 0) continue;
		while(tokenptr != NULL) 
		{
			tokenptr = strtok_s(NULL, separator, &token);
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
			else if(strcmp(label, "G_Type") == 0)
				sscanf_s(tokenptr, "%s",par->G_Type,50);
			else if(strcmp(label, "The_pc_band_of_MODIS_for_calculating") == 0)
				par->pc=atoi(tokenptr);
			else if(strcmp(label, "IN_PAIR_LANDSAT_FNAME") == 0)
				for(i=0; i<par->NUM_PAIRS; i++) 
				{
					sscanf_s(tokenptr, "%s",readpath,1000);
					psensor[i].Read(readpath,par->pf);
					tokenptr = strtok_s(NULL, separator,&token);
				}
			else if(strcmp(label, "IN_PAIR_MODIS_FNAME") == 0)
				for(i=par->NUM_PAIRS; i<2*par->NUM_PAIRS; i++) 
				{
					sscanf_s(tokenptr, "%s", readpath,1000);
					psensor[i].Read(readpath,par->pc);
					tokenptr = strtok_s(NULL, separator,&token);
				}
			else if(strcmp(label, "IN_PDAY_MODIS_FNAME") == 0)
			{
				k = 0;
				do 
				{
				 sscanf_s(tokenptr, "%s", readpath,1000);
				 tokenptr = strtok_s(NULL, separator, &token);
				 psensor[2*(par->NUM_PAIRS+k)].Read(readpath,par->pc);
				 k++;
				}while(tokenptr != NULL);
				if(par->NUM_PREDICTIONS == 0)
					par->NUM_PREDICTIONS = k;
				else if(k != par->NUM_PREDICTIONS) 
				{
					printf("\nnumber of IN_PDAY_MODIS_MASK does not match IN_PDAY_MODIS_FNAME\n");
					return -1;
				}
			}
			else if(strcmp(label, "OUT_PDAY_LANDSAT_FNAME") == 0) 
			{
				k = 0;
				do
				{
				sscanf_s(tokenptr, "%s",  psensor[2*(par->NUM_PAIRS+k)+1].outpath,1000);
				//psensor[2*par->NUM_PAIRS+1].resize(psensor[0].getWidth(),psensor[0].getHeight());
				 tokenptr = strtok_s(NULL, separator,&token);
				k++;
				}while(tokenptr != NULL);
				if(par->NUM_PREDICTIONS == 0)
					par->NUM_PREDICTIONS = k;
				else if(k != par->NUM_PREDICTIONS) 
				{
					printf("\nnumber of IN_PDAY_MODIS_MASK does not match IN_PDAY_MODIS_FNAME\n");
					return -1;
				}
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
			cerr<<""<<endl;
			return -1;
		}
	}
	fclose(in);
  return 0;
}
