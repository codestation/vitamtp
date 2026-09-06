#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <vitamtp.h>

int main(void)
{
    const initiator_info_t *info;
    char *xml = NULL;
    int len = 0;

    if (sizeof(browse_info_t) != 20 || sizeof(treat_object_t) != 12 ||
        offsetof(send_part_init_t, offset) != 4 || sizeof(send_part_init_t) != 20)
        return 1;
    VitaMTP_Set_Logging(VitaMTP_NONE);
    VitaMTP_Log(VitaMTP_DEBUG, "Testing the logging API\n");
    if (VitaMTP_Init() != 0)
        return 2;
    info = VitaMTP_Data_Initiator_New("CTest", 2100010);
    if (!info || VitaMTP_Data_Initiator_To_XML(info, &xml, &len) < 0 ||
        !xml || len <= 6 || strcmp(xml + len - 4, "/>\n") != 0)
        return 3;
    free(xml);
    VitaMTP_Data_Free_Initiator(info);
#if !TEST_USB_SUPPORT
    if (VitaMTP_USB_Init() != -1 || VitaMTP_USB_Clear(NULL) != -1 ||
        VitaMTP_USB_Reset(NULL) != -1)
        return 4;
#endif
#if !TEST_WIRELESS_SUPPORT
    if (VitaMTP_Get_First_Wireless_Vita(NULL, 0, NULL, NULL, NULL) != NULL)
        return 5;
    VitaMTP_Cancel_Get_Wireless_Vita();
#endif
    VitaMTP_Cleanup();
    return 0;
}
