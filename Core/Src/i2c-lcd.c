#include "i2c-lcd.h"
#include <stdint.h>
extern I2C_HandleTypeDef hi2c1;  // handle thiet lap

#define SLAVE_ADDRESS_LCD 0x4E // Dia chi thiet lap cua LCD

// Ham gui lenh den LCD gui 4 bit
void lcd_send_cmd (char cmd)
{
  char data_u, data_l;
	uint8_t data_t[4];
	data_u = (cmd&0xf0);
	data_l = ((cmd<<4)&0xf0);
	data_t[0] = data_u|0x0C;  //en=1, rs=0
	data_t[1] = data_u|0x08;  //en=0, rs=0
	data_t[2] = data_l|0x0C;  //en=1, rs=0
	data_t[3] = data_l|0x08;  //en=0, rs=0
	HAL_I2C_Master_Transmit (&hi2c1, SLAVE_ADDRESS_LCD,(uint8_t *) data_t, 4, 100);
}

// Ham gui du lieu den LCD gui 4 bit
void lcd_send_data (char data)
{
	char data_u, data_l;
	uint8_t data_t[4];
	data_u = (data&0xf0);
	data_l = ((data<<4)&0xf0);
	data_t[0] = data_u|0x0D;  //en=1, rs=1
	data_t[1] = data_u|0x09;  //en=0, rs=1
	data_t[2] = data_l|0x0D;  //en=1, rs=1
	data_t[3] = data_l|0x09;  //en=0, rs=1     
	HAL_I2C_Master_Transmit (&hi2c1, SLAVE_ADDRESS_LCD,(uint8_t *) data_t, 4, 100);
}

// Ham xoa man hinh
void lcd_clear (void)
{
	lcd_send_cmd (0x80);
	for (int i=0; i<70; i++)
	{
		lcd_send_data (' ');
	}
}

// Ham dat con tro tai vi tri cu the cua LCD
void lcd_put_cur(int row, int col)
{
    switch (row)
    {
        case 0:
            col |= 0x80; // 0x80 la vi tri bat dau dong 1
            break;
        case 1:
            col |= 0xC0; // 0xC0 la vi tri bat dau dong 2
            break;
    }

    lcd_send_cmd (col);
}

//Ham khoi tao LCD
void lcd_init (void)
{
	// Khoi tao 4 bit
	HAL_Delay(50);  
	lcd_send_cmd (0x30);
	HAL_Delay(5);  
	lcd_send_cmd (0x30);
	HAL_Delay(1);  
	lcd_send_cmd (0x30); // gui lenh 3 lan de chac chan LCD khoi dong va vao che do 8 bit
	HAL_Delay(10);
	lcd_send_cmd (0x20);  // Che do 4 bit
	HAL_Delay(10);

  // dislay initialisation
	lcd_send_cmd (0x28); // 00100100 - bit 2 F(Font) = 0 (Kich thuoc ky tu 5x8), bit 3 N(Number of lines) = 1 (hien thi 2 dong), bit 4 DL(data length) = 0 (che do 4 bit)
	HAL_Delay(1);
	lcd_send_cmd (0x08); //Dieu khien hien thi on/off --> D=0, C=0, B=0  ---> tat hien thi
	HAL_Delay(1);
	lcd_send_cmd (0x01);  // xóa màn hình
	HAL_Delay(1);
	HAL_Delay(1);
	lcd_send_cmd (0x06); // Cai dat che do nhap --> I/D=1 (tang con tro sang phai) & S=0 (không dich chuyen man hinh)
	HAL_Delay(1);
	lcd_send_cmd (0x0C); //Dieu khien hien thi on/off  --> D = 1, C = 0, B = 0. (Bat man hinh)
}

// Gui chuoi ki tu
void lcd_send_string (char *str)
{
	while (*str) lcd_send_data (*str++);
}

// Hàm chuyen doi so nguyen thanh chuoi ky tu
void display_integer_as_string(uint32_t adc_value) {
    char str[16]; 
    uint32_t temp = adc_value;
    uint8_t digit_count = 0;

    // Ðem so luong chu so cua so nguyên
    do {
        temp /= 10;
        digit_count++;
    } while (temp != 0);
    // Chuyen doi tu so nguyên sang chuoi ky tu
    for (uint8_t i = digit_count; i > 0; i--) {
        str[i - 1] = (adc_value % 10) + '0'; // trong ma ASCII '0' = 48;
        adc_value /= 10;
    }
    // Ket thuc chuoi ky tu NULL
    str[digit_count] = '\0';
    lcd_send_string(str);
}
// Hien thi % do am
void display_adc_percentage(uint32_t adc_value) {
    uint16_t max_in = 4095;
    uint16_t min_in = 0;
    uint8_t percentage = ((float)(max_in - adc_value) / (max_in - min_in)) * 100;

    // Hien thi phan tram tren man hinh LCD
    display_integer_as_string(percentage);
    
    // Gui ky tu '%' den man hinh LCD
    lcd_send_data('%');

    // Neu phan tram nho hon 10, thêm mot khoang trang
    if (percentage < 10) {
//        lcd_put_cur(1, 10); // Di chuyen con tro den hang 2, cot 10
        lcd_send_data(' '); // Gui mot khoang trang den LCD
    }
}



