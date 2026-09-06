#include <vitamtp.h>

int main(void)
{
    VitaMTP_Set_Logging(VitaMTP_NONE);
    VitaMTP_Log(VitaMTP_DEBUG, "Testing the installed header\n");
    return VitaMTP_Init() == 0 ? (VitaMTP_Cleanup(), 0) : 1;
}
