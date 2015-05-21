#include <stdio.h>
#include <stdlib.h>
#include "lib/mlrutil.h"
#include "containers/lrec_parsers.h"
#include "input/file_reader_mmap.h"
#include "input/lrec_readers.h"

typedef struct _lrec_reader_xtab_mmap_state_t {
	char irs;
	char ips; // xxx make me real
	int allow_repeat_ips;
	int at_eof;
	// xxx need to remember EOF for subsequent read
} lrec_reader_xtab_mmap_state_t;

// ----------------------------------------------------------------
static lrec_t* lrec_reader_xtab_mmap_func(mmap_reader_state_t* phandle, void* pvstate, context_t* pctx) {
	lrec_reader_xtab_mmap_state_t* pstate = pvstate;

	if (pstate->at_eof)
		return NULL;
	else
		return lrec_parse_xtab_mmap(phandle, pstate->irs, pstate->ips, pstate->allow_repeat_ips);
}

// xxx rename resets to sof_reset or some such
static void reset_xtab_func(void* pvstate) {
	lrec_reader_xtab_mmap_state_t* pstate = pvstate;
	pstate->at_eof = FALSE;
}

lrec_reader_mmap_t* lrec_reader_xtab_mmap_alloc(char irs, char ips, int allow_repeat_ips) {
	lrec_reader_mmap_t* plrec_reader = mlr_malloc_or_die(sizeof(lrec_reader_mmap_t));

	lrec_reader_xtab_mmap_state_t* pstate = mlr_malloc_or_die(sizeof(lrec_reader_xtab_mmap_state_t));
	//pstate->ips              = ips;
	//pstate->allow_repeat_ips = allow_repeat_ips;
	pstate->irs              = irs;
	pstate->ips              = ' ';
	pstate->allow_repeat_ips = TRUE;
	pstate->at_eof           = FALSE;
	plrec_reader->pvstate         = (void*)pstate;

	plrec_reader->plrec_reader_func = &lrec_reader_xtab_mmap_func;
	plrec_reader->preset_func  = &reset_xtab_func;

	return plrec_reader;
}