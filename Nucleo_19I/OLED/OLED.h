#pragma once

#define OLED_ID 0x3C<<1

class OLED
{
private:
    char a;
    char data[6];
    I2C *myi2c;

public:
    OLED(I2C *instance);

    void OLED_clear(int);
    void OLED_init();
    void OLED_bar(char, char, char);
    void OLED_disp(char*, char, char);
    void OLED_disp_multi(char, char, char, char);
    void OLED_line_clear(char, char);
    void Misaki_Ascii(char, char, char );
    void OLED_printf(char moji[], char);

    ~OLED();
};
