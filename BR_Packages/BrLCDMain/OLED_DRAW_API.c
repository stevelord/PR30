#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <iconv.h>
#include "OLED_DRAW_API.h"
#include <ft2build.h>
#include FT_FREETYPE_H
static int DEFAULT_Y_OFFSET=22;static int DEFAULT_Y_OFFSET2=18;unsigned int BLACK_POINT=0;unsigned int WHITE_POINT=1;extern char dot_matrix[128][64];static FT_Library library;static FT_Face face;
#define BR_USE_CIONV
int UTF8ToUCS4(char*inbuf,unsigned int len_src,char*outbuf,unsigned int lenout){iconv_t conv;conv=iconv_open("UCS-4LE","UTF-8");iconv(conv,NULL,NULL,NULL,NULL);int ret;ret=iconv(conv,(const char**)&inbuf,&len_src,&outbuf,&lenout);iconv_close(conv);return ret;}int Draw_string_line(char*_filename,int _font_size,int _y_offset){char*ttf_path="brlcd.ttf";unsigned int ucode_array[50];int array_counter=0;int max_font_height=0;int max_font_width=0;static int _DoinitLib=1;FT_UInt glyph_index;int x_offset=0;int y_offset=_y_offset;const int oled_width=128;const int oled_height=64;char bufout[512];unsigned int len_src=strlen(_filename);unsigned int*out_text;
#ifdef BR_USE_CIONV
int ret;ret=UTF8ToUCS4(_filename,len_src,bufout,512);out_text=(unsigned int*)bufout;
#else 
out_text=(unsigned int*)line;
#endif 
int i;unsigned int w_tmp;for(i=0;i<len_src;i++){w_tmp=out_text[i];if(w_tmp==0x0a||w_tmp==0){printf("After enc End of char(%d) = 0x%x \n",array_counter,w_tmp);break;}else{ucode_array[array_counter]=w_tmp;array_counter++;}}if(_DoinitLib){if(FT_Init_FreeType(&library)||FT_New_Face(library,ttf_path,0,&face)){return 2;}_DoinitLib=0;printf("Init Freetype lib.\n");}if(FT_Set_Char_Size(face,0,_font_size*64,72,72))return 3;int _isSkipDraw=0;if(y_offset>DEFAULT_Y_OFFSET){_isSkipDraw=1;}x_offset=0;int total_width=0;for(i=0;i<array_counter;i++){glyph_index=FT_Get_Char_Index(face,ucode_array[i]);if(glyph_index==0){continue;}if(FT_Load_Glyph(face,glyph_index,FT_LOAD_DEFAULT)){return 9;}if(FT_Render_Glyph(face->glyph,FT_RENDER_MODE_MONO)){return 10;}if(max_font_height<face->glyph->bitmap.rows){
max_font_height=face->glyph->bitmap.rows;max_font_width=face->glyph->bitmap.width;}if((face->glyph->bitmap.width+(face->glyph->metrics.horiBearingX>>6)+total_width)>oled_width){break;}total_width+=face->glyph->bitmap.width+(face->glyph->metrics.horiBearingX>>6);}if(total_width<oled_width){x_offset=oled_width/2-total_width/2;}for(i=0;i<array_counter;i++){glyph_index=FT_Get_Char_Index(face,ucode_array[i]);if(glyph_index==0){return 8;}if(FT_Load_Glyph(face,glyph_index,FT_LOAD_DEFAULT)){return 9;}if(FT_Render_Glyph(face->glyph,FT_RENDER_MODE_MONO)){return 10;}if((face->glyph->bitmap.width+(face->glyph->metrics.horiBearingX>>6)+x_offset)>oled_width){if(_isSkipDraw)break;x_offset=face->glyph->metrics.horiBearingX>>6;y_offset+=DEFAULT_Y_OFFSET2;_isSkipDraw=1;}int row;int _baseLine_offset=
_font_size-(face->glyph->metrics.horiBearingY>>6);x_offset+=(face->glyph->metrics.horiBearingX>>6);for(row=0;row<face->glyph->metrics.height>>6;++row){int pixel;for(pixel=0;pixel<face->glyph->metrics.width>>6;++pixel){if(face->glyph->bitmap.buffer[row*face->glyph->bitmap.pitch+pixel/8]&(0xC0>>(pixel%8))){if(row+_baseLine_offset+y_offset<64)dot_matrix[pixel+(x_offset)][row+_baseLine_offset+y_offset]=WHITE_POINT;}else{dot_matrix[pixel+(x_offset)][row+_baseLine_offset+y_offset]=BLACK_POINT;}}}if(face->glyph->bitmap.width==0)x_offset+=max_font_width/2;x_offset+=face->glyph->bitmap.width;}return 0;}int Draw_string_line1(char*_filename,int _font_size){int ret=Draw_string_line(_filename,_font_size,DEFAULT_Y_OFFSET);if(ret!=0)printf("Draw string fail(%d).\n",ret);}int Draw_string_line2(char*
_filename,int _font_size){int ret;ret=Draw_string_line(_filename,_font_size,DEFAULT_Y_OFFSET+DEFAULT_Y_OFFSET2);if(ret!=0)printf("Draw string fail(%d).\n",ret);}void freefont(){FT_Done_Face(face);FT_Done_FreeType(library);}
