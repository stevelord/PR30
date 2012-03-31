#include "BR_hostapd.h"

//#define BR_HOSTAPD_WIFI_IFNAME	("tiwlan0")

void *BR_hostapd_shared_memory = (void*)(-1);

int BR_hostapd_shm_open(all_info_t** all_info)
{
	int shmid;

	srand((unsigned int)getpid());

	if ( 0 > (shmid = shmget((key_t)SHARE_KEY,sizeof(all_info_t),0666|IPC_CREAT)) )
	{
		printf("BR hostapd: shmget failed!\n");
		return -1;
	}

	if ( 0 > (BR_hostapd_shared_memory = shmat(shmid,(void*)0,0)) )
	{
		printf("BR hostapd: shmat failed!\n");
		return -1;
	}

	*all_info = (all_info_t*)BR_hostapd_shared_memory;
	return 0;
}

int BR_hostapd_shm_close(void)
{
	if (0 > BR_hostapd_shared_memory)
	{
		printf("BR hostapd: shm_close: shm not initialized!\n");
		return -1;
	}
 	
	if (0 > shmdt(BR_hostapd_shared_memory))
	{
		printf("BR hostapd: shmdt failed!\n");
		return -1;
	}

	BR_hostapd_shared_memory = (void*)(-1);
	return 0;
}

int BR_hostapd_shm_reset_data(void)
{
	all_info_t *all_info = NULL;

	if ( 0 == BR_hostapd_shm_open(&all_info) )
	{
		// Reset share memory data
		all_info->u_wifi.num_sta = 0;

		BR_hostapd_shm_close();
		return 0;
	}

	return -1;
}
