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
#include <sys/types.h>
#include <sys/stat.h>
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


static char *tikzlibs[32];     
static int tikzlibsnum = 0;



void TikzToPng(char *tikzcode, char *exts);

void CmdTikzPicture(int code)
/******************************************************************************
  purpose: handle \begin{tikzpicture} ... \end{tikzpicture}
           by converting to png image and inserting
 ******************************************************************************/
{
	char *pre, *post, *tikzcode, *param;	

	param = getBracketParam();

	if (code & ON) {
		pre = strdup("\\begin{tikzpicture}");
		post = strdup("\\end{tikzpicture}");
		tikzcode=getTexUntil(post,0);
		TikzToPng(tikzcode,param);
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
	        *pdfname, *pngname, *destname;

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
	fprintf(f,"\\usepackage{gnuplot-lua-tikz}\n");

        fprintf(f,"\\begin{document}\n");

        int i;
	for (i=0;i<tikzlibsnum;i++) {
	    fprintf(f,"\\usetikzlibrary{%s}\n",tikzlibs[i]);
	}


        if (exts!=NULL)
        fprintf(f,"\\begin{tikzpicture}[%s]\n",exts);
	else 
        fprintf(f,"\\begin{tikzpicture}\n");
        
	fprintf(f,"\n%s\n",tikzcode);

	fprintf(f,"\\end{tikzpicture}\n");
	fprintf(f,"\\end{document}\n");

        fclose(f);


        int cmd_len = strlen("pdflatex") + strlen(texname)+32+strlen(" >/dev/null");
	char *cmd = (char *)malloc(cmd_len);

        snprintf(cmd, cmd_len, "pdflatex %s >/dev/null",texname);

        char *oldcwd = (char *)malloc(1024);
	getcwd(oldcwd,1024);
	chdir(tmp_dir);
	int err = system(cmd);
	chdir(oldcwd);
	free(cmd);

        if (!err) PutPdfFile(pdfname,g_png_figure_scale,0,TRUE);

        if (g_figs_extract && g_processing_figure) {
	    struct stat st ={0};
	    if (stat(g_figsdir, &st) == -1) {
	       mkdir(g_figsdir,0755);
	    }

	    g_fignum++;
            
	    char *figname = (char *)malloc(15*sizeof(char));
            snprintf(figname,15,"Ris%d.png",g_fignum);

	    destname = strdup_together(g_figsdir,"/");
	    pngname = strdup_together(destname,figname);

	    cmd_len = strlen("convert -alpha off -density 300x300 ")+strlen(destname)+strlen(pdfname)+32;
	    cmd = (char *) malloc(cmd_len);
	    snprintf(cmd,cmd_len,"convert -density 300x300 %s -alpha off %s ",pdfname,pngname);
	    printf(cmd);
	    system(cmd);
	    free(cmd);
	    free(destname);
	    free(pngname);
	}

        remove(texname);
	remove(auxname);
	remove(pdfname);
	remove(logname);

        free(oldcwd);
        free(fullname);
	free(texname);
	free(auxname);
	free(pdfname);
	free(logname);
	free(tmp_dir);
}

void CmdTikzlib(int code) //Запомнить библиотеку Tikz
{
       
       char *tikzlib = getBraceParam();

       tikzlibsnum++;

       tikzlibs[tikzlibsnum-1]=tikzlib;
}
