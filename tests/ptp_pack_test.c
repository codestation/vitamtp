#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ptp.h"
#include "ptp-pack.c"

static void capture_debug(void *data, const char *format, va_list args)
{
    int *wrong_event = data;
    (void)args;
    if (strstr(format, "OLCInfoChanged"))
        *wrong_event = 1;
}

int main(void)
{
    PTPParams params = {0};
    PTPCanon_changes_entry *entries = NULL;
    int wrong_event = 0;
    int count;
    int failed;
    /* A property event with no property payload must not enter the OLC parser. */
    unsigned char data[16] = {0};
    htole32a(data, 8);
    htole32a(data + 4, PTP_EC_CANON_EOS_PropValueChanged);
    htole32a(data + 8, 8); /* End-of-events marker. */
    params.byteorder = PTP_DL_LE;
    params.debug_func = capture_debug;
    params.data = &wrong_event;
    count = ptp_unpack_CANON_changes(&params, data, sizeof(data), &entries);
    failed = wrong_event || count != 1 || !entries ||
             entries[0].type != PTP_CANON_EOS_CHANGES_TYPE_UNKNOWN;
    free(entries);
    return failed;
}
