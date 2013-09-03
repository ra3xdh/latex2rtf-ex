/* caption.c - routines that handle LaTeX caption package commands

Copyright (C) 2001-2002 The Free Software Foundation

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

This file is available from http://sourceforge.net/projects/latex2rtf/
 
Authors:
    2013 Vadim Kuznetsov
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#ifdef UNIX
#include <unistd.h>
#endif
#include "cfg.h"
#include "main.h"
#include "caption.h"
#include "parser.h"
#include "util.h"
#include "commands.h"
#include "convert.h"
#include "funct1.h"


void setPackageCaption(char *options)
{
    printf("%s\n",options);

    char *sep = strstr(options,"labelsep");

    if (sep) {
        if (strstr(sep,"endash")) strcpy(g_labelsep," \\'96 ");
	else 
	if (strstr(sep,"period")) strcpy(g_labelsep," . ");
    }
}

