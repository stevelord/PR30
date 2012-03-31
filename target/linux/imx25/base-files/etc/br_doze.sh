#!/bin/sh
. /etc/functions.sh
 
config_load wfpower

config_get offmode default offmode
config_get idletime default idletime


set_doze_shm () {
     if [ "$offmode" = "auto" ]; then
       ushm wifi_auto_off 1 
     else
       ushm wifi_auto_off 0
     fi
     ushm sleep_time "$idletime"
}
