/*
 * lcd.h
 *
 *  Created on: Apr 2, 2019
 *      Author: Aakash
 */

#ifndef LCD_H_
#define LCD_H_

#ifdef __cplusplus
extern "C" {
#endif

#define LCD_BAUD_RATE   115200
#define LCD_RX_Q_SIZE   100
#define LCD_TX_Q_SIZE   100

// Define event ID
#define READ_OBJ        0x00
#define WRITE_OBJ       0x01
#define WRITE_STR       0x02

// Define object ID
#define WIN_BUTTON      0x06
#define ANGULAR_METER   0x07
#define COOL_GUAGE      0x08
#define FORM            0x0A
#define GUAGE           0x0B
#define LED             0x0E
#define LED_DIGITS      0x0F
#define METER           0x10
#define USER_LED        0x13
#define STATIC_TEXT     0x15
#define BUTTON_4D       0x1E

typedef enum
{
    menu,
    bridge,
    compass,
    sensor,
    motor,
    home,
    mia,
    gps
} lcd_screen_t;

typedef struct
{
    char event_id;
    char object_id;
    char object_num;
    char data_msb;
    char data_lsb;
    char checksum;
    char ack;
} lcd_data_packet_t;


void lcd_init(void);
void lcd_screen_query(void);
void lcd_put_comm(char event_id, char obj_id, char obj_num, char data_msb, char data_lsb);
void lcd_receive(void);
void lcd_get_screen_id(char screen_id);
void lcd_print();
void lcd_print_menu_screen();
void lcd_print_bridge_screen();
void lcd_print_gps_screen();
void lcd_print_motor_screen();
void lcd_print_sensor_screen();
void lcd_print_home_screen();
void lcd_print_mia_screen();
void lcd_print_compass_screen();
void lcd_put_string(char obj_num, char *data, int data_size);

#ifdef __cplusplus
}
#endif

#endif /* LCD_H_ */
