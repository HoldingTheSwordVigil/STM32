#include "u8g2.h"

//u8g2_t *u8g2;                  /*u8g2图形库结构体指针*/
//const u8g2_cb_t *rotation;     /*u8g2图形库ui旋转：U8G2_R0(无旋转),U8G2_R1(顺时针90°),U8G2_R2(顺时针180°)......*/
//u8x8_msg_cb byte_cb;           /*u8g2图形库字节交互回调函数*/
//u8x8_msg_cb gpio_and_delay_cb; /*u8g2图形库gpio和延时回调函数*/

void u8g2_Setup_ssd1306_i2c_128x64_noname_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_ssd1306_128x64_noname, u8x8_cad_ssd13xx_fast_i2c, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_16_8_f(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
