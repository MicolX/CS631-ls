#include "sort.h"


struct stat sba;
struct stat sbb;

int
(compar)(const FTSENT **a, const FTSENT **b)
{
	if (S_ISDIR((*a)->fts_statp->st_mode) && !S_ISDIR((*b)->fts_statp->st_mode)) {
		return 1;
	}
	
	if (!S_ISDIR((*a)->fts_statp->st_mode) && S_ISDIR((*b)->fts_statp->st_mode)) {
		return -1;
	}
	
	return alphb(a, b);
}

int
(alphb)(const FTSENT **a, const FTSENT **b)
{
	return strcmp((*a)->fts_name, (*b)->fts_name);
}

int
(byatime)(const FTSENT **a, const FTSENT **b)
{
	if (difftime((*a)->fts_statp->st_atime, (*b)->fts_statp->st_atime) > 0) {
		return -1;
	}

	if (difftime((*a)->fts_statp->st_atime, (*b)->fts_statp->st_atime) < 0) {
		return 1;
	}

	return alphb(a, b);
}

int
(bymtime)(const FTSENT **a, const FTSENT **b)
{
	if (difftime((*a)->fts_statp->st_mtime, (*b)->fts_statp->st_mtime) > 0) {
		return -1;
	}

	if (difftime((*a)->fts_statp->st_mtime, (*b)->fts_statp->st_mtime) < 0) {
		return 1;
	}

	return alphb(a, b);
}

int
(byctime)(const FTSENT **a, const FTSENT **b)
{
	if (difftime((*a)->fts_statp->st_ctime, (*b)->fts_statp->st_ctime) > 0) {
		return -1;
	}

	if (difftime((*a)->fts_statp->st_ctime, (*b)->fts_statp->st_ctime) < 0) {
		return 1;
	}

	return alphb(a, b);
}

int
(bysize)(const FTSENT **a, const FTSENT **b)
{
	if ((*a)->fts_statp->st_size - (*b)->fts_statp->st_size > 0) {
		return -1;
	}

	if ((*a)->fts_statp->st_size - (*b)->fts_statp->st_size < 0) {
		return 1;
	}

	return alphb(a, b);
}

int
(compar_rev)(const FTSENT **a, const FTSENT **b)
{
	if (S_ISDIR((*a)->fts_statp->st_mode) && !S_ISDIR((*b)->fts_statp->st_mode)) {
		return 1;
	}
	
	if (!S_ISDIR((*a)->fts_statp->st_mode) && S_ISDIR((*b)->fts_statp->st_mode)) {
		return -1;
	}
	
	return alphb_rev(a, b);
}

int
(alphb_rev)(const FTSENT **a, const FTSENT **b)
{
	return -1*strcmp((*a)->fts_name, (*b)->fts_name);
}

int
(byatime_rev)(const FTSENT **a, const FTSENT **b)
{
	if (difftime((*a)->fts_statp->st_atime, (*b)->fts_statp->st_atime) > 0) {
		return 1;
	}

	if (difftime((*a)->fts_statp->st_atime, (*b)->fts_statp->st_atime) < 0) {
		return -1;
	}

	return alphb_rev(a, b);
}

int
(bymtime_rev)(const FTSENT **a, const FTSENT **b)
{
	if (difftime((*a)->fts_statp->st_mtime, (*b)->fts_statp->st_mtime) > 0) {
		return 1;
	}

	if (difftime((*a)->fts_statp->st_mtime, (*b)->fts_statp->st_mtime) < 0) {
		return -1;
	}

	return alphb_rev(a, b);
}

int
(byctime_rev)(const FTSENT **a, const FTSENT **b)
{
	if (difftime((*a)->fts_statp->st_ctime, (*b)->fts_statp->st_ctime) > 0) {
		return 1;
	}

	if (difftime((*a)->fts_statp->st_ctime, (*b)->fts_statp->st_ctime) < 0) {
		return -1;
	}

	return alphb_rev(a, b);
}

int
(bysize_rev)(const FTSENT **a, const FTSENT **b)
{
	if ((*a)->fts_statp->st_size - (*b)->fts_statp->st_size > 0) {
		return 1;
	}

	if ((*a)->fts_statp->st_size - (*b)->fts_statp->st_size < 0) {
		return -1;
	}

	return alphb_rev(a, b);
}

