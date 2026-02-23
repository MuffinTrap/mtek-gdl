#pragma once

#include <string.h>
#include <stdlib.h>
#include <mgdl/mgdl-types.h>
#include "base.h"

enum key_type {
	KEY_STEP,   /* stay constant */
	KEY_LINEAR, /* lerp to the next value */
	KEY_SMOOTH, /* smooth curve to the next value */
	KEY_RAMP,
	KEY_TYPE_COUNT
};

struct track_key {
	int row;
	float value;
	enum key_type type;
};

struct sync_track {
	const char *name;
	struct track_key *keys;
	int num_keys;
};

#ifdef __cplusplus
    extern "C" {
#endif

// CPP write
void save_sync_header(const sync_track** tracks, sizetype trackAmount, const char *filename_h, const char* filename_cpp);

// JSON write
void save_sync_json(const sync_track** tracks, sizetype trackAmount, const char *filename);

int sync_find_key(const struct sync_track *, int);
static inline int key_idx_floor(const struct sync_track *t, int row)
{
	int idx = sync_find_key(t, row);
	if (idx < 0)
		idx = -idx - 2;
	return idx;
}

#ifndef SYNC_PLAYER
int sync_set_key(struct sync_track *, const struct track_key *);
int sync_del_key(struct sync_track *, int);
static inline int is_key_frame(const struct sync_track *t, int row)
{
	return sync_find_key(t, row) >= 0;
}

#endif /* !defined(SYNC_PLAYER) */

#ifdef __cplusplus
	}
#endif

