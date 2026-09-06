#include <stdlib.h>
#include <vitamtp.h>

static void emit_logs(void)
{
    VitaMTP_Log(VitaMTP_ERROR, "error %d\n", 42);
    VitaMTP_Log(VitaMTP_INFO, "info %s\n", "message");
    VitaMTP_Log(VitaMTP_VERBOSE, "verbose\n");
    VitaMTP_Log(VitaMTP_DEBUG, "debug\n");
}

int main(int argc, char **argv)
{
    int evaluated = 0;

    /* With no argument, exercise the default level before initialization. */
    if (argc > 1)
        VitaMTP_Set_Logging(atoi(argv[1]));
    emit_logs();

    VitaMTP_Set_Logging(VitaMTP_NONE);
    VitaMTP_Log(VitaMTP_ERROR, "disabled %d\n", ++evaluated);
    VitaMTP_Log_Message(VitaMTP_ERROR, "direct", 1, "also disabled\n");
    return evaluated != 0 || VitaMTP_Log_Enabled(VitaMTP_DEBUG);
}
