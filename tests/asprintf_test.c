#include <stdlib.h>
#include <string.h>
#include "asprintf.h"

int main(void)
{
    char *text = NULL;
    int len = asprintf(&text, "%s:%d", "Vita", 42);
    int failed = len != 7 || !text || strcmp(text, "Vita:42") != 0;
    free(text);
    if (failed)
        return 1;
    len = asprintf(&text, "%s", "");
    failed = len != 0 || !text || text[0] != '\0';
    free(text);
    return failed;
}
