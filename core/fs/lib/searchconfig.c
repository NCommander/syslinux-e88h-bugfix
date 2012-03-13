#include <dprintf.h>
#include <stdio.h>
#include <string.h>
#include <core.h>
#include <fs.h>

char ConfigName[FILENAME_MAX];
char config_cwd[FILENAME_MAX];

/*
 * This searches for a specified set of filenames in a specified set
 * of directories.  If found, set the current working directory to
 * match.
 */
int search_dirs(struct com32_filedata *filedata,
		const char *search_directories[],
		const char *filenames[],
		char *realname)
{
    char namebuf[FILENAME_MAX];
    const char *sd, **sdp;
    const char *sf, **sfp;

    for (sdp = search_directories; (sd = *sdp); sdp++) {
	for (sfp = filenames; (sf = *sfp); sfp++) {
	    snprintf(namebuf, sizeof namebuf,
		     "%s%s%s",
		     sd, (*sd && sd[strlen(sd)-1] == '/') ? "" : "/",
		     sf);
	    realpath(realname, namebuf, FILENAME_MAX);
	    dprintf("Directory search: %s\n", realname);
	    if (open_file(realname, filedata) >= 0) {
		chdir(sd);
		return 0;	/* Got it */
	    }
	}
    }

    return -1;
}
