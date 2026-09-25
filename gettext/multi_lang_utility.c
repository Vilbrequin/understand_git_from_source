#include <libintl.h>
#include <locale.h>
#include <stdio.h>

/* when we run the executable that has the gettext api, gettext looks for 
 * a compiled translation file called <textdomain>.mo, using a path like follow : 
 * [bindtextdomain_path]/[LANG]/LC_MESSAGES/[textdomain].mo 
 */

int main(char argc, char ** argv){
	// setup locale to use the configuration in LANG env var, and apply that
	// all configurations/facets.
	setlocale(LC_ALL, "");

	// we shall set the base directory that contains message cattalogs
	bindtextdomain("my-domaine", "/usr/share/locale");

	// now we shall speacify the domain it self
	textdomain("my-domaine");
	
	printf(gettext("Hi welcome to this api that does nothing\n"));

	return 0;
}
