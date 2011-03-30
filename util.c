#include <util.h>

#include <string.h>
#include <stdlib.h>

char *strrstr(char *s1, char *s2)
{
	char *sc2, *psc1, *ps1;

	if (*s2 == '\0')
		return((char *)s1);

	ps1 = s1 + strlen(s1);

	while (ps1 != s1) {
		--ps1;
		for (psc1 = ps1, sc2 = s2; ; )
			if (*(psc1++) != *(sc2++))
				break;
			else if (*sc2 == '\0')
				return ((char *)ps1);
	}
	return ((char *)NULL);
}
