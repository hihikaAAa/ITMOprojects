#ifndef PARSER_H
#define PARSER_H

struct formater
{
	char flags[6];
	char conversion_helper;
	int size;
};

int parser(const char *format, struct formater *helper);

#endif	  // PARSER_H
