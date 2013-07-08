/* tikz.c - routines that handle LaTeX TikZ commands

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
#include "graphics.h"
#include "tikz.h"
#include "parser.h"
#include "util.h"
#include "commands.h"
#include "convert.h"
#include "equation.h"
#include "funct1.h"



void CmdTikzPicture(int code)
/******************************************************************************
  purpose: handle \begin{tikzpicture} ... \end{tikzpicture}
           by converting to png image and inserting
 ******************************************************************************/
{
	char *pre, *post, *picture;	
	
	if (code & ON) {
		pre = strdup("\\begin{tikzpicture}");
		post = strdup("\\end{tikzpicture}");
		picture=getTexUntil(post,0);
		printf(picture);
		//WriteLatexAsBitmap(pre,picture,post);
		ConvertString(post);  /* to balance the \begin{picture} */
		free(pre);
		free(post);
		free(picture);
	}
}


