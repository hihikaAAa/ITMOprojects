#ifndef FORMATER_TO_NUMBER_H
#define FORMATER_TO_NUMBER_H

#include "int128.h"
#include "lib_usage.h"
#include "parser.h"

void format_number(int128_t value, struct formater *helper, char *out_buf);

#endif	  // FORMATER_TO_NUMBER_H
