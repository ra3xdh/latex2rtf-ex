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

void TikzToPng(char *tikzcode, char *exts);

void CmdTikzPicture(int code)
/******************************************************************************
  purpose: handle \begin{tikzpicture} ... \end{tikzpicture}
           by converting to png image and inserting
 ******************************************************************************/
{
	char *pre, *post, *tikzcode, *param;	


	param = getBracketParam();
	printf("\n %s \n",param);

	if (code & ON) {
		pre = strdup("\\begin{tikzpicture}");
		post = strdup("\\end{tikzpicture}");
		tikzcode=getTexUntil(post,0);
		//printf(tikzcode);
		TikzToPng(tikzcode,param);
		//WriteLatexAsBitmap(pre,picture,post);
		ConvertString(post);  /* to balance the \begin{picture} */
		free(pre);
		free(post);
		free(tikzcode);
		free(param);
	}
}

void TikzToPng(char *tikzcode,char *exts)
{
        char *fullname, *tmp_dir, *texname, *auxname, *logname,
	        *pdfname;

        FILE *f;
	static int file_number = 0;

        char name[15];

	file_number++;

        tmp_dir = getTmpPath();
	snprintf(name,15,"t2p_%04d",file_number);
	fullname = strdup_together(tmp_dir,name);
	texname = strdup_together(fullname,".tex");
	pdfname = strdup_together(fullname,".pdf");
	auxname = strdup_together(fullname,".aux");
	logname = strdup_together(fullname,".log");


	f = fopen(texname, "w");

        
        fprintf(f,"\\documentclass[varwidth=true,border=10pt]{standalone}\n");
        fprintf(f,"\\usepackage{mathtext}\n");
        fprintf(f,"\\usepackage[T2A]{fontenc}\n");
        fprintf(f,"\\usepackage[%s]{inputenc}\n",g_charset_encoding_name);
        fprintf(f,"\\usepackage[russian]{babel}\n");

        fprintf(f,"\\usepackage{tikz}\n");

        fprintf(f,"\\begin{document}\n");

        fprintf(f,"\\usetikzlibrary{circuits}\n");
        fprintf(f,"\\usetikzlibrary{circuits.ee}\n");
        fprintf(f,"\\usetikzlibrary{circuits.ee.IEC}\n");
        fprintf(f,"\\usetikzlibrary{arrows}\n");
        fprintf(f,"\\usetikzlibrary{patterns}\n");

        fprintf(f,"\\begin{tikzpicture}[%s]\n",exts);
        
	fprintf(f,"\n%s\n",tikzcode);

	fprintf(f,"\\end{tikzpicture}\n");
	fprintf(f,"\\end{document}\n");

        fclose(f);

        printf("%s\n",texname);

        int cmd_len = strlen("pdflatex") + strlen(texname)+32+strlen(" >/dev/null");
	char *cmd = (char *)malloc(cmd_len);

        snprintf(cmd, cmd_len, "pdflatex %s >/dev/null",texname);
	printf(cmd);

        char *oldcwd = (char *)malloc(1024);
	getcwd(oldcwd,1024);
	printf(oldcwd);
	chdir(tmp_dir);
	int err = system(cmd);
	chdir(oldcwd);
	free(oldcwd);

	if (!err) PutPdfFile(pdfname,g_png_figure_scale,0,TRUE);

        free(fullname);
	free(texname);
	free(auxname);
	free(pdfname);
	free(logname);
	free(tmp_dir);
}
