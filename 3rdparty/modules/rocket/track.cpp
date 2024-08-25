#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <stdio.h>

#include "sync.h"
#include "track.h"
#include "base.h"

double key_linear(const struct track_key k[2], double row) {
    double t = (row - k[0].row) / (k[1].row - k[0].row);
    return k[0].value + (k[1].value - k[0].value) * t;
}

double key_smooth(const struct track_key k[2], double row) {
    double t = (row - k[0].row) / (k[1].row - k[0].row);
    t = t * t * (3 - 2 * t);
    return k[0].value + (k[1].value - k[0].value) * t;
}

double key_ramp(const struct track_key k[2], double row) {
    double t = (row - k[0].row) / (k[1].row - k[0].row);
    t = pow(t, 2.0);
    return k[0].value + (k[1].value - k[0].value) * t;
}

double sync_get_val_ref(const struct sync_track &t, double row) {
    int idx, irow;

    /* If we have no keys at all, return a constant 0 */
    if (!t.num_keys)
        return 0.0f;

    irow = (int)floor(row);
    idx = key_idx_floor(&t, irow);

    /* at the edges, return the first/last value */
    if (idx < 0)
        return t.keys[0].value;
    if (idx > (int)t.num_keys - 2)
        return t.keys[t.num_keys - 1].value;

    /* interpolate according to key-type */
    switch (t.keys[idx].type) {
    case KEY_STEP:
        return t.keys[idx].value;
    case KEY_LINEAR:
        return key_linear(t.keys + idx, row);
    case KEY_SMOOTH:
        return key_smooth(t.keys + idx, row);
    case KEY_RAMP:
        return key_ramp(t.keys + idx, row);
    default:
        assert(0);
        return 0.0f;
    }
}
double sync_get_val_ptr(const struct sync_track *t, double row) {
    int idx, irow;

    /* If we have no keys at all, return a constant 0 */
    if (!t->num_keys)
    {
        return 0.0f;
    }

    irow = (int)floor(row);
    idx = key_idx_floor(t, irow);

    /* at the edges, return the first/last value */
    if (idx < 0)
        return t->keys[0].value;
    if (idx > (int)t->num_keys - 2)
        return t->keys[t->num_keys - 1].value;

    /* interpolate according to key-type */
    switch (t->keys[idx].type) {
    case KEY_STEP:
        return t->keys[idx].value;
    case KEY_LINEAR:
        return key_linear(t->keys + idx, row);
    case KEY_SMOOTH:
        return key_smooth(t->keys + idx, row);
    case KEY_RAMP:
        return key_ramp(t->keys + idx, row);
    default:
        assert(0 && "Invalid key type");
        return 0.0f;
    }
}

void start_save_sync(const char *filename_h, const char *filename_cpp) {
    FILE *file_h = fopen(filename_h, "w");
    if (file_h == NULL)
    {
        printf("Cannot write tracks: No such directory/file %s\n", filename_h);
        return;
    }
    fprintf(file_h, "// sync data declaration\n");
    fprintf(file_h, "#ifdef SYNC_PLAYER\n");
    fprintf(file_h, "#pragma once\n");
    fprintf(file_h, "#include \"../rocket/track.h\"\n");
    fclose(file_h);

    FILE *file_cpp = fopen(filename_cpp, "w");
    if (file_cpp == NULL) {
        perror("Error opening file");
        return;
    }
    fprintf(file_cpp, "// sync data implementation\n");
    fprintf(file_h, "#ifdef SYNC_PLAYER\n");
    fprintf(file_cpp, "#include \"../rocket/track.h\"\n");
    fclose(file_cpp);
}

void end_save_sync(const char *filename_h, const char *filename_cpp) {
    FILE *file_h = fopen(filename_h, "a");
    fprintf(file_h, "#endif\n // SYNC_PLAYER");
    fclose(file_h);

    FILE *file_cpp = fopen(filename_cpp, "a");
    fprintf(file_h, "#endif\n // SYNC_PLAYER");
    fclose(file_cpp);
}

const char* key_type_to_string(enum key_type tp) {
    switch(tp) {
        case KEY_STEP: return "KEY_STEP"; break;
        case KEY_LINEAR: return "KEY_LINEAR"; break;
        case KEY_SMOOTH: return "KEY_RAMP"; break;
        case KEY_RAMP: return "KEY_RAMP"; break;
        default: return ""; break;
    };
}

void save_sync(const struct sync_track *t, const char *filename_h, const char *filename_cpp) {
    FILE *file_h = fopen(filename_h, "a");
    if (file_h == NULL) {
        perror("Error opening file");
        return;
    }
    FILE *file_cpp = fopen(filename_cpp, "a");
    if (file_cpp == NULL) {
        perror("Error opening file");
        return;
    }

    // Track contents as static in .cpp file
    fprintf(file_cpp, "static track_key %s_keys[] = {", t->name);
    for (int i = 0; i < t->num_keys; i++) {
        int row = t->keys[i].row;
        float value = t->keys[i].value;
        enum key_type type = t->keys[i].type;

        fprintf(file_cpp, "{ %d, %f, %s}, ", row, value, key_type_to_string(type));
    }
    fprintf(file_cpp, "};\n");

    // Track names as extern in .h file
    fprintf(file_h, "extern const sync_track %s;\n",  t->name);

    // Tracks in .cpp file
    // define the variable
    //fprintf(file_cpp, "const sync_track %s;\n", t->name);

    // assign to it
    fprintf(file_cpp, "const sync_track %s = { \"%s\", ", t->name, t->name);
    fprintf(file_cpp, "%s_keys", t->name);
    fprintf(file_cpp, ",%d};\n", t->num_keys);

    fclose(file_h);
    fclose(file_cpp);
}

int sync_find_key(const struct sync_track *t, int row) {
    int lo = 0, hi = t->num_keys;

    /* binary search, t->keys is sorted by row */
    while (lo < hi) {
        int mi = (lo + hi) / 2;
        assert(mi != hi);

        if (t->keys[mi].row < row)
            lo = mi + 1;
        else if (t->keys[mi].row > row)
            hi = mi;
        else
            return mi; /* exact hit */
    }
    assert(lo == hi);

    /* return first key after row, negated and biased (to allow -0) */
    return -lo - 1;
}

#ifndef SYNC_PLAYER
int sync_set_key(struct sync_track *t, const struct track_key *k) {
    int idx = sync_find_key(t, k->row);
    if (idx < 0) {
        /* no exact hit, we need to allocate a new key */
        void *tmp;
        idx = -idx - 1;
        tmp = realloc(t->keys, sizeof(struct track_key) * (t->num_keys + 1));
        if (!tmp)
            return -1;
        t->num_keys++;
        t->keys = (struct track_key *)tmp; // Casting to struct track_key *
        memmove(t->keys + idx + 1, t->keys + idx, sizeof(struct track_key) * (t->num_keys - idx - 1));
    }
    t->keys[idx] = *k;
    return 0;
}

int sync_del_key(struct sync_track *t, int pos) {
    void *tmp;
    int idx = sync_find_key(t, pos);
    assert(idx >= 0);
    memmove(t->keys + idx, t->keys + idx + 1, sizeof(struct track_key) * (t->num_keys - idx - 1));
    assert(t->keys);
    tmp = realloc(t->keys, sizeof(struct track_key) * (t->num_keys - 1));
    if (t->num_keys != 1 && !tmp)
        return -1;
    t->num_keys--;
    t->keys = (struct track_key *)tmp; // Casting to struct track_key *
    return 0;
}
#endif
