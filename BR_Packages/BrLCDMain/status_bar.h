#ifndef status_bar_H_
#define status_bar_H_
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <linux/Bandrich/sharememory.h>
void drawblk(char*_file_name,int x_off_set);void reset_bar();void drawblk_1(int _param);void drawblk_2(all_info_t*_all_info);void drawblk_3(int _param);void drawblk_4(int _param);void drawblk_5(int _param);void drawblk_6(int _param);void draw_bar(all_info_t*all_info);
#endif /* status_bar_H_ */
