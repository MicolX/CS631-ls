#include "sort.h"


int
(compar)(const FTSENT **a, const FTSENT **b)
{
	if (S_ISDIR((*a)->fts_statp->st_mode) && !S_ISDIR((*b)->fts_statp->st_mode)) {
		return 1;
	}
	
	if (!S_ISDIR((*a)->fts_statp->st_mode) && S_ISDIR((*b)->fts_statp->st_mode)) {
		return -1;
	}

	return strcmp((*a)->fts_name, (*b)->fts_name);
}
