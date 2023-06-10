#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_LEN 1000

typedef struct 
{
	int intValue;
	char charValue;
	float floatValue;
	double doubleValue;
	char stringValue[MAX_LEN];
	char temp_intValue[100];
	int* intPtr;
	char* charPtr;
	float* floatPtr;
	double* doublePtr;
	char* stringPtr;
	char type[20];
	char variable_name[20];
} VariableType;


void trim(char *str);
void concatenate_words(char *str);
void comma_seperator(/*char* str*/ VariableType *variableArray[], int *numVariables, char temp_type[]);
//void variable_checker(char changed_text[], VariableType variableArray[], int numVariables);
//void variable_checker(char changed_text[printf_counter]);
int isNumeric(const char* str);
void applie(void (*func)(), int number_of_applies);

char *var_name_start, *var_type_start;
int name_length, *name_seperated = 0 ;
char existing_var_type[10][20] = {"int","float","double","char","string","int.pointer","float.pointer","double.pointer","char.pointer","string.pointer"};
/* run this program using the console pauser or add your own getch, system("pause") or input loop */

int main(int argc, char *argv[]) 
{
	int k;
	for (k = 0; k < 10; k++) 
	{
    	existing_var_type[k][strlen(existing_var_type[k])] = '\0';
	}

	VariableType *variableArray[100];			
    int printf_counter = 0, numVariables = 0, End_condition = 0, command_counter = 0; //counters
    int i, j, temp_numVariables;
    char text[MAX_LEN], changed_text[MAX_LEN][MAX_LEN], temp_type[20], text_after_command[MAX_LEN][MAX_LEN];
    char *text_start, *slashcontrol, *token[MAX_LEN], *semicolonPos; //controllers
    printf("#include<stdio.h>\nint main()\n{\n    ");
    
    while (End_condition == 0) 
    {
        /*printf("Press ");
        scanf("%d", options);*/
        
        memset(text, 0, sizeof(text));
        memset(changed_text[printf_counter], 0, sizeof(changed_text[printf_counter]));
        /*fflush(stdin);
        scanf("%[^\n]", text);*/
        fgets(text, sizeof(text), stdin);
        printf("    ");
        trim(text);
        semicolonPos = strchr(text, ';');
        if(semicolonPos == NULL) //First, check is there any ';'
        {
        	printf("Error: Command sign(';') not found\n");
        	continue;
		}
        semicolonPos++;
        if(*semicolonPos == '\0') //Second, check is there anything written after ';'
        {
        	strcpy(text_after_command[command_counter], "");
		}
		else if(*semicolonPos != '\0')
		{
			strcpy(text_after_command[command_counter], semicolonPos);
		}
        token[command_counter] = strtok(text, ";");
        while(1) //Processing every command by seperating every ';' 
        {
        	if(token[command_counter] != NULL)
	        {
	        	trim(token[command_counter]);
	        	strcat(token[command_counter], ";");
	        	text_start = strstr(token[command_counter], "text(\"");
		        if (strcmp("out;", token[command_counter]) == 0) //End condition
		        {
		        	End_condition = 1;
		            break;
		        }
		        for(i = 0; i < 10; i++)
		        {
		        	var_type_start = strstr(token[command_counter], existing_var_type[i]); 
		        	if ( var_type_start != NULL )
		        	{
		        		// Allocate memory for the current VariableType struct
                		variableArray[numVariables] = malloc(sizeof(VariableType));
		        		printf("var_type_start basarili 2 = %c\n", *var_type_start);
		        		strcpy(variableArray[numVariables]->type ,existing_var_type[i]);
		        		if (strcmp(variableArray[numVariables]->type, "int") == 0) 
						{
	                    	variableArray[numVariables]->intPtr = malloc(sizeof(int));
		                } 
						else if (strcmp(variableArray[numVariables]->type, "float") == 0) 
						{
		                    variableArray[numVariables]->floatPtr = malloc(sizeof(float));
		                } 
						else if (strcmp(variableArray[numVariables]->type, "double") == 0) 
						{
		                    variableArray[numVariables]->doublePtr = malloc(sizeof(double));
		                } 
						else if (strcmp(variableArray[numVariables]->type, "char") == 0) 
						{
		                    variableArray[numVariables]->charPtr = malloc(sizeof(char));
		                } 
						else if (strcmp(variableArray[numVariables]->type, "string") == 0) 
						{
		                    variableArray[numVariables]->stringPtr = malloc(MAX_LEN * sizeof(char));
		                }
		        		printf("(*variableArray[numVariables])->type) = %s\n", variableArray[numVariables]->type);
		        		strcpy(temp_type ,existing_var_type[i]);
		        		printf("temp_type = %s\n", temp_type);
		        		break;
					}
					else if( var_type_start == NULL )
					{
						printf("written 1");
						continue;
					}
				}
				
		        if ( var_type_start != NULL) /*strstr(token, "int") != NULL || strstr(token, "char") != NULL || strstr(token, "float") != NULL || 
				strstr(token, "string") != NULL || strstr(token, "int.pointer") != NULL || strstr(token, "char.pointer") != 
				NULL || strstr(token, "float.pointer") != NULL || strstr(token, "string.pointer") != NULL*/
		        {
		        	printf("var_type_start = %c\n", *var_type_start);
		        	var_type_start += strlen(temp_type) + 1;
		        	printf("var_type_start = %c\n", *var_type_start);
		        	var_name_start = var_type_start;
		        	printf("var_name_start = %c\n", *var_name_start);
		        	var_type_start = strchr(var_name_start, '=');
		        	printf("var_type_start = %c\n", *var_type_start);
		        	if (var_type_start != NULL)
		        	{
		        		int name_length = var_type_start - var_name_start;
		        		strncpy(variableArray[numVariables]->variable_name, var_name_start, name_length);
		        		variableArray[numVariables]->variable_name[strlen(variableArray[numVariables]->variable_name)] = '\0';
		        		printf("variableArray[numVariables]->variable_name = %s1\n", variableArray[numVariables]->variable_name);
		        		concatenate_words(variableArray[numVariables]->variable_name);
		        		printf("variableArray[numVariables]->variable_name = %s1\n", variableArray[numVariables]->variable_name);
		        		temp_numVariables = numVariables;
		        		printf("program worked properly here.temp_numVariables = %d = numVariables = %d \n", temp_numVariables, numVariables);
		        		comma_seperator(variableArray, &numVariables, temp_type);
		        		printf("program worked properly here too. The name = %s\n", variableArray[numVariables]->variable_name);
						
						var_type_start = strchr(var_type_start, ';');
						var_name_start = strchr(var_name_start, '=');
						int value_length = var_type_start - var_name_start;
						
						for(i = 0; i < 10; i++)
						{
							if(variableArray[temp_numVariables]->type == existing_var_type[i] && ((i >= 0 && i <= 2) || (i >= 5 && i <= 7)) )
							{
								strncpy(variableArray[temp_numVariables]->temp_intValue, var_name_start, value_length);
								concatenate_words(variableArray[temp_numVariables]->temp_intValue);
								comma_seperator(variableArray, &temp_numVariables, existing_var_type[i]);
							}
							else if(variableArray[temp_numVariables]->type == existing_var_type[i] && ((i >= 3 && i <= 4) || (i >= 8 && i <= 9)) )
							{
								strncpy(variableArray[numVariables]->stringValue, var_name_start, value_length);
								concatenate_words(variableArray[numVariables]->stringValue);
								comma_seperator(&variableArray[numVariables], &temp_numVariables, existing_var_type[i]);
							}
						}
					}
					else if(var_type_start == NULL)
					{
						var_type_start = strchr(var_name_start, ';');
						int name_length = var_type_start - var_name_start;
						strncpy(variableArray[numVariables]->variable_name, var_name_start, name_length);
						concatenate_words(variableArray[numVariables]->variable_name);
						comma_seperator(&variableArray[numVariables], &numVariables, temp_type);
						*name_seperated = 0;
					}
					semicolonPos = strchr(text_after_command[command_counter], ';');
					if(semicolonPos == NULL)
					{
						if(strcmp(text_after_command[command_counter], "") == 0)
						{
							printf("Buradan dondu 10\n");
							break;
						}
						else if(strcmp(text_after_command[command_counter], "") != 0)
						{
							printf("Error: Command sign(';') not found after last semicolon");
							break;
						}
					}
					semicolonPos++;
					command_counter ++;
					strcpy(text_after_command[command_counter], semicolonPos);
		            token[command_counter] = strtok(text_after_command[command_counter - 1],";");
		            continue;
		        }
		        /*else if(strcmp("if", text) == 0)
		        {
		            
		        }
		        else if(strcmp("else if", text) == 0)
		        {
		            
		        }
		        else if(strcmp("else", text) == 0)
		        {
		            
		        }
		        else if(strcmp("for", text) == 0)
		        {
		            
		        }
		        else if(strcmp("while", text) == 0)
		        {
		            
		        }*/
		        
				if (text_start != NULL)
		        {
		            slashcontrol = strchr(text_start, '\\');
		            
		            if (slashcontrol == NULL)
		            {
		                char *end = strstr(text_start, "\");");
		                if (end != NULL)
		                {
		                    text_start += strlen("text(\"");
		                    int length = end - text_start;
		                    strncpy(changed_text[printf_counter], text_start, length);
		                    //variable_checker(changed_text[printf_counter], variableArray, numVariables);
		                    printf_counter++;
		                    semicolonPos = strchr(text_after_command[command_counter], ';');
		                    if(semicolonPos == NULL)
							{
								if(strcmp(text_after_command[command_counter], "") == 0)
								{
									break;
								}
								else if(strcmp(text_after_command[command_counter], "") != 0)
								{
									printf("Error: Command sign(';') not found after last semicolon");
									break;
								}
							}
							semicolonPos++;
							command_counter ++;
							strcpy(text_after_command[command_counter], semicolonPos);
				            token[command_counter] = strtok(text_after_command[command_counter - 1],";");
		                    continue;
		                }
		                else if (end == NULL)
		                {
		                    semicolonPos = strchr(text_after_command[command_counter], ';');
		                    if(semicolonPos == NULL)
							{
								if(strcmp(text_after_command[command_counter], "") == 0)
								{
									printf("Error: Incorrect usage of text function\n");
									break;
								}
								else if(strcmp(text_after_command[command_counter], "") != 0)
								{
									printf("Error: Command sign(';') not found after last semicolon\n");
									break;
								}
							}
							semicolonPos++;
							command_counter ++;
							strcpy(text_after_command[command_counter], semicolonPos);
				            token[command_counter] = strtok(text_after_command[command_counter - 1],";");
		                    continue;
		                }
		            }
		            else if (slashcontrol != NULL)
		            {
		                text_start += strlen("text(\"");
		                char next_char = *(slashcontrol + 1);
		
		                while (1) // Check every '\\' sequence
		                {
		                    if (next_char == '\\')
		                    {
		                        // Treat double backslash as a single literal backslash
		                        strncat(changed_text[printf_counter], text_start, slashcontrol - text_start + 1);
		                        text_start = slashcontrol + 2;
		                        slashcontrol = strchr(text_start, '\\');
		                        if(slashcontrol != NULL)
					            {
					            	next_char = *(slashcontrol + 1);
					            	continue;
								}
		                        else if (slashcontrol == NULL)
		                        {
		                            char *end2 = strstr(text_start, "\");");
		                            if (end2 != NULL)
		                            {
		                                int length2 = end2 - text_start;
		                                strncat(changed_text[printf_counter], text_start, length2);
		                                break;
		                            }
		                            else if (end2 == NULL)
		                            {
		                                printf("Error: Incorrect usage of text function\n");
		                                break;
		                            }
		                        }
		                    }
		                    else if (next_char == '\"')
		                    {
		                        strncat(changed_text[printf_counter], text_start, slashcontrol - text_start);
		                        strcat(changed_text[printf_counter], "\"");
		                        text_start = slashcontrol + 2;
		                        slashcontrol = strchr(text_start, '\\');
		                        if (slashcontrol != NULL)
		                        {
		                            next_char = *(slashcontrol + 1);
		                            continue;
		                        }
		                        else if (slashcontrol == NULL)
		                        {
		                            char *end2 = strstr(text_start, "\");");
		                            if (end2 != NULL)
		                            {
		                                int length2 = end2 - text_start;
		                                strncat(changed_text[printf_counter], text_start, length2);
		                                break;
		                            }
		                            else if (end2 == NULL)
		                            {
		                                printf("Error: Incorrect usage of text function\n");
		                                break;
		                            }
		                        }
		                    }
		                    else if (next_char == 'n' || next_char == 'b')
		                    {
		                        strncat(changed_text[printf_counter], text_start, slashcontrol - text_start + 2);
		                        // Replace "\n" with newline character
								for (i = 0; changed_text[printf_counter][i] != '\0'; i++)
					            {
					                if (changed_text[printf_counter][i] == '\\' && changed_text[printf_counter][i + 1] == 'n')
					                {
					                    changed_text[printf_counter][i] = '\n';
					                    changed_text[printf_counter][i + 1] = '\b';
					                }
					            }
				            
					            //backup backspace key
								for (i = 0, j = 0; changed_text[printf_counter][i] != '\0'; i++, j++) 
								{
								    if (changed_text[printf_counter][i] == '\\' && changed_text[printf_counter][i + 1] == 'b') 
									{
								        j -= 2;
								        i++;
								    } 
									else 
									{
								        changed_text[printf_counter][j] = changed_text[printf_counter][i];
								    }
								}
								changed_text[printf_counter][j] = '\0';
		                        text_start = slashcontrol + 2;
		                        slashcontrol = strchr(text_start, '\\');
		                        if (slashcontrol != NULL)
		                        {
		                            next_char = *(slashcontrol + 1);
		                            continue;
		                        }
		                        else if (slashcontrol == NULL)
		                        {
		                            char *end2 = strstr(text_start, "\");");
		                            if (end2 != NULL)
		                            {
		                                int length2 = end2 - text_start;
		                                strncat(changed_text[printf_counter], text_start, length2);
		                                printf_counter++;
		                                break;
		                            }
		                            else if (end2 == NULL)
		                            {
		                                printf("Error: Incorrect usage of text function\n");
		                                break;
		                            }
		                        }
		                    }
		                    else //text("DENEME1\\DENEME2\"DENEME3\bDENEME4\nDENEME5\DDENEME6");
		                    {
		                        strncat(changed_text[printf_counter], text_start, slashcontrol - text_start);
		                        text_start = slashcontrol + 1;
		                        slashcontrol = strchr(text_start, '\\');
		                        if (slashcontrol != NULL)
		                        {
		                            next_char = *(slashcontrol + 1);
		                            continue;
		                        }
		                        else if (slashcontrol == NULL)
		                        {
		                            char *end2 = strstr(text_start, "\");");
		                            if (end2 != NULL)
		                            {
		                                int length2 = end2 - text_start;
		                                strncat(changed_text[printf_counter], text_start, length2);
		                                break;
		                            }
		                            else if (end2 == NULL)
		                            {
		                                printf("Error: Incorrect usage of text function\n");
		                                break;
		                            }
		                        }
		                    }
		                }
		                
						//variable_checker(changed_text[printf_counter], variableArray, numVariables);
		                printf_counter++;
		                semicolonPos = strchr(text_after_command[command_counter], ';');
		                if(semicolonPos == NULL)
						{
							if(strcmp(text_after_command[command_counter], "") == 0)
							{
								break;
							}
							else if(strcmp(text_after_command[command_counter], "") != 0)
							{
								printf("Error: Command sign(';') not found after last semicolon");
								break;
							}
						}
						semicolonPos++;
						command_counter ++;
						strcpy(text_after_command[command_counter], semicolonPos);
			            token[command_counter] = strtok(text_after_command[command_counter - 1],";");
		                continue;
		            }
		        }
		        else
		        {
		        	break;
				}
			}
			else if(token[command_counter] == NULL)
			{
				for (i = 0; text_after_command[command_counter][i] != '\0'; i++)
		        {
		        	if(text_after_command[command_counter][i] == ' ' || text_after_command[command_counter][i] == '\t' || text_after_command[command_counter][i] == '\n')
					{
						break;
					}
					else
					{
						printf("Error: Command sign(';') not found after last semicolon\n");
		        		break;
					}
		        }
		        break;
			}
		}
		continue;  
    }

    printf("\b\b\b\b}\n");
    for (i = 0; i < printf_counter; i++)
    {
        printf("%s", changed_text[i]);
    }
    
    for (i = 0; i < numVariables; i++) 
	{
        free(variableArray[i]->intPtr);
        free(variableArray[i]->floatPtr);
        free(variableArray[i]->doublePtr);
        free(variableArray[i]->charPtr);
        free(variableArray[i]->stringPtr);
        free(variableArray[i]);
    }
	return 0;
}

void trim(char *str)
{
    int i, j;
    for (i = 0; str[i] == ' ' || str[i] == '\t' || str[i] == '\n'; i++)
    {
        str[i] = '\0';
    }

    if (i >= 0)
    {
        for (j = 0; str[i] != '\0'; i++, j++)
        {
            str[j] = str[i];
        }
        str[j] = '\0';
    }

    for (i = strlen(str) - 1; i >= 0 && (str[i] == ' ' || str[i] == '\t' || str[i] == '\n'); i--)
    {
        str[i] = '\0';
    }
}


void concatenate_words(char *str)
{
	int i, j;
	for (i = 0, j = 0; str[i] != '\0'; i++, j++) 
	{
	    if (str[i] != ' ') 
		{
	        str[j] = str[i];
	    }
	    else if (str[i] == ' ') 
		{
	        j--;
	    }
	}
	str[j] = '\0';
}

void comma_seperator(/*char* str*/ VariableType *variableArray[], int *numVariables, char temp_type[])
{
	int C = 0;
    char *comma_seperator[100];
    if(*name_seperated == 0)
    {
    	comma_seperator[C]= strtok(variableArray[*numVariables]->variable_name, ",");
    	if(comma_seperator[C] != NULL)
    	{
    		while(C<100) //check every comma sequence
		    {
		        if(comma_seperator[C] != NULL)
		        {
		            strcpy(variableArray[*numVariables]->variable_name, comma_seperator[C]);
		            (*numVariables)++;
		            strcpy(variableArray[*numVariables]->type, temp_type);
		            (*var_name_start) += strlen(comma_seperator[C]); 
		            C++;
		            comma_seperator[C] = strtok(NULL, ",");
		            continue;
		        }
		        else if(comma_seperator[C] == NULL)
		        {
		            name_length = (*var_type_start) - (*var_name_start); 
		            strncpy(variableArray[*numVariables]->variable_name, var_name_start, name_length);
					(*numVariables)++;
		            break;
		        }
		    }
		}
	    *name_seperated = 1;
	}
	else if(*name_seperated == 1)
	{
		int i, j;
		for(i = 0; i < 10; i++)
		{
			if( strcmp(temp_type, existing_var_type[i]) == 0 && ((i >= 0 && i <= 2) || (i >= 5 && i <= 7)) )
			{
				int mistake_places[100] = {0};
				int mistake_counter = 0;
				comma_seperator[C]= strtok(variableArray[*numVariables]->temp_intValue, ",");
				if(comma_seperator[C] != NULL)
				{
					while(C<100) //check every comma sequence
				    {
				        if(comma_seperator[C] != NULL)
				        {
				        	if(isNumeric(comma_seperator[C]))
							{
								variableArray[*numVariables]->intValue = atoi(comma_seperator[C]);
							}
							else if(!isNumeric(comma_seperator[C]))
							{
								mistake_places[C] = C + 1;
								mistake_counter ++;
							}
				            (*numVariables)++;
				            (*var_name_start) += strlen(comma_seperator[C]); 
				            C++;
				            comma_seperator[C] = strtok(NULL, ",");
				            continue;
				        }
				        else if(comma_seperator[C] == NULL)
				        {
				            name_length = (*var_type_start) - (*var_name_start); 
				            strncpy(variableArray[*numVariables]->temp_intValue, var_name_start, name_length); 
				            if(isNumeric(variableArray[*numVariables]->temp_intValue))
				            {
				            	variableArray[*numVariables]->intValue = atoi(variableArray[*numVariables]->temp_intValue);
							}
							else if(!isNumeric(variableArray[*numVariables]->temp_intValue))
							{
								mistake_places[C] = C + 1;
								mistake_counter ++;
							}
				            (*numVariables)++;
				            break;
				        }
				    }
				    if(mistake_counter == 0)
				    {
				    	break;
					}
					else if(mistake_counter > 0)
					{
						if(mistake_counter == 1)
						{
							printf("Error: value of ");
						}
						else if(mistake_counter > 1)
						{
							printf("Error: values of ");
						}
						for(i = 0, j = 0; i < 100; i++)
						{
							if(mistake_places[i] == 0)
							{
								continue;
							}
							if(mistake_counter == 1)
							{
								printf("%d. variable is not numeric\n",mistake_places[i]);
								break;
							}
							else if(mistake_counter > 1)
							{
								if(mistake_places[i] != 0 && j < mistake_counter)
								{
									if(j == mistake_counter - 2)
									{
										printf("%d and ",mistake_places[i]);	
										j++;
										continue;
									}
									printf("%d,",mistake_places[i]);
								}
								else if(mistake_places[i] != 0 && j == mistake_counter - 1)
								{
									printf("%d. variables are not numeric\n",mistake_places[i]);
									break;
								}	
							}
						}
						break;
					}
				}
				else if(comma_seperator[C] == NULL)
				{
					break;
				}
			}
			else if( strcmp(temp_type, existing_var_type[i]) == 0 && ((i >= 3 && i <= 4) || (i >= 8 && i <= 9)))
			{
				comma_seperator[C]= strtok(variableArray[*numVariables]->stringValue, ",");
				if(comma_seperator[C] != NULL)
				{
					while(C<100) //check every comma sequence
				    {
				        if(comma_seperator[C] != NULL)
				        {
				            strcpy(variableArray[*numVariables]->stringValue, comma_seperator[C]);
				            (*numVariables)++;
				            (*var_name_start) += strlen(comma_seperator[C]); 
				            C++;
				            comma_seperator[C] = strtok(NULL, ",");
				            continue;
				        }
				        else if(comma_seperator[C] == NULL)
				        {
				            name_length = (*var_type_start) - (*var_name_start); 
				            strncpy(variableArray[*numVariables]->stringValue, var_name_start, name_length);  
				            (*numVariables)++;
				            break;
				        }
				    }
				}
				else if(comma_seperator[C] == NULL)
				{
					break;
				}
			}
		}
	    *name_seperated = 0;
	}
}


					  
/*void variable_checker(char changed_text[], VariableType variableArray[], int numVariables)
{
	int i,j;
    char* addresscontrol = strchr(changed_text, '&');

    if (addresscontrol == NULL)
    {
        // Search for the variable in the variableArray based on its name
        for (i = 0; i < numVariables; i++)
        {
            if (strcmp(changed_text, variableArray[i].variable_name) == 0)
            {
                if (strcmp(variableArray[i].type, "int") == 0)
                {
                    printf("printf(\"%d\", %s);\n", variableArray[i].intValue, variableArray[i].variable_name);
                }
                else if (strcmp(variableArray[i].type, "float") == 0)
                {
                    printf("printf(\"%f\", %s);\n", variableArray[i].floatValue, variableArray[i].variable_name);
                }
                else if (strcmp(variableArray[i].type, "double") == 0)
                {
                    printf("printf(\"%lf\", %s);\n", variableArray[i].doubleValue, variableArray[i].variable_name);
                }
                else if (strcmp(variableArray[i].type, "char") == 0)
                {
                    printf("printf(\"%c\", %s);\n", variableArray[i].charValue, variableArray[i].variable_name);
                }
                else if (strcmp(variableArray[i].type, "string") == 0)
                {
                    printf("printf(\"%s\", %s);\n", variableArray[i].stringValue, variableArray[i].variable_name);
                }
                break;
            }
        }
    }
    else if (addresscontrol != NULL)
    {
        // Remove the '&' symbol from the changed_text
        for (i = 0; changed_text[i] != '\0'; i++)
        {
            if (changed_text[i] == '&')
            {
                for (j = i; changed_text[j] != '\0'; j++)
                {
                    changed_text[j] = changed_text[j + 1];
                }
                break;
            }
        }

        // Search for the variable in the variableArray based on its name
        for (i = 0; i < numVariables; i++)
        {
            if (strcmp(changed_text, variableArray[i].variable_name) == 0)
            {
                if (strcmp(variableArray[i].type, "int.pointer") == 0)
                {
                    printf("printf(\"%p\", %s);\n", variableArray[i].intPtr, variableArray[i].variable_name);
                }
                else if (strcmp(variableArray[i].type, "float.pointer") == 0)
                {
                    printf("printf(\"%p\", %s);\n", variableArray[i].floatPtr, variableArray[i].variable_name);
                }
                else if (strcmp(variableArray[i].type, "double.pointer") == 0)
                {
                    printf("printf(\"%p\", %s);\n", variableArray[i].doublePtr, variableArray[i].variable_name);
                }
                else if (strcmp(variableArray[i].type, "char.pointer") == 0)
                {
                    printf("printf(\"%p\", %s);\n", variableArray[i].charPtr, variableArray[i].variable_name);
                }
                else if (strcmp(variableArray[i].type, "string.pointer") == 0)
                {
                    printf("printf(\"%p\", %s);\n", variableArray[i].stringPtr, variableArray[i].variable_name);
                }
                break;
            }
        }
    }
}*/


int isNumeric(const char *str) 
{
    if (str == NULL || *str == '\0') 
	{
        return 0;  // Empty string or NULL pointer
    }

    while (*str) 
	{
        if (*str < '0' || *str > '9') 
		{
            return 0;  // Non-digit character found
        }
        str++;
    }

    return 1;  // All characters are digits
}

void applie(void (*func)(), int number_of_applies) // Calling the function through the function pointer
{
	int applied_amount;
	for(applied_amount = 0; applied_amount < number_of_applies; applied_amount++)
	{
		func(); 
	}
}
