/*
 * xdx - GTK+ DX-cluster client for amateur radio
 * Copyright (C) 2002-2003 Joop Stakenborg <pg4i@amsat.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Library General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

/*
 * preferences.c - private functions for saving and recalling xdx preferences.
 */

#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "preferences.h"
#include "utils.h"
#include "gui.h"

preferencestype preferences;

/*
 * check if ~/.xdx directory exists
 */

void
dircheck ()
{
  struct stat statdir;

  gui->preferencesdir = g_strdup_printf ("%s/.%s", g_get_home_dir (), PACKAGE);
  if (stat (gui->preferencesdir, &statdir) == -1)
    {
      if (mkdir (gui->preferencesdir, S_IRUSR | S_IWUSR | S_IXUSR) == -1)
	g_error (_("Creating ~/.%s directory."), PACKAGE);
    }
  else if (!S_ISDIR (statdir.st_mode))
    g_error (_("~/.%s is not a directory."), PACKAGE);
}

/*
 * look up settings in ~/.xdx/preferences
 */

void
loadpreferences (void)
{
  gchar *preferencesfile, label[100], value[100];
  FILE *fp;

  /* defaults */
  preferences.x = 10;
  preferences.y = 30;
  preferences.width = 700;
  preferences.height = 550;
  preferences.columnwidths = g_strdup("70,70,70,360,60,60,");
  preferences.autologin = 0;
  preferences.callsign = g_strdup("N0CALL");
  preferences.commands = g_strdup("set/page 0");
  preferences.savedx = 0;
  preferences.savewwv = 0;
  preferences.savetoall = 0;
  preferences.savewx = 0;
  preferences.hamlib = 0;
  preferences.rigctl = g_strdup("rigctl -m 1 set_freq %d");;
  preferences.browserapp = g_strdup("mozilla %s");
  preferences.mailapp = g_strdup("mozilla -compose \"to=%s\"");

  /* open preferences file */
  preferencesfile = g_strdup_printf ("%s/preferences", gui->preferencesdir);
  fp = fopen (preferencesfile, "r");
  if (fp)
  {
    while (!feof (fp))
      {
        if (fscanf (fp, "%s %s", label, value) == EOF) break;
	      if (!g_ascii_strcasecmp(label, "x")) 
          preferences.x = atoi(value);
        else if (!g_ascii_strcasecmp(label, "y")) 
          preferences.y = atoi(value);
        else if (!g_ascii_strcasecmp(label, "width")) 
          preferences.width = atoi(value);
        else if (!g_ascii_strcasecmp(label, "height")) 
          preferences.height = atoi(value);
        else if (!g_ascii_strcasecmp(label, "columnwidths")) 
          preferences.columnwidths = g_strdup(value);
        else if (!g_ascii_strcasecmp(label, "autologin")) 
          preferences.autologin = atoi(value);
        else if (!g_ascii_strcasecmp(label, "callsign")) 
          preferences.callsign = g_strdup(value);
        else if (!g_ascii_strcasecmp(label, "commands"))
        {
          g_strdelimit (value, "_", ' ');
          preferences.commands = g_strdup(value);
        }
        else if (!g_ascii_strcasecmp(label, "savedx")) 
          preferences.savedx = atoi(value);
        else if (!g_ascii_strcasecmp(label, "savewwv")) 
          preferences.savewwv = atoi(value);
        else if (!g_ascii_strcasecmp(label, "savetoall")) 
          preferences.savetoall = atoi(value);
        else if (!g_ascii_strcasecmp(label, "savewx")) 
          preferences.savewx = atoi(value);
        else if (!g_ascii_strcasecmp(label, "hamlib")) 
          preferences.hamlib = atoi(value);
        else if (!g_ascii_strcasecmp(label, "rigctl"))
        {
          g_strdelimit (value, "~", ' ');
          preferences.rigctl = g_strdup(value);
        }
        else if (!g_ascii_strcasecmp(label, "browserapp"))
        {
          g_strdelimit (value, "~", ' ');
          preferences.browserapp = g_strdup(value);
        }
        else if (!g_ascii_strcasecmp(label, "mailapp"))
        {
          g_strdelimit (value, "~", ' ');
          preferences.mailapp = g_strdup(value);
        }
      }
    fclose (fp);
  }
  g_free(preferencesfile);
}

/*
 * save preferences in ~/.xdx/preferences
 */

void
savepreferences (void)
{
  gchar *preferencesfile, *str;
  FILE *fp;

  /* open preferences file */
  preferencesfile = g_strdup_printf ("%s/preferences", gui->preferencesdir);
  fp = fopen (preferencesfile, "w");
  if (fp)
  {
    fprintf (fp, "version %s\n", VERSION);
    str = g_strdup_printf("%d", preferences.x);
    fprintf(fp, "x %s\n", str);
    str = g_strdup_printf("%d", preferences.y);
    fprintf(fp, "y %s\n", str);
    str = g_strdup_printf("%d", preferences.width);
    fprintf(fp, "width %s\n", str);
    str = g_strdup_printf("%d", preferences.height);
    fprintf(fp, "height %s\n", str);
    str = g_strdup_printf("%s", preferences.columnwidths);
    fprintf(fp, "columnwidths %s\n", str);
    str = g_strdup_printf("%d", preferences.autologin);
    fprintf(fp, "autologin %s\n", str);
    str = g_strdup_printf("%s", preferences.callsign);
    fprintf(fp, "callsign %s\n", str);
    str = g_strdup_printf("%s", preferences.commands);
    g_strdelimit (str, " ", '_');
    fprintf(fp, "commands %s\n", str);
    str = g_strdup_printf("%d", preferences.savedx);
    fprintf(fp, "savedx %s\n", str);
    str = g_strdup_printf("%d", preferences.savewwv);
    fprintf(fp, "savewwv %s\n", str);
    str = g_strdup_printf("%d", preferences.savetoall);
    fprintf(fp, "savetoall %s\n", str);
    str = g_strdup_printf("%d", preferences.savewx);
    fprintf(fp, "savewx %s\n", str);
    str = g_strdup_printf("%d", preferences.hamlib);
    fprintf(fp, "hamlib %s\n", str);
    str = g_strdup_printf("%s", preferences.rigctl);
    g_strdelimit (str, " ", '~');
    fprintf(fp, "rigctl %s\n", str);
    str = g_strdup_printf("%s", preferences.browserapp);
    g_strdelimit (str, " ", '~');
    fprintf(fp, "browserapp %s\n", str);
    str = g_strdup_printf("%s", preferences.mailapp);
    g_strdelimit (str, " ", '~');
    fprintf(fp, "mailapp %s\n", str);
    g_free(str);
    fclose (fp);
  }
  g_free(preferencesfile);
}
