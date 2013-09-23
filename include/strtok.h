#ifndef __STOTOK_H__
#define __STOTOK_H__ 1


char * strtok(char *s, const char *delim);
char * strtok_r(char *s, const char *delim, char **last);

#endif