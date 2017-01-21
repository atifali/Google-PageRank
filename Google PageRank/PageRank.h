#pragma once

#define WEB "web.txt"
#define BUFFER 128 // buffer size for the file parsing

//function prototypes
int get_web_dimension(FILE * web_file);
double ** parse_web(FILE * web_file, int dimension);