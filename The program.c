#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/ioctl.h>
#include <unistd.h>
#endif

#define MAX_LEN 1000

typedef struct 
{
	int Priority_level;
	const char * command_format[10];
} Command;

typedef struct 
{
	int array_size;
	char array_name[100];
	char int_values[100];
	char string_values[100];
} Array;

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
	char variable_name[200];
	Array *array;
} VariableType;

typedef struct 
{
	const char * defined_functions[100];
	const char * custom_functions[100];
} Function;


void delete_comment(char *str);
void trim(char *str);
int semicolon_checker(char *str, ptrdiff_t *index, char **checker, int *temp_indexs);
int is_index_in_text(const char *str, ptrdiff_t index, int *pharantesis_checker);
void concatenate_words(char *str, int start, int end);
void insert(char* str1, const char* str2, int position);
int is_in_struct(const VariableType** variableArray, const char* str, int numVariable);
void mistake_specifier_in_comma_seperator(const int * mistake_places, int mistake_counter, int function_part);
void comma_seperator(VariableType*** variableArray, Array*** array, int *numVariables, int* array_counter, const char temp_type[]);
//void variable_checker(char changed_text[], VariableType variableArray[], int numVariables);
//void variable_checker(char changed_text[printf_counter]);
int isNumeric(char* str);
void applie(void (*func)(), int number_of_applies);
void sleep_ms(unsigned int milliseconds);

size_t dynamicSize;
char *var_name_start, *var_type_start; int name_length;
int semicolon_counter = 0, array_points[25] = {0}, seperated_value_capacity = 0, zero = 0, *name_seperated = &zero ; 
const char existing_var_type[10][20] = {"int ","float ","double ","char ","string ","int.pointer ","float.pointer ","double.pointer ","char.pointer ","string.pointer "};
// run this program using the console pauser or add your own getch, system("pause") or input loop 

int main(int argc, char *argv[]) 
{
	srand(time(NULL));
	VariableType** variableArray = (VariableType**)malloc(1 * sizeof(VariableType*));
	Array** array = (Array**)malloc(1 * sizeof(Array*));
	dynamicSize = _msize(variableArray);
	if (variableArray == NULL) 
	{
        printf("1-Memory reallocation failed.\n");
        exit(1);
    }
    else
    {
    	printf("1-Size of variableArray: %zu bytes1\n", dynamicSize);
	}		
    int printf_counter = 0, numVariables = 0, temp_numVariables = 0, array_counter = 0, command_counter = 0; //counters
    int i, j, temp_indexs[25] = {0} ; ptrdiff_t index;
    char text[MAX_LEN], changed_text[MAX_LEN][MAX_LEN], temp_type[20], text_after_command[MAX_LEN][MAX_LEN];
    char *text_start, *slashcontrol, *token[MAX_LEN], *semicolonPos, *appenderPos, *commaPos; int result, End_condition = 0; //controllers
    printf("#include<stdio.h>\nint main()\n{\n    ");
    
    while (End_condition == 0) 
    {
        memset(text, 0, sizeof(text));
        memset(changed_text[printf_counter], 0, sizeof(changed_text[printf_counter]));
        fgets(text, sizeof(text), stdin);
        printf("    ");
        delete_comment(text);
        trim(text);
        if(text[0] == '\0')
        {
        	continue;
		}
		else if(text[0] == ';')
        {
        	printf("Error: There is no command before first semicolon\n    ");
        	continue;
		}
		else if(text[0] == '>' && text[1] == '>')
		{
			printf("Error: Appender sign(\">>\") can't be used at the beginning of line\n    ");
        	continue;
		}
        semicolonPos = strchr(text, ';');
        if(semicolonPos == NULL) //First, check is there any ';'
        {
        	appenderPos = strstr(text, ">>");
        	if(appenderPos == NULL)
        	{
        		strcpy(text_after_command[command_counter], "");
				token[command_counter] = text;
			}
			else if(appenderPos != NULL)
			{
				index = appenderPos - text; 
				result = semicolon_checker(text, &index, &appenderPos, temp_indexs);
				if(temp_indexs[0] != 0)
	        	{
	        		for(i = 0; i < semicolon_counter; i++)
		        	{
		        		token[command_counter][temp_indexs[i]] = '<';
					}
					semicolon_counter = 0;
					memset(temp_indexs, 0, sizeof(temp_indexs));
				}
				if(!result)
				{
					strcpy(text_after_command[command_counter], "");
					token[command_counter] = text;
				}
				else if(result)
				{
					appenderPos++;
					strcpy(text_after_command[command_counter], appenderPos);
					token[command_counter] = strtok(text, ">>");
									
				}
			}
        	/*printf("Error: Command sign ';' not found\n    ");
        	continue;*/
		}
		else if(semicolonPos != NULL)
		{
			index = semicolonPos - text; 
			result = semicolon_checker(text, &index, &semicolonPos, temp_indexs);
		}
		if(!result)
		{
			strcpy(text_after_command[command_counter], "");
			token[command_counter] = text;
			/*printf("Error: There must be ';' after pharantesis to end command\n    ");
        	continue;*/
		}
		else if(result)
		{
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
		}
        while(1) //Processing every command by seperating every ';' 
        {
        	if(temp_indexs[0] != 0)
        	{
        		for(i = 0; i < semicolon_counter; i++)
	        	{
	        		token[command_counter][temp_indexs[i]] = ';';
				}
				semicolon_counter = 0;
				memset(temp_indexs, 0, sizeof(temp_indexs));
			}
        	if(token[command_counter] != NULL)
	        {
	        	trim(token[command_counter]);
	        	strcat(token[command_counter], ";");
	        	text_start = strstr(token[command_counter], "text");
		        if (strcmp("out;", token[command_counter]) == 0) //End condition
		        {
		        	command_counter++;
		        	End_condition = 1;
		            break;
		        }
		        
		        if (text_start != NULL)
		        {
		        	if (text_start == token[command_counter]) 
					{
						text_start += 4;
			            while (*text_start == ' ')
			            {
			            	text_start++;
						}
			            if (*text_start == '(' && *(text_start + 1) == '\"') 
						{
							text_start += 2;
			                slashcontrol = strchr(text_start, '\\');
		            
				            if (slashcontrol == NULL)
				            {
				                char *end = strstr(text_start, "\")");
				                if (end != NULL)
				                {
				                    //text_start += strlen("text(\"");
				                    int length = end - text_start;
				                    strncpy(changed_text[printf_counter], text_start, length);
				                    //variable_checker(changed_text[printf_counter], variableArray, numVariables);
				                    printf_counter++;
				                    /*command_counter ++;
				                    semicolonPos = strchr(text_after_command[command_counter - 1], ';');
				                    if(semicolonPos == NULL)
									{
										if(strcmp(text_after_command[command_counter - 1], "") == 0)
										{
											break;
										}
										else if(strcmp(text_after_command[command_counter - 1], "") != 0)
										{
											printf("Error: Command sign(';') not found after last semicolon\n    ");
											break;
										}
									}
									
									result = semicolon_checker(text_after_command[command_counter - 1], &index, &semicolonPos, temp_indexs);
									if(temp_indexs[0] != 0)
						        	{
						        		for(i = 0; i < semicolon_counter; i++)
							        	{
							        		text_after_command[command_counter - 1][temp_indexs[i]] = ';';
										}
										semicolon_counter = 0;
										memset(temp_indexs, 0, sizeof(temp_indexs));
									}
									if(!result)
									{
										printf("Error: There must be ';' after pharantesis to end command\n    ");
							        	break;
									}
									semicolonPos++;
									strcpy(text_after_command[command_counter], semicolonPos);
						            token[command_counter] = strtok(text_after_command[command_counter - 1],";");*/
				                    continue;
				                }
				                else if (end == NULL)
				                {
				                    semicolonPos = strchr(text_after_command[command_counter], ';');
				                    if(semicolonPos == NULL)
									{
										if(strcmp(text_after_command[command_counter], "") == 0)
										{
											printf("Error: Incorrect usage of text function\n    ");
											break;
										}
										else if(strcmp(text_after_command[command_counter], "") != 0)
										{
											printf("Error: Incorrect usage of text function\n    ");
											printf("Error: Command sign(';') not found after last semicolon\n    ");
											break;
										}
									}
									printf("Warning: Incorrect usage of text function\n    ");
									
									result = semicolon_checker(text_after_command[command_counter], &index, &semicolonPos, temp_indexs);
									if(temp_indexs[0] != 0)
						        	{
						        		for(i = 0; i < semicolon_counter; i++)
							        	{
							        		text_after_command[command_counter][temp_indexs[i]] = ';';
										}
										semicolon_counter = 0;
										memset(temp_indexs, 0, sizeof(temp_indexs));
									}
									if(!result)
									{
										printf("Error: There must be ';' after pharantesis to end command\n    ");
							        	break;
									}
									command_counter ++;
									semicolonPos++;
									strcpy(text_after_command[command_counter], semicolonPos);
						            token[command_counter] = strtok(text_after_command[command_counter - 1],";");
				                    continue;
				                }
				            }
				            else if (slashcontrol != NULL)
				            {
				                //text_start += strlen("text(\"");
				                char next_char = *(slashcontrol + 1);
				                char *end2;
				
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
				                            end2 = strstr(text_start, "\")");
				                            if (end2 != NULL)
				                            {
				                                int length2 = end2 - text_start;
				                                strncat(changed_text[printf_counter], text_start, length2);
				                                break;
				                            }
				                            else if (end2 == NULL)
				                            {
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
				                            end2 = strstr(text_start, "\")");
				                            if (end2 != NULL)
				                            {
				                                int length2 = end2 - text_start;
				                                strncat(changed_text[printf_counter], text_start, length2);
				                                break;
				                            }
				                            else if (end2 == NULL)
				                            {
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
				                            end2 = strstr(text_start, "\")");
				                            if (end2 != NULL)
				                            {
				                                int length2 = end2 - text_start;
				                                strncat(changed_text[printf_counter], text_start, length2);
				                                printf_counter++;
				                                break;
				                            }
				                            else if (end2 == NULL)
				                            {
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
				                            end2 = strstr(text_start, "\")");
				                            if (end2 != NULL)
				                            {
				                                int length2 = end2 - text_start;
				                                strncat(changed_text[printf_counter], text_start, length2);
				                                break;
				                            }
				                            else if (end2 == NULL)
				                            {
				                                break;
				                            }
				                        }
				                    }
				                }
				                if(end2 != NULL)
								{
									printf_counter++;
								}
				                
								//variable_checker(changed_text[printf_counter], variableArray, numVariables);
				                semicolonPos = strchr(text_after_command[command_counter], ';');
				                if(semicolonPos == NULL)
								{
									if(end2 == NULL)
									{
										printf("Error: Incorrect usage of text function\n    ");
									}
									else if(end2 != NULL)
									{
										command_counter ++;
									}
									if(strcmp(text_after_command[command_counter], "") == 0)
									{
										break;
									}
									else if(strcmp(text_after_command[command_counter], "") != 0)
									{
										printf("Error: Command sign(';') not found after last semicolon\n    ");
										break;
									}
								}
								if(end2 == NULL)
								{
									printf("Warning: Incorrect usage of text function\n    ");
								}
								
								result = semicolon_checker(text_after_command[command_counter], &index, &semicolonPos, temp_indexs);
								if(temp_indexs[0] != 0)
					        	{
					        		for(i = 0; i < semicolon_counter; i++)
						        	{
						        		text_after_command[command_counter][temp_indexs[i]] = ';';
									}
									semicolon_counter = 0;
									memset(temp_indexs, 0, sizeof(temp_indexs));
								}
								if(!result)
								{
									printf("Error: There must be ';' after pharantesis to end command\n    ");
						        	break;
								}
								command_counter ++;
								semicolonPos++;
								strcpy(text_after_command[command_counter], semicolonPos);
					            token[command_counter] = strtok(text_after_command[command_counter - 1],";");
				                continue;
				            }
			            }
			            else
			            {
			            	printf("Error:Unknown command --> \"%s\"\n    ", token[command_counter]);
			            	break;
						}
					}
		        }
		        
		        for(i = 0; i < 10; i++)
		        {
		        	var_type_start = strstr(token[command_counter], existing_var_type[i]); 
		        	if ( var_type_start != NULL )
		        	{
		        		if(var_type_start == token[command_counter])
		        		{
		        			// Allocate memory for the current VariableType struct
	                		variableArray[numVariables] = (VariableType*)malloc(sizeof(VariableType));
	                		dynamicSize = _msize(variableArray[numVariables]);
	                		if (variableArray[numVariables] == NULL) 
							{
						        printf("2-Memory reallocation failed.\n");
						        exit(1);
						    }
						    else
						    {
						    	printf("2-Size of variableArray[%d]: %zu bytes\n", numVariables, dynamicSize);
							}
			        		strcpy(temp_type ,existing_var_type[i]);
			        		if (strcmp(temp_type, existing_var_type[0]) == 0 || strcmp(temp_type, existing_var_type[1]) == 0 ||
							strcmp(temp_type, existing_var_type[2]) == 0 )
		                    {
		                        variableArray[numVariables]->intPtr = (int*)malloc(sizeof(int));
		                        variableArray[numVariables]->intPtr[0] = 0;
		                        variableArray[numVariables]->floatPtr = (float*)malloc(sizeof(float));
		                        variableArray[numVariables]->floatPtr[0] = 0.0;
		                        variableArray[numVariables]->doublePtr = (double*)malloc(sizeof(double));
		                        variableArray[numVariables]->doublePtr[0] = 0.0;
		                    }
		                    else if (strcmp(temp_type, existing_var_type[3]) == 0 || strcmp(temp_type, existing_var_type[4]) == 0)
		                    {
		                        variableArray[numVariables]->charPtr = (char*)malloc(sizeof(char));
		                        variableArray[numVariables]->charPtr[0] = '\0';
		                        variableArray[numVariables]->stringPtr = (char*)malloc(MAX_LEN * sizeof(char));
		                        variableArray[numVariables]->stringPtr[0] = '\0';
		                    }
		                    else if (strcmp(temp_type, existing_var_type[5]) == 0)
		                    {
		                        variableArray[numVariables]->intPtr = (int*)malloc(sizeof(int));
		                    }
		                    else if (strcmp(temp_type, existing_var_type[6]) == 0)
		                    {
		                        variableArray[numVariables]->floatPtr = (float*)malloc(sizeof(float));
		                    }
		                    else if (strcmp(temp_type, existing_var_type[7]) == 0) 
		                    {
		                        variableArray[numVariables]->doublePtr = (double*)malloc(sizeof(double));
		                    }
		                    else if (strcmp(temp_type, existing_var_type[8]) == 0)
		                    {
		                        variableArray[numVariables]->charPtr = (char*)malloc(sizeof(char));
		                    }
		                    else if (strcmp(temp_type, existing_var_type[9]) == 0)
		                    {
		                        variableArray[numVariables]->stringPtr = (char*)malloc(MAX_LEN * sizeof(char));
		                    }
		                    variableArray[numVariables]->array = (Array*)malloc(sizeof(Array));
			        		break;
						}
						else
						{
							var_type_start = NULL;
							break;
						}
					}
					else if( var_type_start == NULL )
					{
						continue;
					}
				}
				
		        if ( var_type_start != NULL)
		        {
		        	concatenate_words(token[command_counter], strlen(temp_type), strlen(token[command_counter]) - 1);
		        	var_type_start = strstr(token[command_counter], existing_var_type[i]); 
		        	var_type_start += strlen(temp_type);
		        	var_name_start = var_type_start;
		        	var_type_start = strchr(token[command_counter], '=');
		        	if (var_type_start != NULL)
		        	{
		        		int name_length = var_type_start - var_name_start;
		        		strncpy(variableArray[numVariables]->variable_name, var_name_start, name_length);
		        		variableArray[numVariables]->variable_name[name_length] = '\0';					
		        		strcpy(variableArray[numVariables]->type ,existing_var_type[i]);
		        		variableArray[numVariables]->type[strlen(existing_var_type[i])] = '\0';
		        		temp_numVariables = numVariables;
		        		comma_seperator(&variableArray, &array, &numVariables, &array_counter, temp_type);
		        		if(*name_seperated == 0)
		        		{
		        			break;
						}
						dynamicSize = _msize(variableArray);
						printf("Size of variableArray: %zu bytes\n", dynamicSize);
		        		printf("program worked properly here. The names:\n");
		        		for(i = 0; i<numVariables; i++)
		        		{
		        			if(strcmp(variableArray[i]->array->array_name, "") != 0)
		        			{
		        				printf("%s  ", variableArray[i]->array->array_name);
							}
		        			printf("%s\n", variableArray[i]->variable_name);
						}
						var_type_start = strchr(var_type_start, ';');
						index = var_type_start - token[command_counter];
						if(!semicolon_checker(token[command_counter], &index, &var_type_start, temp_indexs))
						{
							printf("Error: There must be ';' after pharantesis to end command\n    ");
				        	continue;
						}
						if(temp_indexs[0] != 0)
			        	{
			        		for(i = 0; i < semicolon_counter; i++)
				        	{
				        		token[command_counter][temp_indexs[i]] = ';';
							}
							semicolon_counter = 0;
							memset(temp_indexs, 0, sizeof(temp_indexs));
						}
						var_name_start = strchr(var_name_start, '=');
						var_name_start += 1;
						int value_length = var_type_start - var_name_start;
						
						for(i = 0; i < 10; i++)
						{
							if( (strcmp(variableArray[temp_numVariables]->type, existing_var_type[i]) == 0) && ((i >= 0 && i <= 2) || (i >= 5 && i <= 7)) )
							{
								printf("came 1\n");
								strncpy(variableArray[temp_numVariables]->temp_intValue, var_name_start, value_length);
								variableArray[temp_numVariables]->temp_intValue[value_length] = '\0';
								printf("variableArray[temp_numVariables]->temp_intValue = %s\n", variableArray[temp_numVariables]->temp_intValue);
								comma_seperator(&variableArray, &array, &temp_numVariables, &array_counter, existing_var_type[i]);
								if(*name_seperated == 1)
				        		{
				        			break;
								}
								memset(array_points, 0, sizeof(array_points));
								printf("program worked properly here too. The values:\n");
								for(i = 0; i<numVariables; i++)
				        		{
				        			printf("%lf  %f  %d\n",variableArray[i]->doubleValue, variableArray[i]->floatValue, variableArray[i]->intValue);
								}
							}
							else if( (strcmp(variableArray[temp_numVariables]->type, existing_var_type[i]) == 0) && ((i >= 3 && i <= 4) || (i >= 8 && i <= 9)) )
							{
								printf("\ncame 2\n");
								strncpy(variableArray[temp_numVariables]->stringValue, var_name_start, value_length);
								variableArray[temp_numVariables]->stringValue[value_length] = '\0';
								printf("variableArray[temp_numVariables]->stringValue = %s\n", variableArray[temp_numVariables]->stringValue);
								comma_seperator(&variableArray, &array, &temp_numVariables, &array_counter, existing_var_type[i]);
								if(*name_seperated == 1)
				        		{
				        			break;
								}
								memset(array_points, 0, sizeof(array_points));
								printf("program worked properly here too. The values:\n");
								for(i = 0; i<numVariables; i++)
				        		{
				        			printf("%s\n", variableArray[i]->stringValue);
								}
							}
						}
						if(*name_seperated == 1)
		        		{
		        			*name_seperated = 0;
		        			break;
						}
					}
					else if(var_type_start == NULL)
					{
						var_type_start = strchr(var_name_start, ';');
						index = var_type_start - token[command_counter];
						if(!semicolon_checker(token[command_counter], &index, &var_type_start, temp_indexs))
						{
							printf("Error: There must be ';' after pharantesis to end command\n    ");
				        	continue;
						}
						if(temp_indexs[0] != 0)
			        	{
			        		for(i = 0; i < semicolon_counter; i++)
				        	{
				        		token[command_counter][temp_indexs[i]] = ';';
							}
							semicolon_counter = 0;
							memset(temp_indexs, 0, sizeof(temp_indexs));
						}
						int name_length = var_type_start - var_name_start;
						strncpy(variableArray[numVariables]->variable_name, var_name_start, name_length);
						variableArray[numVariables]->variable_name[name_length] = '\0';					
		        		strcpy(variableArray[numVariables]->type ,existing_var_type[i]);
		        		variableArray[numVariables]->type[strlen(existing_var_type[i])] = '\0';
						comma_seperator(&variableArray, &array, &numVariables, &array_counter, temp_type);
						memset(array_points, 0, sizeof(array_points));
						*name_seperated = 0;
						printf("program worked properly here. The names:\n");
						for(i = 0; i<numVariables; i++)
		        		{
		        			printf("%s\n", variableArray[i]->variable_name);
						}
					}
					semicolonPos = strchr(text_after_command[command_counter], ';');
					if(semicolonPos == NULL)
					{
						if(strcmp(text_after_command[command_counter], "") == 0)
						{
							printf("Buradan dondu 10\n    ");
							break;
						}
						else if(strcmp(text_after_command[command_counter], "") != 0)
						{
							printf("Error: Command sign(';') not found after last semicolon\n    ");
							break;
						}
					}
					result = semicolon_checker(text_after_command[command_counter - 1], &index, &semicolonPos, temp_indexs);
					if(temp_indexs[0] != 0)
		        	{
		        		for(i = 0; i < semicolon_counter; i++)
			        	{
			        		text_after_command[command_counter - 1][temp_indexs[i]] = ';';
						}
						semicolon_counter = 0;
						memset(temp_indexs, 0, sizeof(temp_indexs));
					}
					if(!result)
					{
						printf("Error: There must be ';' after pharantesis to end command\n    ");
			        	continue;
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
		        else
		        {
		        	printf("Error:Unknown command --> \"%s\"\n    ", token[command_counter]);
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
						printf("Error: Command sign(';') not found after last semicolon\n    ");
		        		break;
					}
		        }
		        break;
			}
		}
		continue;  
    }

    printf("\b\b\b\b}\n");
    printf("Command_counter = %d\n", command_counter);
    int random_num = 0;
    for (i = 0; i < printf_counter; i++)
    {
    	random_num = 1 + rand() % 12;
    	for (j = 0; changed_text[i][j] != '\0'; j++) 
		{
			printf("\033[32m%c\033[0m", changed_text[i][j]);
	        fflush(stdout); // Flush the output buffer to ensure immediate printing
	        sleep_ms(1 + rand() % random_num); // (0.001 - 0.012 seconds)
		}
    }
    
    for (i = 0; i < numVariables; i++) 
	{
        if (strcmp(variableArray[i]->type, "int ") == 0 || strcmp(variableArray[i]->type, "float ") == 0 || strcmp(variableArray[i]->type, "double ") == 0)
        {
        	dynamicSize = _msize(variableArray[i]->intPtr);
        	printf("\nSize of variableArray[%d]->intPtr at the end before freed: %zu bytes\n", i, dynamicSize);
            free(variableArray[i]->intPtr);
            //dynamicSize = _msize(variableArray[i]->intPtr);
            if(variableArray[i]->intPtr == NULL)
            {
            	printf("it's completely freed\n");
			}
			else
			{
				printf("Size of variableArray[%d]->intPtr at the end after freed: %zu bytes\n", i, dynamicSize);
			}
            dynamicSize = _msize(variableArray[i]->floatPtr);
        	printf("Size of variableArray[%d]->floatPtr at the end before freed: %zu bytes\n", i, dynamicSize);
            free(variableArray[i]->floatPtr);
            dynamicSize = _msize(variableArray[i]->floatPtr);
            printf("Size of variableArray[%d]->floatPtr at the end after freed: %zu bytes\n", i, dynamicSize);
            dynamicSize = _msize(variableArray[i]->doublePtr);
        	printf("Size of variableArray[%d]->doublePtr at the end before freed: %zu bytes\n", i, dynamicSize);
            free(variableArray[i]->doublePtr);
            dynamicSize = _msize(variableArray[i]->doublePtr);
            printf("Size of variableArray[%d]->doublePtr at the end after freed: %zu bytes\n", i, dynamicSize);
        }
        else if (strcmp(variableArray[i]->type, "char ") == 0 || strcmp(variableArray[i]->type, "string ") == 0)
        {
            free(variableArray[i]->charPtr);
            free(variableArray[i]->stringPtr);
        }
        free(variableArray[i]->array);
        free(variableArray[i]);
    }
    
    free(variableArray);
    
    for(i = 0; i < array_counter; i++)
    {
    	free(array[i]);
	}
	
	free(array);
	
	return 0;
}


void delete_comment(char *str)
{
    int i, j;
    bool inside_quotes = false;
    int parentheses_count = 0;

    for (i = 0, j = 0; str[i] != '\0'; i++)
    {
        if (str[i] == '\"'&& str[i - 1] != '\\')
        {
            inside_quotes = !inside_quotes; // Toggle the inside_quotes flag
            str[j++] = str[i]; // Keep the quotes in the modified string
        }
        else if (!inside_quotes)
        {
			if (str[i] == '/' && str[i + 1] == '*')
            {
                // Skip the comment block
                while (!(str[i] == '*' && str[i + 1] == '/'))
                {
                    i++;
                }
                i++; // Skip the '/' character
            }
            else
            {
                str[j++] = str[i]; // Keep all other characters in the modified string
            }
        }
        else
        {
            str[j++] = str[i]; // Keep characters inside quotes in the modified string
        }
    }

    str[j] = '\0'; // Null-terminate the modified string
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

int is_index_in_text(const char *str, ptrdiff_t index, int *parenthes_checker)
{
	int i = 0;
	int quote_count = 0;
	for(i = index; i >= 0; i--)
	{
		if(str[i] == '(' ) 
		{
            *parenthes_checker = 1; 
            break;
        } 
        else if(str[i] == '\"' && str[i - 1] != '\\')
        {
        	quote_count = 1;
        	*parenthes_checker = 1; 
            break;
		}
		else if (str[i]  == ')' && str[i - 1] == '\"' && str[i - 2] != '\\')
		{
			*parenthes_checker = 0; 
            return 0; 
        }
	}
	for(i = index; str[i] != '\0' ; i++)
	{
		if(str[i + 1] == '\0')
        {
        	*parenthes_checker = 0; 
        	return 0;
		}
		if(str[i] == '(' && str[i + 1] == '\"' ) 
		{
			*parenthes_checker = 0; 
        	return 0; 
        } 
        else if(str[i] == '\"' && str[i - 1] != '\\' )
        {
        	*parenthes_checker = 1; 
        	return 1;
		}
		else if (str[i] == ')' && quote_count == 0) 
		{
            *parenthes_checker = 1; 
        	return 1;
        }
	}
}

int semicolon_checker(char *str, ptrdiff_t *index, char **checker, int *temp_indexs)
{
	int parenthes_checker = 0;
	while(*checker != NULL)
	{
		*index = *checker - str;
		int result = is_index_in_text(str, *index, &parenthes_checker);
		if(result)
		{
			if(**checker == ';')
			{
				*checker = strchr((*checker + 1), ';');
			}
			else if(**checker == ',')
			{
				*checker = strchr((*checker + 1), ',');
			}
			str[*index] = '1';
			temp_indexs[semicolon_counter] = *index;
			semicolon_counter++;
			continue;
		}
		else if(!result)
		{
			break;
		}
	}
	if(parenthes_checker == 1)
	{
		return 0;
	}
	else if(parenthes_checker == 0)
	{
		return 1;
	}
}

void concatenate_words(char *str, int start, int end)
{
	int len = strlen(str);
	int i, j, parenthes_count = 0;
	
	if (start < 0 || end >= len || start > end) 
	{
		printf("len =%d\n", len);
		printf("Start=%d\n", start);
		printf("end=%d\n", end);
        printf("Error:Invalid range.\n");
        return;
    }
    
    int rangeLength = end - start + 1;
    char tempStr[len - end + 1];
    if(end < len - 1)
    {
    	for (i = end + 1, j = 0; i < len; i++, j++) 
		{
	        tempStr[j] = str[i];
	    }
	    tempStr[j] = '\0';
	}

	for (i = start, j = start; i<=end; i++, j++) 
	{
		if (str[i] == '(' && str[i + 1] == '\"')
        {
            parenthes_count = 1;
            str[j] = str[i];
            while (parenthes_count != 0 && i <= end)
            {
            	i++;
            	j++;
                str[j] = str[i];
                if (str[i] == '\"' && str[i + 1] == ')')
                {
                    parenthes_count = 0;
                    break;
                }
            }
        }
	    else if (str[i] != ' ' ) 
		{
	        str[j] = str[i];
	    }
	    else if (str[i] == ' ') 
		{
	        j--;
	    }
	}
	str[j] = '\0';
	if(end < len - 1)
	{
		strcat(str, tempStr);
	}
}

void insert(char* str1, const char* str2, int position) 
{
    int len1 = strlen(str1);
    int len2 = strlen(str2);

    if (position >= len1) 
	{
        strcat(str1, str2);
    }
    else if (position <= 0) // If the position is 0 or negative, insert str2 at the beginning of str1.
	{
        memmove(str1 + len2, str1, len1 + 1); // +1 to include the null terminator
        strncpy(str1, str2, len2);
    }
    else // Insert str2 into str1 at the specified position.
	{
        memmove(str1 + position + len2, str1 + position, len1 - position + 1); // +1 to include the null terminator
        strncpy(str1 + position, str2, len2);
    }
}

int is_in_struct(const VariableType** variableArray, const char* str, int numVariable)
{
	if (variableArray == NULL || str == NULL || numVariable == 0) 
	{
        return 0; // Handle edge cases
    }
	size_t i = 0;
    for (i = 0; i < numVariable; i++)
    {
        if (strcmp(variableArray[i]->variable_name, str) == 0 || strcmp(variableArray[i]->array->array_name, str) == 0)
        {
            return 1;
        }
    }
    return 0;
}

void mistake_specifier_in_comma_seperator(const int * mistake_places, int mistake_counter, int function_part)
{
	int i = 0, j = 0;
	if(mistake_counter == 1)
	{
		if(function_part == 1)
		{
			printf("Warning: value of ");
		}
		else if(function_part == 2)
		{
			printf("Warning: pointed variable of ");
		}
	}
	else if(mistake_counter > 1)
	{
		if(function_part == 1)
		{
			printf("Warning: values of ");
		}
		else if(function_part == 2)
		{
			printf("Warning: pointed variables of ");
		}
	}
	for(i = 0, j = 0; i < 100; i++)
	{
		if(mistake_places[i] == 0)
		{
			continue;
		}
		if(mistake_counter == 1)
		{
			if(function_part == 1)
			{
				printf("%d. variable is not numeric\n",mistake_places[i]);
				break;
			}
			else if(function_part == 2)
			{
				printf("%d. pointer is numeric\n",mistake_places[i]);
				break;
			}
		}
		else if(mistake_counter > 1)
		{
			if(mistake_places[i] != 0 && j < mistake_counter - 1)
			{
				if(j == mistake_counter - 2)
				{
					printf("%d and ",mistake_places[i]);	
					j++;
					continue;
				}
				printf("%d,",mistake_places[i]);
				j++;
				continue;
			}
			else if(mistake_places[i] != 0 && j == mistake_counter - 1)
			{
				if(function_part == 1)
				{
					printf("%d. variables are not numeric\n",mistake_places[i]);
					break;
				}
				else if(function_part == 2)
				{
					printf("%d. pointers are numeric\n",mistake_places[i]);
					break;
				}
			}	
		}
	}
}

void comma_seperator(VariableType*** variableArray, Array*** array, int *numVariable, int* array_counter, const char temp_type[])
{
	int i,result, C = 0;
	int mistake_counter = 0;
	char *commaPos;
	char *square_bracket_pos;
	char *second_square_bracket_pos;
	ptrdiff_t index;
	ptrdiff_t previous_index;
	size_t newSize;
	char temp_str[200];
	strcpy(temp_str, (*variableArray)[*numVariable]->variable_name);
	char second_temp_str[200];
	strcpy(second_temp_str, (*variableArray)[*numVariable]->variable_name);
	int temp_indexs[25] = {0};
	int index_of_array_points = 0;
    char *comma_seperator[100];
    char *comma_seperator_in_array[100];
    int temp_numVariable = *numVariable;
    if(*name_seperated == 0)
    {
    	commaPos = strchr(temp_str, ',');
    	index = commaPos - temp_str;			
		semicolon_checker(temp_str, &index, &commaPos, temp_indexs);
		previous_index = index;	
		if(commaPos != NULL)
		{
			commaPos = strchr(commaPos + 1, ',');
		}
    	comma_seperator[C] = strtok(temp_str, ",");
		while(C<100) //check every comma sequence
	    {
	        if(comma_seperator[C] != NULL)
	        {
	        	if(temp_indexs[0] != 0)
	        	{
	        		for(i = 0; i < semicolon_counter; i++)
		        	{
		        		comma_seperator[C][temp_indexs[i]] = ',';
					}
					semicolon_counter = 0;
					memset(temp_indexs, 0, sizeof(temp_indexs));
				}
				if(is_in_struct((const VariableType**) *variableArray, comma_seperator[C], *numVariable))
				{
					printf("\nError:Name of the %d.Variable(%s) is already given before\n    ", *numVariable - temp_numVariable + 1, comma_seperator[C]);
					(*numVariable) = temp_numVariable;
					mistake_counter = 1;
					break;
				}
	            strcpy((*variableArray)[*numVariable]->variable_name, comma_seperator[C]);
	            printf("(*variableArray)[%d]->variable_name = %s\n", *numVariable, (*variableArray)[*numVariable]->variable_name);
	            if(C == 0)
	            {
	            	var_name_start += strlen(comma_seperator[C]); 
				}
				else if(C > 0)
				{
					var_name_start += strlen(comma_seperator[C]) + 1; 
				}
				square_bracket_pos = strchr((*variableArray)[*numVariable]->variable_name, '[');
				if(square_bracket_pos != NULL)
				{
					second_square_bracket_pos = strchr(square_bracket_pos + 1, ']');
				}
				if(square_bracket_pos != NULL && second_square_bracket_pos != NULL)
				{ 
					int array_size_length = second_square_bracket_pos - square_bracket_pos - 1;
					char temp_array_size[7] = "";
					char temp_array_name[100] = "";
					strncpy(temp_array_size, square_bracket_pos + 1, array_size_length);
					temp_array_size[strlen(temp_array_size)] = '\0';
					concatenate_words(temp_array_size, 0, strlen(temp_array_size) - 1);
					printf("temp_array_size %s\n", temp_array_size);
					strncpy(temp_array_name, (*variableArray)[*numVariable]->variable_name, square_bracket_pos - (*variableArray)[*numVariable]->variable_name);
					temp_array_name[strlen(temp_array_name)] = '\0';
					if(is_in_struct((const VariableType**) *variableArray, temp_array_name, *numVariable))
					{
						printf("\nError:Name of the %d.Variable(%s) is already given before\n    ", *numVariable - temp_numVariable + 1, temp_array_name);
						(*numVariable) = temp_numVariable;
						mistake_counter = 1;
						break;
					}
					printf("temp_array_name %s\n    ", temp_array_name);
					if(isNumeric(temp_array_size))
					{
						array_points[index_of_array_points] = C;
						index_of_array_points++;
						(*array)[*array_counter] = (Array*)malloc(sizeof(Array));
						(*array)[*array_counter]->array_size = atoi(temp_array_size);
						strcpy((*array)[*array_counter]->array_name, temp_array_name);
						(*variableArray)[*numVariable]->array = (*array)[*array_counter];
						(*array_counter)++;
						newSize = (*array_counter + 1);
						Array** temp = (Array**)realloc(*array, newSize * sizeof(Array*));
						*array = temp;
						
						for(i = 0; i < atoi(temp_array_size); i++)
						{
							sprintf((*variableArray)[*numVariable]->variable_name, "%s[%d]", temp_array_name, i);
							printf("(*variableArray)[*numVariable]->variable_name = %s\n    ", (*variableArray)[*numVariable]->variable_name);
							(*numVariable)++;
							newSize = (*numVariable + 1);
							VariableType** temp = (VariableType**)realloc(*variableArray, newSize * sizeof(VariableType*));
    						*variableArray = temp;
							dynamicSize = _msize(*variableArray);
							if (*variableArray == NULL) 
							{
						        printf("3-Memory reallocation failed.\n    ");
						        exit(1);
						    }
						    else
						    {
						    	printf("3-Size of variableArray: %zu bytes\n    ", dynamicSize);
							}
							(*variableArray)[*numVariable] = (VariableType*)malloc(sizeof(VariableType));
							(*variableArray)[*numVariable]->array = (Array*)malloc(sizeof(Array));	
							dynamicSize = _msize((*variableArray)[*numVariable]);
							if ((*variableArray)[*numVariable] == NULL) 
							{
						        printf("4-Memory reallocation failed.\n    ");
						        exit(1);
						    }
						    else
						    {
						    	printf("4-Size of variableArray[%d]: %zu bytes\n    ", *numVariable, dynamicSize);
							}
							if(i < atoi(temp_array_size) - 1)
							{
								strcpy((*variableArray)[*numVariable]->type, temp_type);
								(*variableArray)[*numVariable]->array = (*array)[*array_counter - 1];
							}
						}
					}
					else if(!isNumeric(temp_array_size))
					{
						printf("Error: Array size is not numeric.\n    ");
						(*numVariable) = temp_numVariable;
						mistake_counter = 1;
						break;
					}
				}
				else if(square_bracket_pos == NULL || second_square_bracket_pos == NULL)
				{
					strcpy((*variableArray)[*numVariable]->array->array_name, "");
	                (*variableArray)[*numVariable]->array->array_size = 1;
					(*numVariable)++;
					newSize = (*numVariable + 1);
					VariableType** temp = (VariableType**)realloc(*variableArray, newSize * sizeof( VariableType*));
    				*variableArray = temp;
					dynamicSize = _msize(*variableArray);
					if (*variableArray == NULL) 
					{
				        printf("5-Memory reallocation failed.\n    ");
				        exit(1);
				    }
				    else
				    {
				    	printf("5-Size of variableArray: %zu bytes\n    ", dynamicSize);
					}
					(*variableArray)[*numVariable] = (VariableType*)malloc(sizeof(VariableType));
					(*variableArray)[*numVariable]->array = (Array*)malloc(sizeof(Array));	
					dynamicSize = _msize((*variableArray)[*numVariable]);
					if ((*variableArray)[*numVariable] == NULL) 
					{
				        printf("6-Memory reallocation failed.\n    ");
				        exit(1);
				    }
				    else
				    {
				    	printf("6-Size of variableArray[%d]: %zu bytes\n    ", *numVariable, dynamicSize);
					}
				}
				printf("Buraya geldi\n    ");
	            if((*var_name_start) == ',' && *(var_name_start + 1) != ',' && *(var_name_start + 1) != '=' && *(var_name_start + 1) != ';')
	            {
		            strcpy((*variableArray)[*numVariable]->type, temp_type);
		            strcpy((*variableArray)[*numVariable]->array->array_name, "");
	                (*variableArray)[*numVariable]->array->array_size = 1;
		            C++;
		            strcpy(temp_str, second_temp_str + previous_index + 1);
		            commaPos = strchr(temp_str, ',');
	            	result = semicolon_checker(temp_str, &index, &commaPos, temp_indexs);
	            	if(result)
	            	{
	            		previous_index += index + 1;	
						if(commaPos != NULL)
						{
							commaPos = strchr(commaPos + 1, ',');
						}
					}
	            	else if(!result)
		            {
		            	strcat(temp_str, ",");
					}
		            comma_seperator[C] = strtok(temp_str, ",");
		            continue;
				}
				else if((*var_name_start) == '=' || (*var_name_start) == ';' )
				{
					seperated_value_capacity = *numVariable;
					break;
				}
				else if(*(var_name_start + 1) == ',' || *(var_name_start + 1) == '=' || *(var_name_start + 1) == ';')
				{
					printf("\nError:%d.Variable name missing\n", (*numVariable) - temp_numVariable + 1);
					(*numVariable) = temp_numVariable;
					mistake_counter = 1;
					break;
				}
	        }
	        else if(comma_seperator[C] == NULL)
	        {
				printf("Error:There is no variable name before first comma\n");
				(*numVariable) = temp_numVariable;
				memset((*variableArray)[*numVariable]->variable_name, 0, sizeof((*variableArray)[*numVariable]->variable_name));
				mistake_counter = 1;
	            break;
	        }
	    }
	    if(mistake_counter == 0)
	    {
	    	*name_seperated = 1;
		}
	}
	else if(*name_seperated == 1)
	{
		printf("YAAASS2\n");
		int i, j, restart = 0;
		for(i = 0; i < 10; i++)
		{
			if( strcmp(temp_type, existing_var_type[i]) == 0 && (i >= 0 && i <= 2))
			{
				int mistake_places[100] = {0};
				mistake_counter = 0;
				strcpy(temp_str, (*variableArray)[*numVariable]->temp_intValue);
				strcpy(second_temp_str, (*variableArray)[*numVariable]->temp_intValue);
				commaPos = strchr(temp_str, ',');
		    	index = commaPos - temp_str;			
				semicolon_checker(temp_str, &index, &commaPos, temp_indexs);
				previous_index = index;	
				if(commaPos != NULL)
				{
					commaPos = strchr(commaPos + 1, ',');
				}
				comma_seperator[C] = strtok(temp_str, ",");
				while(C<100) 
			    {
			        if(comma_seperator[C] != NULL)
			        {
			        	if(*numVariable == seperated_value_capacity)
			        	{
			        		break;
						}
			        	if(temp_indexs[0] != 0)
			        	{
			        		for(i = 0; i < semicolon_counter; i++)
				        	{
				        		comma_seperator[C][temp_indexs[i]] = ',';
							}
							semicolon_counter = 0;
							memset(temp_indexs, 0, sizeof(temp_indexs));
						}
			        	if(isNumeric(comma_seperator[C]))
						{
							(*variableArray)[*numVariable]->doubleValue = strtod(comma_seperator[C], NULL);
							(*variableArray)[*numVariable]->floatValue = atof(comma_seperator[C]);
							(*variableArray)[*numVariable]->intValue = atoi(comma_seperator[C]);
							(*numVariable)++;
						}
						else if(!isNumeric(comma_seperator[C]))
						{
							if(comma_seperator[C][0] == '(' && comma_seperator[C][1] == '\"' && comma_seperator[C][strlen(comma_seperator[C]) - 2] == '\"' && comma_seperator[C][strlen(comma_seperator[C]) - 1] == ')')
							{
								printf("Total elements: %d\n", dynamicSize/8 - 1);
								char temp_of_comma_seperator[100];
								memmove(temp_of_comma_seperator, comma_seperator[C] + 2, strlen(comma_seperator[C]) - 4);
								temp_of_comma_seperator[strlen(comma_seperator[C]) - 4] = '\0';
								concatenate_words(temp_of_comma_seperator, 0, strlen(temp_of_comma_seperator) - 1);
								printf("temp_of_comma_seperator = %s\n", temp_of_comma_seperator);
								for(i = 0; i < C + 1; i++)
								{
									printf("array_points[i] = %d\n", array_points[i]);
									if( C == array_points[i])
									{
										int k;
										double default_value = 0;
										int remaining_array_size = (*variableArray)[*numVariable]->array->array_size;
										char temp_of_default[100];
											
										if(strchr(temp_of_comma_seperator, ',') != NULL)
										{
											printf("Virgul var\n");
											j = 0;
											comma_seperator_in_array[j] = strtok(temp_of_comma_seperator, ",");
											while(comma_seperator_in_array[j] != NULL)
											{
												printf("comma_seperator_in_array[j] = %s\n", comma_seperator_in_array[j]);
												if(isNumeric(comma_seperator_in_array[j]))
												{
													(*variableArray)[*numVariable]->doubleValue = strtod(comma_seperator_in_array[j], NULL);
													(*variableArray)[*numVariable]->floatValue = atof(comma_seperator_in_array[j]);
													(*variableArray)[*numVariable]->intValue = atoi(comma_seperator_in_array[j]);
												}
												else if(!isNumeric(comma_seperator_in_array[j]))
												{
													if(comma_seperator_in_array[j][0] == '{' && comma_seperator_in_array[j][strlen(comma_seperator_in_array[j]) - 1] == '}')
													{
														memmove(temp_of_default, comma_seperator_in_array[j] + 1, strlen(comma_seperator_in_array[j]) - 2);
														temp_of_default[strlen(comma_seperator_in_array[j]) - 2] = '\0';
														if(temp_of_default[0] == '+' || temp_of_default[0] == '-' || temp_of_default[0] == '*' || temp_of_default[0] == '/' || temp_of_default[0] == '%' || temp_of_default[0] == '^')
														{
															if(isNumeric(temp_of_default + 1))
															{
																(*variableArray)[*numVariable]->doubleValue = strtod(temp_of_default + 1, NULL);
																(*variableArray)[*numVariable]->floatValue = atof(temp_of_default + 1);
																(*variableArray)[*numVariable]->intValue = atoi(temp_of_default + 1);
																if(temp_of_default[0] == '+')
																{
																	default_value += (*variableArray)[*numVariable]->intValue;
																}
																else if(temp_of_default[0] == '-')
																{
																	default_value -= (*variableArray)[*numVariable]->intValue;
																}
																else if(temp_of_default[0] == '*')
																{
																	default_value *= (*variableArray)[*numVariable]->intValue;
																}
																else if(temp_of_default[0] == '/')
																{
																	default_value /= (*variableArray)[*numVariable]->intValue;
																}
																else if(temp_of_default[0] == '%')
																{
																	default_value = (int) default_value % (*variableArray)[*numVariable]->intValue;
																}
																else if(temp_of_default[0] == '^')
																{
																	default_value = pow(default_value, (*variableArray)[*numVariable]->intValue);
																}
															}
															else if(!isNumeric(temp_of_default + 1))
															{
																(*variableArray)[*numVariable]->doubleValue = 0.0;
																(*variableArray)[*numVariable]->floatValue = 0.0f;
																(*variableArray)[*numVariable]->intValue = 0;
																mistake_places[*numVariable - temp_numVariable] = *numVariable - temp_numVariable + 1;
																mistake_counter ++;
															}
														}
														else
														{
															if(isNumeric(temp_of_default))
															{
																(*variableArray)[*numVariable]->doubleValue = strtod(temp_of_default, NULL);
																(*variableArray)[*numVariable]->floatValue = atof(temp_of_default);
																(*variableArray)[*numVariable]->intValue = atoi(temp_of_default);
																default_value = (*variableArray)[*numVariable]->intValue;
															}
															else if(!isNumeric(temp_of_default))
															{
																(*variableArray)[*numVariable]->doubleValue = 0.0;
																(*variableArray)[*numVariable]->floatValue = 0.0f;
																(*variableArray)[*numVariable]->intValue = 0;
																mistake_places[*numVariable - temp_numVariable] = *numVariable - temp_numVariable + 1;
																mistake_counter ++;
															}
														}
													}
													else
													{
														(*variableArray)[*numVariable]->doubleValue = 0.0;
														(*variableArray)[*numVariable]->floatValue = 0.0f;
														(*variableArray)[*numVariable]->intValue = 0;
														mistake_places[*numVariable - temp_numVariable] = *numVariable - temp_numVariable + 1;
														mistake_counter ++;
													}
												}
												(*numVariable)++;
												j++;
												comma_seperator_in_array[j] = strtok(NULL, ",");
											}
											
											for(k = 0; k < remaining_array_size - j; k++)
											{
												(*variableArray)[*numVariable]->doubleValue = default_value;
												(*variableArray)[*numVariable]->floatValue = default_value;
												(*variableArray)[*numVariable]->intValue = default_value;
												(*numVariable)++;
											}
										}
										else if(strchr(temp_of_comma_seperator, ',') == NULL)
										{
											printf("Virgul yok\n");
											if(isNumeric(temp_of_comma_seperator))
											{
												(*variableArray)[*numVariable]->doubleValue = strtod(temp_of_comma_seperator, NULL);
												(*variableArray)[*numVariable]->floatValue = atof(temp_of_comma_seperator);
												(*variableArray)[*numVariable]->intValue = atoi(temp_of_comma_seperator);
											}
											else if(!isNumeric(temp_of_comma_seperator))
											{
												if(temp_of_comma_seperator[0] == '{' && temp_of_comma_seperator[strlen(temp_of_comma_seperator) - 1] == '}')
												{
													memmove(temp_of_default, temp_of_comma_seperator + 1, strlen(temp_of_comma_seperator) - 2);
													temp_of_default[strlen(temp_of_comma_seperator) - 2] = '\0';
													if(temp_of_default[0] == '+' || temp_of_default[0] == '-' || temp_of_default[0] == '*' || temp_of_default[0] == '/' || temp_of_default[0] == '%' || temp_of_default[0] == '^')
													{
														if(isNumeric(temp_of_default + 1))
														{
															(*variableArray)[*numVariable]->doubleValue = strtod(temp_of_default + 1, NULL);
															(*variableArray)[*numVariable]->floatValue = atof(temp_of_default + 1);
															(*variableArray)[*numVariable]->intValue = atoi(temp_of_default + 1);
															if(temp_of_default[0] == '+')
															{
																default_value += (*variableArray)[*numVariable]->intValue;
															}
															else if(temp_of_default[0] == '-')
															{
																default_value -= (*variableArray)[*numVariable]->intValue;
															}
															else if(temp_of_default[0] == '*')
															{
																default_value *= (*variableArray)[*numVariable]->intValue;
															}
															else if(temp_of_default[0] == '/')
															{
																default_value /= (*variableArray)[*numVariable]->intValue;
															}
															else if(temp_of_default[0] == '%')
															{
																default_value = (int) default_value % (*variableArray)[*numVariable]->intValue;
															}
															else if(temp_of_default[0] == '^')
															{
																default_value = pow(default_value, (*variableArray)[*numVariable]->intValue);
															}
														}
														else if(!isNumeric(temp_of_default + 1))
														{
															(*variableArray)[*numVariable]->doubleValue = 0.0;
															(*variableArray)[*numVariable]->floatValue = 0.0f;
															(*variableArray)[*numVariable]->intValue = 0;
															mistake_places[C] = C + 1;
															mistake_counter ++;
														}
													}
													else
													{
														if(isNumeric(temp_of_default))
														{
															(*variableArray)[*numVariable]->doubleValue = strtod(temp_of_default, NULL);
															(*variableArray)[*numVariable]->floatValue = atof(temp_of_default);
															(*variableArray)[*numVariable]->intValue = atoi(temp_of_default);
															default_value = (*variableArray)[*numVariable]->intValue;
														}
														else if(!isNumeric(temp_of_default))
														{
															(*variableArray)[*numVariable]->doubleValue = 0.0;
															(*variableArray)[*numVariable]->floatValue = 0.0f;
															(*variableArray)[*numVariable]->intValue = 0;
															mistake_places[C] = C + 1;
															mistake_counter ++;
														}
													}
												}
												else
												{
													(*variableArray)[*numVariable]->doubleValue = 0.0;
													(*variableArray)[*numVariable]->floatValue = 0.0f;
													(*variableArray)[*numVariable]->intValue = 0;
													mistake_places[C] = C + 1;
													mistake_counter ++;
												}
											}
											(*numVariable)++;
											
											for(k = 0; k < remaining_array_size - 1; k++)
											{
												(*variableArray)[*numVariable]->doubleValue = default_value;
												(*variableArray)[*numVariable]->floatValue = default_value;
												(*variableArray)[*numVariable]->intValue = default_value;
												(*numVariable)++;
											}
										}
										break;
									}
									else if( C != array_points[i] && i ==  C)
									{
										printf("Warning: %d. value %s is not belong to array.\n",  C + 1, comma_seperator[C]);
										(*numVariable)++;
									}
								}
							}
							else
							{
								mistake_places[C] = C + 1;
								mistake_counter ++;
								(*numVariable)++;
							}
						}
			            if(C == 0)
			            {
			            	var_name_start += strlen(comma_seperator[C]); 
						}
						else if(C > 0)
						{
							var_name_start += strlen(comma_seperator[C]) + 1; 
						}
			            if((*var_name_start) == ',' && *(var_name_start + 1) != ',' && *(var_name_start + 1) != ';')
			            {
				            C++;
				            strcpy(temp_str, second_temp_str + previous_index + 1);
				            commaPos = strchr(temp_str, ',');
			            	result = semicolon_checker(temp_str, &index, &commaPos, temp_indexs);
			            	if(result)
			            	{
			            		previous_index += index + 1;	
								if(commaPos != NULL)
								{
									commaPos = strchr(commaPos + 1, ',');
								}
							}
			            	else if(!result)
				            {
				            	strcat(temp_str, ",");
							}
				            comma_seperator[C] = strtok(temp_str, ",");
				            printf("comma_seperator[C] = %s\n", comma_seperator[C]);
				            continue;
						}
						else if((*var_name_start) == ';')
						{
							printf("\nReturned after last comma','\n");
							break;
						}
						else if(*(var_name_start + 1) == ',' || *(var_name_start + 1) == ';')
						{
							printf("\nError:%d.Variable value missing\n", (*numVariable) - temp_numVariable + 1);
							(*numVariable) = temp_numVariable;
							restart = 1;
							break;
						}
			        }
			        else if(comma_seperator[C] == NULL)
			        {
			            printf("Error:There is no variable value before first comma\n");
						(*numVariable) = temp_numVariable;
						(*variableArray)[*numVariable]->intValue = 0;
						(*variableArray)[*numVariable]->floatValue = 0.0f;
						(*variableArray)[*numVariable]->doubleValue = 0.0;
						restart = 1;
			            break;
			        }
			    }
			    if(mistake_counter == 0)
			    {
			    	break;
				}
				else if(mistake_counter > 0)
				{
					mistake_specifier_in_comma_seperator(mistake_places, mistake_counter, 1);
					break;
				}
			}
			else if( strcmp(temp_type, existing_var_type[i]) == 0 && (i >= 3 && i <= 4))
			{
				strcpy(temp_str, (*variableArray)[*numVariable]->stringValue);
				strcpy(second_temp_str, (*variableArray)[*numVariable]->stringValue);
				commaPos = strchr(temp_str, ',');
		    	index = commaPos - temp_str;			
				semicolon_checker(temp_str, &index, &commaPos, temp_indexs);
				previous_index = index;	
				if(commaPos != NULL)
				{
					commaPos = strchr(commaPos + 1, ',');
				}
				comma_seperator[C] = strtok(temp_str, ",");
				while(C<100) //check every comma sequence
			    {
			        if(comma_seperator[C] != NULL)
			        {
			        	if(*numVariable == seperated_value_capacity)
			        	{
			        		break;
						}
			        	if(temp_indexs[0] != 0)
			        	{
			        		for(i = 0; i < semicolon_counter; i++)
				        	{
				        		comma_seperator[C][temp_indexs[i]] = ',';
							}
							semicolon_counter = 0;
							memset(temp_indexs, 0, sizeof(temp_indexs));
						}
						if(strcmp(temp_type, "char ") == 0)
						{
							/*char temp_of_comma_seperator[100];
							for(i = 0; i < C + 1; i++)
							{
								printf("array_points[i] = %d\n", array_points[i]);
								if( C == array_points[i])
								{
									int k;
									int remaining_array_size = (*variableArray)[*numVariable]->array->array_size;
									char default_value = '\0';
									printf("Total elements: %d\n", dynamicSize/8 - 1);
									
									if(comma_seperator[C][0] == '(' && comma_seperator[C][1] == '\"' && comma_seperator[C][strlen(comma_seperator[C]) - 2] == '\"' && comma_seperator[C][strlen(comma_seperator[C]) - 1] == ')')
									{
										memmove(temp_of_comma_seperator, comma_seperator[C] + 2, strlen(comma_seperator[C]) - 4);
										temp_of_comma_seperator[strlen(comma_seperator[C]) - 4] = '\0';
										printf("temp_of_comma_seperator = %s\n", temp_of_comma_seperator);
									}
									else
									{
										strcpy(temp_of_comma_seperator, comma_seperator[C]);
										printf("temp_of_comma_seperator = %s\n", temp_of_comma_seperator);
									}
									
									if(strlen(temp_of_comma_seperator) <= remaining_array_size)
									{
										for(k = 0; k < )
										{
											(*variableArray)[*numVariable]->charValue = strtod(temp_of_default + 1, NULL);
											(*numVariable)++;
										}
									}
									
									for(k = 0; k < remaining_array_size - 1; k++)
									{
										(*variableArray)[*numVariable]->doubleValue = default_value;
										(*variableArray)[*numVariable]->floatValue = default_value;
										(*variableArray)[*numVariable]->intValue = default_value;
										(*numVariable)++;
									}
									break;
								}
								else if( C != array_points[i] && i ==  C)
								{
									(*variableArray)[*numVariable]->charValue = temp_of_comma_seperator[0];
									strcpy((*variableArray)[*numVariable]->stringValue, temp_of_comma_seperator);
									(*numVariable)++;
								}
							}
							if(comma_seperator[C][0] == '(' && comma_seperator[C][1] == '\"' && comma_seperator[C][strlen(comma_seperator[C]) - 2] == '\"' && comma_seperator[C][strlen(comma_seperator[C]) - 1] == ')')
							{
								printf("Total elements: %d\n", dynamicSize/8 - 1);
								memmove(temp_of_comma_seperator, comma_seperator[C] + 2, strlen(comma_seperator[C]) - 4);
								temp_of_comma_seperator[strlen(comma_seperator[C]) - 4] = '\0';
								printf("temp_of_comma_seperator = %s\n", temp_of_comma_seperator);
							}
							else
							{
								strcpy(temp_of_comma_seperator, comma_seperator[C]);
								
								(*numVariable)++;
							}*/
							strcpy((*variableArray)[*numVariable]->stringValue, comma_seperator[C]);
						}
       	 				else if (strcmp(temp_type, "string ") == 0)
       	 				{
       	 					
       	 					strcpy((*variableArray)[*numVariable]->stringValue, comma_seperator[C]);
						}
			            printf("\n(*variableArray)[*numVariable]->stringValue = %s\n", (*variableArray)[*numVariable]->stringValue);
			            (*numVariable)++;
			            if(C == 0)
			            {
			            	var_name_start += strlen(comma_seperator[C]); 
						}
						else if(C > 0)
						{
							var_name_start += strlen(comma_seperator[C]) + 1; 
						}
			            if((*var_name_start) == ',' && *(var_name_start + 1) != ',' && *(var_name_start + 1) != ';')
			            {
				            C++;
				            strcpy(temp_str, second_temp_str + previous_index + 1);
				            commaPos = strchr(temp_str, ',');
			            	result = semicolon_checker(temp_str, &index, &commaPos, temp_indexs);
			            	if(result)
			            	{
			            		previous_index += index + 1;	
								if(commaPos != NULL)
								{
									commaPos = strchr(commaPos + 1, ',');
								}
							}
			            	else if(!result)
				            {
				            	strcat(temp_str, ",");
							}
				            comma_seperator[C] = strtok(temp_str, ",");
				            printf("comma_seperator[C] = %s\n", comma_seperator[C]);
				            continue;
						}
						else if((*var_name_start) == ';')
						{
							printf("\nReturned after last comma','\n");
							break;
						}
						else if(*(var_name_start + 1) == ',' || *(var_name_start + 1) == ';')
						{
							printf("\nError:%d.Variable value missing\n", (*numVariable) - temp_numVariable + 1);
							(*numVariable) = temp_numVariable;
							restart = 1;
							break;
						}
			        }
			        else if(comma_seperator[C] == NULL)
			        {
			            printf("Error:There is no variable value before first comma\n");
						(*numVariable) = temp_numVariable;
						memset((*variableArray)[*numVariable]->stringValue, 0, sizeof((*variableArray)[*numVariable]->stringValue));
						restart = 1;
			            break;
			        }
			    }
			    break;
			}
			else if( strcmp(temp_type, existing_var_type[i]) == 0 && (i >= 5 && i <= 7))
			{
				int mistake_places[100] = {0};
				mistake_counter = 0;
				strcpy(temp_str, (*variableArray)[*numVariable]->temp_intValue);
				strcpy(second_temp_str, (*variableArray)[*numVariable]->temp_intValue);
				commaPos = strchr(temp_str, ',');
		    	index = commaPos - temp_str;			
				semicolon_checker(temp_str, &index, &commaPos, temp_indexs);
				previous_index = index;	
				if(commaPos != NULL)
				{
					commaPos = strchr(commaPos + 1, ',');
				}
				comma_seperator[C] = strtok(temp_str, ",");
				while(C<100) 
			    {
			        if(comma_seperator[C] != NULL)
			        {
			        	if(*numVariable == seperated_value_capacity)
			        	{
			        		break;
						}
			        	if(temp_indexs[0] != 0)
			        	{
			        		for(i = 0; i < semicolon_counter; i++)
				        	{
				        		comma_seperator[C][temp_indexs[i]] = ',';
							}
							semicolon_counter = 0;
							memset(temp_indexs, 0, sizeof(temp_indexs));
						}
			        	if(isNumeric(comma_seperator[C]))
						{
							mistake_places[C] = C + 1;
							mistake_counter ++;
						}
						else if(!isNumeric(comma_seperator[C]))
						{
							printf("\n*numVariable = %d\n", *numVariable);
							for(j = 0; j < *numVariable; j++)
						    {
						    	if(strcmp((*variableArray)[j]->variable_name, comma_seperator[C]) == 0)
						    	{
						    		(*variableArray)[*numVariable]->intPtr = (int*)malloc(sizeof(int));
						    		(*variableArray)[*numVariable]->floatPtr = (float*)malloc(sizeof(float));
						    		(*variableArray)[*numVariable]->doublePtr = (double*)malloc(sizeof(double));
						    		if(strcmp((*variableArray)[j]->type, existing_var_type[0]) == 0 || strcmp((*variableArray)[j]->type, existing_var_type[1]) == 0 ||
						    		strcmp((*variableArray)[j]->type, existing_var_type[2]) == 0)
						    		{
										(*variableArray)[*numVariable]->intPtr = &((*variableArray)[j]->intValue);
										(*variableArray)[*numVariable]->intValue = (*variableArray)[j]->intValue;
										(*variableArray)[*numVariable]->floatPtr = &((*variableArray)[j]->floatValue);
										(*variableArray)[*numVariable]->floatValue = (*variableArray)[j]->floatValue;
										(*variableArray)[*numVariable]->doublePtr = &((*variableArray)[j]->doubleValue);
										(*variableArray)[*numVariable]->doubleValue = (*variableArray)[j]->doubleValue;
										break;
									}
									else if(strcmp((*variableArray)[j]->type, existing_var_type[5]) == 0 || strcmp((*variableArray)[j]->type, existing_var_type[6]) == 0 ||
						    		strcmp((*variableArray)[j]->type, existing_var_type[7]) == 0)
									{
										(*variableArray)[*numVariable]->intPtr = &(*((*variableArray)[j]->intPtr));
										(*variableArray)[*numVariable]->intValue = (*variableArray)[j]->intValue;
										(*variableArray)[*numVariable]->floatPtr = &(*((*variableArray)[j]->floatPtr));
										(*variableArray)[*numVariable]->floatValue = (*variableArray)[j]->floatValue;
										(*variableArray)[*numVariable]->doublePtr = &(*((*variableArray)[j]->doublePtr));
										(*variableArray)[*numVariable]->doubleValue = (*variableArray)[j]->doubleValue;
										break;
									}
									else 
									{
										printf("Error: The variable(%s) tried to be pointed by the pointer(%s) is not numeric(%s).\n", (*variableArray)[j]->variable_name, (*variableArray)[*numVariable]->variable_name, (*variableArray)[j]->type);
										(*numVariable) = temp_numVariable;
										restart = 1;
								   		break;
									}
								}
								else if(strcmp((*variableArray)[j]->variable_name, comma_seperator[C]) != 0 && j == *numVariable - 1)
								{
									printf("Error: There is no variable named \"%s\".\n", comma_seperator[C]);
									(*numVariable) = temp_numVariable;
									restart = 1;
								   	break;
								}
							}
						}
			            (*numVariable)++;
			            if(C == 0)
			            {
			            	var_name_start += strlen(comma_seperator[C]); 
						}
						else if(C > 0)
						{
							var_name_start += strlen(comma_seperator[C]) + 1; 
						}
			            if((*var_name_start) == ',' && *(var_name_start + 1) != ',' && *(var_name_start + 1) != ';')
			            {
				            C++;
				            strcpy(temp_str, second_temp_str + previous_index + 1);
				            commaPos = strchr(temp_str, ',');
			            	result = semicolon_checker(temp_str, &index, &commaPos, temp_indexs);
			            	if(result)
			            	{
			            		previous_index += index + 1;	
								if(commaPos != NULL)
								{
									commaPos = strchr(commaPos + 1, ',');
								}
							}
			            	else if(!result)
				            {
				            	strcat(temp_str, ",");
							}
				            comma_seperator[C] = strtok(temp_str, ",");
				            printf("comma_seperator[C] = %s\n", comma_seperator[C]);
				            continue;
						}
						else if((*var_name_start) == ';')
						{
							printf("\nReturned after last comma','\n");
							break;
						}
						else if(*(var_name_start + 1) == ',' || *(var_name_start + 1) == ';')
						{
							printf("\nError:%d.Variable name missing\n", (*numVariable) - temp_numVariable + 1);
							(*numVariable) = temp_numVariable;
							restart = 1;
							break;
						}
			        }
			        else if(comma_seperator[C] == NULL)
			        {
			            printf("Error:There is no variable name before first comma\n");
						(*numVariable) = temp_numVariable;
						(*variableArray)[*numVariable]->intValue = 0;
						(*variableArray)[*numVariable]->floatValue = 0.0f;
						(*variableArray)[*numVariable]->doubleValue = 0.0;
						restart = 1;
			            break;
			        }
			    }
			    if(mistake_counter == 0)
			    {
			    	break;
				}
				else if(mistake_counter > 0)
				{
					mistake_specifier_in_comma_seperator(mistake_places, mistake_counter, 2);
					break;
				}
			}
			/*else if( strcmp(temp_type, existing_var_type[i]) == 0 && (i >= 8 && i <= 9))
			{
				int mistake_places[100] = {0};
				mistake_counter = 0;
				strcpy(temp_str, (*variableArray)[*numVariable]->stringValue);
				strcpy(second_temp_str, (*variableArray)[*numVariable]->stringValue);
				commaPos = strchr(temp_str, ',');
		    	index = commaPos - temp_str;			
				semicolon_checker(temp_str, &index, &commaPos, temp_indexs);
				previous_index = index;	
				if(commaPos != NULL)
				{
					commaPos = strchr(commaPos + 1, ',');
				}
				comma_seperator[C] = strtok(temp_str, ",");
				while(C<100) 
			    {
			        if(comma_seperator[C] != NULL)
			        {
			        	if(*numVariable == seperated_value_capacity)
			        	{
			        		break;
						}
			        	if(temp_indexs[0] != 0)
			        	{
			        		for(i = 0; i < semicolon_counter; i++)
				        	{
				        		comma_seperator[C][temp_indexs[i]] = ',';
							}
							semicolon_counter = 0;
							memset(temp_indexs, 0, sizeof(temp_indexs));
						}
			        	if(isNumeric(comma_seperator[C]))
						{
							mistake_places[C] = C + 1;
							mistake_counter ++;
						}
						else if(!isNumeric(comma_seperator[C]))
						{
							for(j =0; j<100; j++)
						    {
						    	if(strcmp((*variableArray)[j]->variable_name, comma_seperator[C]) == 0)
						    	{
						    		(*variableArray)[*numVariable]->intPtr = (int*)malloc(sizeof(int));
						    		(*variableArray)[*numVariable]->floatPtr = (float*)malloc(sizeof(float));
						    		(*variableArray)[*numVariable]->doublePtr = (double*)malloc(sizeof(double));
						    		if(strcmp((*variableArray)[j]->type, existing_var_type[0]) == 0 || strcmp((*variableArray)[j]->type, existing_var_type[1]) == 0 ||
						    		strcmp((*variableArray)[j]->type, existing_var_type[2]) == 0)
						    		{
										(*variableArray)[*numVariable]->intPtr = &((*variableArray)[j]->intValue);
										(*variableArray)[*numVariable]->intValue = (*variableArray)[j]->intValue;
										(*variableArray)[*numVariable]->floatPtr = &((*variableArray)[j]->floatValue);
										(*variableArray)[*numVariable]->floatValue = (*variableArray)[j]->floatValue;
										(*variableArray)[*numVariable]->doublePtr = &((*variableArray)[j]->doubleValue);
										(*variableArray)[*numVariable]->doubleValue = (*variableArray)[j]->doubleValue;
										break;
									}
									else if(strcmp((*variableArray)[j]->type, existing_var_type[5]) == 0 || strcmp((*variableArray)[j]->type, existing_var_type[6]) == 0 ||
						    		strcmp((*variableArray)[j]->type, existing_var_type[7]) == 0)
									{
										(*variableArray)[*numVariable]->intPtr = &(*((*variableArray)[j]->intPtr));
										(*variableArray)[*numVariable]->intValue = (*variableArray)[j]->intValue;
										(*variableArray)[*numVariable]->floatPtr = &(*((*variableArray)[j]->floatPtr));
										(*variableArray)[*numVariable]->floatValue = (*variableArray)[j]->floatValue;
										(*variableArray)[*numVariable]->doublePtr = &(*((*variableArray)[j]->doublePtr));
										(*variableArray)[*numVariable]->doubleValue = (*variableArray)[j]->doubleValue;
										break;
									}
									else 
									{
										printf("Error: The variable(%s) tried to be pointed by the pointer(%s) is not numeric.\n", (*variableArray)[j]->variable_name, (*variableArray)[*numVariable]->variable_name);
								   		break;
									}
								}
							}
						}
			            (*numVariable)++;
			            if(C == 0)
			            {
			            	var_name_start += strlen(comma_seperator[C]); 
						}
						else if(C > 0)
						{
							var_name_start += strlen(comma_seperator[C]) + 1; 
						}
			            if((*var_name_start) == ',' && *(var_name_start + 1) != ',' && *(var_name_start + 1) != ';')
			            {
				            C++;
				            strcpy(temp_str, second_temp_str + previous_index + 1);
				            commaPos = strchr(temp_str, ',');
			            	result = semicolon_checker(temp_str, &index, &commaPos, temp_indexs);
			            	if(result)
			            	{
			            		previous_index += index + 1;	
								if(commaPos != NULL)
								{
									commaPos = strchr(commaPos + 1, ',');
								}
							}
			            	else if(!result)
				            {
				            	strcat(temp_str, ",");
							}
				            comma_seperator[C] = strtok(temp_str, ",");
				            printf("comma_seperator[C] = %s\n", comma_seperator[C]);
				            continue;
						}
						else if((*var_name_start) == ';')
						{
							printf("\nReturned after last comma','\n");
							break;
						}
						else if(*(var_name_start + 1) == ',' || *(var_name_start + 1) == ';')
						{
							printf("\nError:%d.Variable name missing\n", (*numVariable) - temp_numVariable + 1);
							(*numVariable) = temp_numVariable;
							restart = 1;
							break;
						}
			        }
			        else if(comma_seperator[C] == NULL)
			        {
			            printf("Error:There is no variable name before first comma\n");
						(*numVariable) = temp_numVariable;
						(*variableArray)[*numVariable]->charValue = '\0';
						strcpy((*variableArray)[*numVariable]->stringValue, "");
						restart = 1;
			            break;
			        }
			    }
			    if(mistake_counter == 0)
			    {
			    	break;
				}
				else if(mistake_counter > 0)
				{
					mistake_specifier_in_comma_seperator(mistake_places, mistake_counter, 2);
					break;
				}
			}*/
		}
		if(restart == 0)
		{
			*name_seperated = 0;
		}
	}
}


					  
/*void variable_checker( VariableType *variableArray[], char *str)
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


int isNumeric(char *str) 
{
	int dot_counter = 0;
	int i = 0;
    if (str == NULL || str[0] == '\0' || str[0] == '.') 
	{
        return 0;  // Empty string or NULL pointer
    }

    while (str[i] != '\0') 
	{
        if (str[i] < '0' || str[i] > '9') 
		{
			if(str[0] == '-' || str[0] == '+')
			{
				i++;
				continue;
			}
			if(str[i] == '.' && dot_counter == 0)
			{
				dot_counter = 1;
				i++;
				continue;
			}
			else if(str[i] == '.' && dot_counter == 1)
			{
				str[i] = '0';
				i ++;
				continue;
			}
			else
			{
				return 0;  // Non-digit character found
			}
        }
        i++;
    }
    return 1;  // it's a number
}

void applie(void (*func)(), int number_of_applies) // Calling the function through the function pointer
{
	int applied_amount;
	for(applied_amount = 0; applied_amount < number_of_applies; applied_amount++)
	{
		func(); 
	}
}

void sleep_ms(unsigned int milliseconds) 
{
	#ifdef _WIN32
    Sleep(milliseconds);
	#else
    usleep(milliseconds * 1000); // usleep takes microseconds
	#endif
}
