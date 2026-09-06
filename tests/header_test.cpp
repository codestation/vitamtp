#include <vitamtp.h>

int main()
{
    VitaMTP_Set_Logging(VitaMTP_NONE);
    VitaMTP_Log(VitaMTP_DEBUG, "C++ logging: %d\n", 42);
    vita_raw_device_t device{};
    return device.serial[0];
}
