#include "includes.h"
#include <linux/Bandrich/sharememory.h>
#include <sys/ipc.h>
#include <sys/shm.h>

int BR_hostapd_shm_open(all_info_t** all_info);

int BR_hostapd_shm_close(void);

int BR_hostapd_shm_reset_data(void);
