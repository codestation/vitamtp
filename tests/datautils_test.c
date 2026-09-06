#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <vitamtp.h>

static int check_timestamp(int year, const char *expected)
{
    struct tm date = {0};
    time_t value;
    char *timestamp;
    int failed;

    date.tm_year = year - 1900;
    date.tm_mday = 1;
    value = mktime(&date);
    /* Some C runtimes cannot represent years beyond 9999. */
    if (value == (time_t)-1)
        return year <= 9999;
    timestamp = VitaMTP_Data_Make_Timestamp(value);
    failed = !timestamp || strcmp(timestamp, expected) != 0;
    free(timestamp);
    return failed;
}

int main(void)
{
    char *xml = NULL;
    int len = 0;
    int failed;

    /* CTest sets TZ=UTC0 to make these expected values deterministic. */
    if (check_timestamp(2000, "2000-01-01T00:00:00+00:00") ||
        check_timestamp(100000, "100000-01-01T00:00:00+00:00"))
        return 1;

    if (VitaMTP_Init() != 0)
        return 2;
    failed = VitaMTP_Data_Metadata_To_XML(NULL, &xml, &len) != 0;
    if (!failed)
        failed = !xml || len < 5 || len != (int)strlen(xml + 4) + 5 ||
                 !strstr(xml + 4, "<objectMetadata/>");
    free(xml);
    VitaMTP_Cleanup();
    return failed;
}
