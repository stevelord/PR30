#ifndef OLED_DRAW_API_H_
#define OLED_DRAW_API_H_
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
typedef struct BR_oled_icon_cfg_s{int ID;int x_offset;int y_offset;int**ICON_Bmp;}BR_oled_icon_cfg_s_type;typedef struct BR_oled_font_cfg_s{int ID;int x_offset;int y_offset;int font_width;int font_height;int oled_width;int oled_height;int autowrap;}BR_oled_font_cfg_s_type;int UTF8ToUCS4(char*inbuf,unsigned int len_src,char*outbuf,unsigned int lenout);int Draw_string_line(char*_filename,int _font_size,int _y_offset);int Draw_string_line1(char*_filename,int _font_size);int Draw_string_line2(char*_filename,int _font_size);void freefont();
#endif /* OLED_DRAW_API_H_ */
