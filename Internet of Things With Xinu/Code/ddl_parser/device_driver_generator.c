
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"cJSON.h"

char xinu_directory[100] ="/home/nikita/Desktop/DOSP_PROJECT_FINAL_LINUX/xinu_jinal/xinu_jinal";

struct highLevelDriverFunc{
	char returnType[50];
	char funcName[50];
	char argument[50];
	char code[200];
};

void create_func(struct highLevelDriverFunc *func, char * generated_func)
{
	strcpy(generated_func, func->returnType);
	strcat(generated_func, " ");
	strcat(generated_func, func->funcName);
	strcat(generated_func, "(");
	strcat(generated_func, func->argument);
	strcat(generated_func, ")\n{");
	strcat(generated_func, func->code);
	strcat(generated_func, "}");
}

void edit_prototypes_h(struct highLevelDriverFunc *func)
{
			char fileName[200];
			strcpy(fileName, xinu_directory);
			strcat(fileName,"/include/prototypes.h");
			FILE *fp=fopen(fileName, "a");
			//add to file the following
			//extern devcall	tempread(struct dentry *, char *, int32);
			char func_entry[200];
			strcpy(func_entry,"\n\nextern devcall ");
			strcat(func_entry,func->funcName);
			strcat(func_entry, "(");
			strcat(func_entry, func->argument);
			strcat(func_entry,");\n");
			fprintf(fp, func_entry);
		    if(fp!=NULL) fclose(fp);
}

char* func1[] = {"-i","-o","-c","-r","-g","-p","-w","-s","-n","-intr"};

void create_device_entry(char *dev_entry, char* device_name, char* device_type, char* work)
{
	strcpy(dev_entry,"\n/* type of a ");
	strcat(dev_entry, device_name);
	strcat(dev_entry," device */\n");
	strcat(dev_entry,device_name);
	strcat(dev_entry,":\n\t");
	strcat(dev_entry,"on ");
	strcat(dev_entry,device_type);
	strcat(dev_entry,"\n\t\t");
	int func_to_set=-1;
	if(!strcasecmp(work,"read"))
		{
			func_to_set=3;
		}
	else if(!strcasecmp(work,"write"))
		{
			func_to_set=6;
		}

			int j;
			for(j=0;j<10;j++)
				{
				if(j %3 ==0)
				{
					strcat(dev_entry,"\n\t\t");
				}
				strcat(dev_entry,func1[j]);
				strcat(dev_entry," ");
				if(j==func_to_set)
					{
					strcat(dev_entry,device_name);
					strcat(dev_entry,work);
					}
				else
					{
					strcat(dev_entry,"ionull");
					}
				strcat(dev_entry,"\t");
				}


	strcat(dev_entry,"\n%%");
	strcat(dev_entry,"%%");
}

void create_device_declaration(char * device_declaration, char * device_name, char * device_type)
{
	strcpy(device_declaration,"\n\t/*Define a ");
	strcat(device_declaration,device_name);
	strcat(device_declaration," device */\n\t");
	char device_name_upper_case[20];
	strcpy(device_name_upper_case,device_name);
	strcat(device_declaration,device_name_upper_case);
	strcat(device_declaration," is ");
	strcat(device_declaration,device_name);
	strcat(device_declaration," on ");
	strcat(device_declaration,device_type);
	strcat(device_declaration,"\n\n");
	strcat(device_declaration,"%%");
	strcat(device_declaration,"%%");
}

void edit_conf(char* device_name, char* device_type, char* work)
{
	
	char fileName1[200];
	strcpy(fileName1, xinu_directory);
	strcat(fileName1,"/config/Configuration");
	FILE *fp=fopen(fileName1, "r");
	
	char fileName2[200];
	strcpy(fileName2, xinu_directory);
	strcat(fileName2,"/config/Configuration2");
	FILE *fp2=fopen(fileName2, "w+");
			

	//FILE *fp=fopen("Configuration", "r");
		    //FILE *fp2=fopen("Configuration2", "w+");
		    char tmp[256]={0x0};
		    int first_encounter=0;
		    while(fp!=NULL && fgets(tmp, sizeof(tmp), fp)!=NULL)
		    {
		        if (strstr(tmp, "%%") && first_encounter==0)
		        	{
		        		first_encounter++;
		        		char dev_entry[200];
		        		create_device_entry(dev_entry,device_name, device_type,work);
		        		fprintf(fp2, dev_entry);
		        	}
		        else if(strstr(tmp, "%%") && first_encounter==1)
		        	{
		        		first_encounter++;
		        		char device_declaration[200];
		        		create_device_declaration(device_declaration, device_name, device_type);
		        		fprintf(fp2, device_declaration);
		        	}
		        else
		        	{
		        	fprintf(fp2, tmp);
		        	}
		    }
		    if(fp!=NULL) fclose(fp);
		    if(fp2!=NULL) fclose(fp2);
		    //remove("Configuration");
		    //rename("Configuration2","Configuration");

		    remove(fileName1);
		    rename(fileName2,fileName1);


}

int main(int n, char* args[])
{
	char * myJsonString;
	long length;
	//mode is rb because the file is other than .txt
	
	char fileNamejson[200];
	strcpy(fileNamejson, xinu_directory);
	strcat(fileNamejson,"/ddl_parser/our_device.json");
	FILE *f=fopen(fileNamejson, "rb");
	

	//FILE * f = fopen ("our_device.json", "rb");

	if (f) //if file exists
	{
	  fseek (f, 0, SEEK_END); //set the pointer to end of the file
	  length = ftell (f); //get the pointer position to get the length
	  fseek (f, 0, SEEK_SET); //set the pointer back to the origin ie SEEK_SET

	  myJsonString = malloc (length);
	  if (myJsonString)
	    {
	      fread (myJsonString, 1, length, f);
	      cJSON * root = cJSON_Parse(myJsonString);
	      root = root->child;
	      int i=0;
	      int no_of_devices = cJSON_GetArraySize(root);
	      FILE *deviceFilePtr;
	      while(i<no_of_devices)
	    	  {
	    	   cJSON * device = cJSON_GetArrayItem(root, i);
	    	   device = device->child;
	    	   int sensor_flag=0;
	    	   struct highLevelDriverFunc func;
	    	   char device_num[50];
	    	   char *device_name;
	    	   char *work;
	    	   char *device_type;
	    	   while (device)
	    	   	      {
	    		     strcpy(func.returnType,"devcall");
	    		   	 char fileLocation[200],fileName[400], extenstion[2];


	    		   	 if(!strcasecmp(device->string,"name"))
	    		   	  {
	    		   		strcpy(fileName,xinu_directory);
	    		   		strcat(fileName,"/system/");
	    		   		device_name = device->valuestring;
	    		   		strcat(fileName, device->valuestring);
	    		   		strcat(fileName,".c");
	    		   		//strcpy(extenstion, ".c");
	    		   	    //strcpy(fileLocation, "/");
	    		   	    //strcat(fileLocation, fileName);
	    		   	    deviceFilePtr = fopen(fileName, "w+");
	    		   		fprintf(deviceFilePtr, "#include<xinu.h>\n\n");
	    		   	  }
                        if(!strcasecmp(device->string,"description"))
	    	   	    	  	  {
	    	   	    		   if(!strcasecmp(device->valuestring,"sensor"))
	    	   	    		   	   {
	    	   	    			   	   sensor_flag=1;
	    	   	    		   	   }
	    	   	    	  	  }
                        if(!strcasecmp(device->string,"work"))
                        		{
                        			work = device->valuestring;
                        			strcpy(func.funcName,device_name);
                        			strcat(func.funcName,device->valuestring);

                        		}
                        if(!strcasecmp(device->string,"type"))
                                {
                        			device_type = device->valuestring;
                                }

                        if(!strcasecmp(device->string,"pin"))
                        		{
                        			strcpy(func.argument, "struct dentry	*devptr, char	*buff,  int32	count");

                        			if(!sensor_flag)
                        			{
                        				strcpy(func.code,"\nwrite_pin(");
                        				strcat(func.code,device->valuestring);
                        				strcat(func.code,", buff);\n");
                        				strcat(func.code,"return OK;");
                        			}
                        			else if(sensor_flag && strcasecmp(device->valuestring, "ADC"))
                        			{
                        				strcpy(func.code,"\nread_pin(");
                        				strcat(func.code,device->valuestring);
                        				strcat(func.code,", buff);\n");
                        				strcat(func.code,"return OK;");
                        			}
                        			else
                        			{
                        				
                        				strcpy(func.code,"\nchar data_adc[4];\n");
										strcat(func.code,"read(");
                        				strcat(func.code, device->valuestring);
                        				strcat(func.code,",(char*)data_adc,4);\n");
                        				strcat(func.code,"unsigned int data = *(unsigned int*)data_adc;\n");

                        			}

                        		}
                        if(!strcasecmp(device->string,"reading"))
                        	{

                        		strcat(func.code,"int32 var = (unsigned int)");
                        		strcat(func.code,device->valuestring);
                        		strcat(func.code,";\n");
                        		strcat(func.code,"intToStr(var, buff, 0);\n");
                        		strcat(func.code,"return OK;\n");
                        	}

                        if(device->child)
	    	   	    	  {
	    	   	    		device = device->child;
	    	   	    	  }
	    	   	    	  else
	    	   	    	  {
	    	   	    		device = device->next;
	    	   	    	  }
	    	  }
	    	   char generated_func[500];
	    	   create_func(&func, generated_func);
	    	   fprintf(deviceFilePtr, generated_func);
	    	   fclose(deviceFilePtr);

	    	   edit_conf(device_name, device_type, work);
	    	   edit_prototypes_h(&func);
	    	   i++;
	    	  }
	    }
	    fclose (f);
	  }

		return 0;


	}
