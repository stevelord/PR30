/* This source has been formatted by an unregistered SourceFormatX */
/* If you want to remove this info, please register this shareware */
/* Please visit http://www.textrush.com to get more information    */

#ifndef br_define_H_
#define br_define_H_
#include <semaphore.h>
typedef enum BR_Lcd_menu_e{BR_BASIC=1,BR_3G_Status,BR_WiFi_Status,BR_Version,BR_WPS_Fun}BR_Lcd_menu_e_type;static sem_t g_update_sem;static sem_t g_timer_sem;
#endif /* br_define_H_ */
