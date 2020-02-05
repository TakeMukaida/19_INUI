/*
#include "includes.h"
#include "gains.h"

// rotation
#define KP 0.6
#define KI 0.3
#define KD 0.001

DigitalIn sw(PC_4);
DigitalIn button(PB_12);

DigitalOut myled1(PC_8);
DigitalOut myled2(PC_6);
DigitalOut myled3(PC_5);

I2C i2c(PB_7,PB_8);
I2C i2c2(PB_4,PA_8);

Serial pc(USBTX,USBRX);
Serial serial(PB_6,PA_10);
Serial gyro(PA_11,PA_12);

PID pid(KP, KI, KD);
OLED oled(&i2c);
OMNI omni(&i2c, &pid, &oled);

char R6093U_data[24] = {0};
double R6093U_before = 0, R6093U_act_before = 0;

char can_data[3] = {0};

char pc_data[8], pc_data_buckup[8];

signed short distance_x = 0, distance_y = 0;
signed short phase = 0, log_phase = 0;
signed short sub_phase_blue[5] = {0}
signed short sub_phase_red[5] = {0}

bool color = false;
bool flags[7] = {false};

char cnt;

int main(){
    gyro.baud(38400);
    pc.baud(19200);
    serial.baud(115200);

    oled.OLED_init();
    gyro.printf("$MIB,RESET*87");
    rotary_encoder_ab_phase e(TIM2, 24);
    e.start();
    
    serial.attach(can_reception, Serial::RxIrq);
    gyro.attach(gyro_handler, Serial::RxIrq);
    pc.attach(pc_handler, Serial::RxIrq);
    
    myled1 = 0;
    myled2 = 0;
    myled3 = 0;
    
    char d = 0;
    char moji[16] = {0};
    char decoration = 0;
    char dec_cnt = 0;
    char dec_color = 0;
    
    float z = 0;
    
    short cnt_bathtowel = 0;
    
    float angle = 0;
    float power = 0;
    float keep = 0;
    double pid_x = 0, pid_y = 0;
    
    bool order[8] = {true};
    
    //can_transmission(INTAKE, 0x03);
    
    while(!sw && !button){
    //char ph = 1;
    //while(1){
        i2c.write(LED_ADDRESS, &decoration, 1, 0);
        if(decoration == 50){
            dec_cnt = 0;
        }
        else if(decoration == 0){
            dec_cnt = 1;
            dec_color = !dec_color;
        }
        else if((signed char)decoration == -50){
            dec_cnt = 0;
        }
        if(dec_color){
            dec_cnt ? decoration += 5 : decoration -= 5;
        }
        else{
            dec_cnt ? decoration -= 5 : decoration += 5;
        }
        z = R6093U_get(R6093U_data, 'Z');
        sprintf(moji, "%7.2f", z);
        oled.OLED_printf(moji, 1);
        sprintf(moji, "p:%d", phase);
        oled.OLED_printf(moji, 2);
        omni.only_disp_omni(0, 0, 0, z, 0);
        d = e.get_counts();
        oled.OLED_bar(0, 7, (char)d);
        oled.OLED_line_clear(7, (char)d);
//        if(sw){
//            uart_sending(0x01, ph);
//            uart_recv_data = 5;
//            ph++;
//            if(ph == 4) ph = 1;
//            wait_ms(1000);
//        }
    }
    
    while(1) {
        
        z = R6093U_get(R6093U_data, 'Z');
        
        switch(color) {
    //***********BLUE ZONE***********
        case 0:                                             
            switch(phase) {
            case 0:
                keep = 0;
                //TODO:バスタオル・シーツ回収
                
                break;
        
             case 1:
                
                //TODO:バスタオル・シーツ排出
                
                break;
                
             case 2:
            
                //TODO:Tシャツ回収
                
                break;
                
             case 3:
            
                //TODO:Tシャツ排出
                
                break;
                
             case 4:
                pid_y = pidy0.get_pid( distance_y, 263);
                pid_x = pidx0.get_pid(-distance_x, -50);
                angle = atan2(pid_x, pid_y)*180/M_PI;
                omni.omni(angle - z, sqrt(pow(pid_y,2)+pow(pid_x,2)), 0, z, keep);
            
                //TODO:Tシャツ掛け
                
                break;
            }   
            
            case  0:                                            //ランダムエリア始端へ移動
                if(z > -80) {
                    omni.omni(0, 100, -90, z, (int)z);
                }
                else {
                    keep = -90;
                    if(distance_y > 250) {
                        pid_y = pidy0.get_pid( distance_y, 50);    //50は適当な値,(250-50)
                        pid_x = pidx0.get_pid(-distance_x, 60);
                    }
                    else {
                        pid_y = pidy0.get_pid( distance_y, 120);    //ランダムエリア始端700cmから少し隙間を開ける
                        pid_x = pidx0.get_pid(-distance_x,  45);    //x幅41.5cmから少し隙間を開ける
                    }    
                    angle = atan2(pid_x, pid_y)*180/M_PI;
                    power = sqrt(pow(pid_x,2)+pow(pid_y,2));
                    omni.omni(angle - z, power, 0, z, (int)keep);
                }
                break;
    
            case  1:                                            //画像処理(bath_towel)
                keep = -90;
                
                if(flags[6]) {                                  //取込モジュール
                    do {
                        omni.omni(0, 0, 0, z, (int)keep);
                        can_transmission(INTAKE, 0x01);     //1回目
                        while(can_data[0] != 0x01);
                        can_transmission(INTAKE, 0x01);     //2回目
                        while(can_data[0] != 0x01);
                        can_data[0] = 0x00;                 //初期化
                    } while(flags[6]);                      //未回収ならばもう一度
                    cnt_bathtowel++;
                }
                
                if(cnt_bathtowel < NUMBER_OF_TOWELS) {
                    pid_y = -10;
                    pid_x = pidx1.get_pid(-distance_x, 45);
                    angle = atan2(pid_x, pid_y)*180/M_PI;
                    power = sqrt(pow(pid_x, 2)+pow(pid_y,2));
                    omni.omni(angle - z, power, 0, z, (int)keep);
                }
                else {
                    pid_y = pidy1.get_pid( distance_y, 285);
                    pid_x = pidx1.get_pid(-distance_x,  45);
                    angle = atan2(pid_x, pid_y)*180/M_PI;
                    power = sqrt(pow(pid_x,2)+pow(pid_y,2));
                    omni.omni(angle - z, power, 0, z, (int)keep);
                }
                break;
    
            case  2:                                    //画像処理(sheets)
                keep = -90;
                if(flags[6]) {
                    omni.omni(0, 0, 0, z, (int)keep);
                    can_transmission(INTAKE, 0x01);     //1回目
                    while(can_data[0] != 0x01 && (phase == 2));
                    can_transmission(INTAKE, 0x01);     //2回目
                    while(can_data[0] != 0x01 && (phase == 2));
                    can_data[0] = 0x00;                 //初期化
                }
                break;
                
            case  3:
                keep = -90;
                if(-distance_x < -315) {
                    pid_y = pidy3.get_pid( distance_y,  240);
                    pid_x = pidx3.get_pid(-distance_x, -240);
                }
                else {
                    pid_y = pidy3.get_pid( distance_y, 230);
                    pid_x = pidx3.get_pid(-distance_x, -50);
                }
                angle = atan2(pid_x, pid_y)*180/M_PI;
                power = sqrt(pow(pid_x,2)+pow(pid_y,2));
                omni.omni(angle - z, power, 0, z, (int)keep);
                
                if(distance_x > 60 && distance_y < 240)
//                    transmission_hub(INTAKE, 0x02);
                break;
        
            case  4:
                if(-distance_x < 100)
                    distance_x = -300; 
                if(!order[0])
                    order[0] = true;
                    
                keep = 0;
                pid_y = pidy4.get_pid( distance_y, 80);
                if(phase == 4)                                  
                    pid_x = pidy4.get_pid(-distance_x, 200);
                else                                            
                    pid_x = pidy4.get_pid(-distance_x, 170);
                angle = atan2(pid_x, pid_y)*180/M_PI;
                omni.omni(angle - z, sqrt(pow(pid_x,2)+pow(pid_y,2)), 0, z, keep);
                break;
            }
                
                   
            case  5:
            case  7:
                if(-distance_x < 130)
                    distance_x -= 100;
                if(distance_y <= 160)
                    order[0] = false;
                
                keep = 0;
                if(order[0])                                    //投下
                    pid_y = pidy5.get_pid( distance_y, 140);
                else                                            //撤退
                    pid_y = pidy5.get_pid( distance_y, 260);
                if(phase == 5)                                  
                 pid_x = pidy5.get_pid(-distance_x, 200);
                else                                            
                    pid_x = pidy5.get_pid(-distance_x, 170);
                angle = atan2(pid_x, pid_y)*180/M_PI;
                omni.omni(angle - z, sqrt(pow(pid_x,2)+pow(pid_y,2))/2, 0, z, keep);
                break;  
            
            
        break;
 
    //***********RED ZONE**********         
        case 1:             
            switch(phase) {
            case  0:                                                            //ランダムエリア始端へ移動
                pid_y = pidy0.get_pid( distance_y, 263);
                pid_x = pidx0.get_pid(-distance_x, -50);
                angle = atan2(pid_x, pid_y)*180/M_PI;
                omni.omni(angle - z, sqrt(pow(pid_y,2)+pow(pid_x,2)), 0, z, keep);
                break;
    
            case  1:                                                            //画像処理(bath_towel)
                if(flags[6]) {
                    omni.omni(0, 0, 0, z, 0);
//                    transmission_hub(INTAKE, 0x01);
                    while(can_data[0] != 0x01);             //数値訂正の必要有
                    can_data[0] = 0x00;
                    if(!flags[6])
                        cnt_bathtowel++;
                }
                
                if(cnt_bathtowel < 3) {
                    pid_x = pidx1.get_pid(-distance_x, -50);
                    angle = atan2(pid_x, 10)*180/M_PI;
                    omni.omni(angle - z, sqrt(pow(pid_y,2)+pow(pid_x,2)), 0, z, keep);
                }
                else {
                    pid_y = pidy1.get_pid( distance_y, 263);
                    pid_x = pidx1.get_pid(-distance_x, -50);
                    angle = atan2(pid_x, pid_y)*180/M_PI;
                    omni.omni(angle - z, sqrt(pow(pid_y,2)+pow(pid_x,2)), 0, z, keep);
                }
                break;
    
            case  2:                                                            //画像処理(sheets)
                if(flags[6]) {
                    omni.omni(0, 0, 0, z, 0);
//                    transmission_hub(INTAKE, 0x01);
                    while(can_data[0] != 0x01);             //数値訂正の必要有
                    can_data[0] = 0x00;
                }
                break;
              
            case  3:
            case  8:
                if(-distance_x < 315) {
                    pid_y = pidy3.get_pid( distance_y, 210);
                    pid_x = pidx3.get_pid(-distance_x, 215);
                }
                else {
                    pid_y = pidy3.get_pid( distance_y, 230);
                    pid_x = pidx3.get_pid(-distance_x,  50);
                }
                angle = atan2(pid_x, pid_y)*180/M_PI;
                omni.omni(angle - z, sqrt(pow(pid_y,2)+pow(pid_x,2)), 0, z, keep);
                
                 if(distance_x < -60 && distance_y < 240)
//                    transmission_hub(INTAKE, 0x02);
                break;
         
            case  4:
            case  6:
                if(!order[4])                               //case5,7用準備
                    order[4] = true;
                if(-distance_x < -100)
                    distance_x = 300;
                    
                pid_y = pidy4.get_pid( distance_y, 80);
                if(phase == 4)                                  
                    pid_x = pidy4.get_pid(-distance_x, -200);
                else                                            
                    pid_x = pidy4.get_pid(-distance_x, -170);
                angle = atan2(pid_x, pid_y)*180/M_PI;
                omni.omni(angle - z, sqrt(pow(pid_y,2)+pow(pid_x,2)), 0, z, keep);
                break;
                
            case  5:
            case  7:
                if(-distance_x < -130)
                    distance_x += 100;
                if(distance_y <= 160)
                    order[4] = false;
                
                if(order[4])                                    //投下
                    pid_y = pidy5.get_pid( distance_y, 140);
                else                                            //撤退
                    pid_y = pidy5.get_pid( distance_y, 260);
                if(phase == 5)                                  
                    pid_x = pidy5.get_pid(-distance_x, 200);
                else                                            
                    pid_x = pidy5.get_pid(-distance_x, 170);
                angle = atan2(pid_x, pid_y)*180/M_PI;
                omni.omni(angle - z, sqrt(pow(pid_y,2)+pow(pid_x,2)), 0, z, keep);
                break;   
                
            case  9: //往路
                if(z < -10){
                        omni.omni(0, 126, 0, z, z);
                }
                else if(z > 10){
                        omni.omni(0, 126, 0, z, z);
                }
                else {
                    if(order[1]) 
                        distance_y = 300;    
                    if(distance_y < 300)
                        order[1] = false;   
                    keep = 0;
                    pid_y = pidy15.get_pid( distance_y, 250);
                    pid_x = pidx15.get_pid(-distance_x,  60);
                    angle = atan2(pid_x, pid_y)*180/M_PI;
                    omni.omni(angle - z, sqrt(pow(pid_y,2)+pow(pid_x,2)), 0, z, keep);
                }
                break;
     
            case 10:                        //phase10以降dx,dyが逆になる
                if(z > -80){
                    omni.omni(-90, 80, -90, z, z);
                }
                else if(z <= -100){
                    omni.omni(-90, 80, 90, z, z);
                }
                else {
                    keep = -90;
                    if(distance_x < -140)        //後段土台手前検出
                        distance_x -= 170;
                    pid_y = pidy10.get_pid(distance_x, -250);
                    pid_x = pidx10.get_pid(distance_y,  200);
                    angle = atan2(pid_y, pid_x)*180/M_PI;
                    omni.omni(angle - z, sqrt(pow(pid_y,2)+pow(pid_x,2)), 0, z, keep);
                }
                break;
            
            case 11:
                if(z > -80){
                    omni.omni(-90, 80, -90, z, z);
                }
                else if(z <= -100){
                    omni.omni(-90, 80, 90, z, z);
                }
                else {
                    keep = -90;
                    pid_y = pidy11.get_pid(distance_x, -150);
                    pid_x = pidx11.get_pid(distance_y,   15);
                    angle = 90 - atan2(pid_x, pid_y)*180/M_PI;
                    omni.omni(angle - z, sqrt(pow(pid_y,2)+pow(pid_x,2)), 0, z, keep);
                }
                break;
            
            case 12: //シーツ投下
                if(z > -80){
                    omni.omni(-90, 80, -90, z, z);
                }
                else if(z <= -100){
                    omni.omni(-90, 80, 90, z, z);
                }
                else {
                    keep = -90;
                    pid_y = pidy12.get_pid(distance_x, -150);
                    pid_x = pidx12.get_pid(distance_y,  185);//
                    angle = 90 - atan2(pid_x, pid_y)*180/M_PI;
                    omni.omni(angle - z, sqrt(pow(pid_y,2)+pow(pid_x,2)), 0, z, keep);
                }
                
//                transmission_hub(ROLLER, (abs(distance_x - 15) & 0x7F));
//                transmission_hub(ROLLER, ((abs(distance_x - 15) >> 7) & 0x7F) + 0x80);
                
                serial.putc((abs(distance_x-60) & 0x7F) | 0x00);
                serial.putc(((abs(distance_x-60) >> 7) & 0x7F) + 0x80);
                
                break;
            
            case 13:                                        //現在凍結中
            
                if(distance_y < 150)//
                    order[5] = false;
                    
                keep = 90;
                pid_y = pidy13.get_pid(distance_x, 150);
                if(order[5])
                    pid_x = pidx13.get_pid(distance_y, 140);         //待機
                else
                    pid_x = pidx13.get_pid(distance_y, 185);         //実行
                angle = 90 - atan2(pid_x, pid_y)*180/M_PI;
                omni.omni((int)angle - (int)z, sqrt(pow(pid_x,2)+pow(pid_y,2)), 0, (int)z, (int)keep);
            
                break;
    //end_flag            
            case 14:
                if(distance_x < 245 && order[6])            //後段土台手前長辺検出
                    distance_y = 130;
                else 
                    order[6] = false;
                if(distance_x < 100)                        //後段土台手前短辺検出
                    distance_x += 170;
                
                keep = 90;
                pid_y = pidy14.get_pid(distance_x, 250); 
                pid_x = pidx14.get_pid(distance_y,  80);
                angle = 90 - atan2(pid_x, pid_y)*180/M_PI;
                omni.omni(angle - z, sqrt(pow(pid_y,2)+pow(pid_x,2)), 0, z, keep);
                break;
            
            case 15: //復路
                if(z < 180){
                    omni.omni(90, 80, 180, z, z);
                }
                else if(z >= 190){
                    omni.omni(90, 80, -180, z, z);
                }
                else {
                    if(order[7]) 
                        distance_y = 300;    
                    if(distance_y < 300)
                        order[7] = false;   
                    keep = 180;
                    pid_y = pidy15.get_pid( distance_y, 20);
                    pid_x = pidx15.get_pid(-distance_x, 60);
                    angle = atan2(pid_x, pid_y)*180/M_PI;
                    omni.omni(angle - z, sqrt(pow(pid_y,2)+pow(pid_x,2)), 0, z, keep);
                }
                break;
                
            case 20: // roller testing module
                for(cnt = 4; cnt <= 7; cnt++)
                    if(!order[cnt])
                        order[cnt] = true;
                    
                keep = 0;
                angle = 90;
                if(-distance_x > 200)
                    omni.omni(angle - z, 50, 0, z, keep);
               
//                transmission_hub(ROLLER, (abs(distance_x - 15) & 0x7F));
//                transmission_hub(ROLLER, ((abs(distance_x - 15) >> 7) & 0x7F) + 0x80);
                /*
                serial.putc((abs(distance_x-60) & 0x7F) | 0x00);
                serial.putc(((abs(distance_x-60) >> 7) & 0x7F) + 0x80);
                */
                
                serial.putc((d & 0x7F) | 0x00);
                serial.putc(((d >> 7) & 0x7F) | 0x80);
            
                sprintf(moji, "dx:%05d", abs(distance_x-60));
                oled.OLED_printf(moji, 7);
                
                break;
            }
            break;
        }
    
    
        
//        sprintf(moji, "%f", angle);
//        oled.OLED_printf(moji, 4);
//        sprintf(moji, "%d", (int)distance_y);
//        oled.OLED_printf(moji, 5);
        
//        z = R6093U_get(R6093U_data, 'Z');
//        angle = atan2(pid_x, pid_y)*180/M_PI;
//        omni.omni((int)angle, sqrt(pow(pid_x,2)+pow(pid_y,2)), 0, (int)z, 0);
        
        if(sw || button){
//            transmission_hub(INTAKE, 0x00);       
            myled1 = 1;
            myled2 = !button;
            myled3 = !sw;
            gyro.printf("$MIB,RESET*87");
        }
        else{
//            gotou_phase = (d/4)%5;
            
            myled1 = ((d%12)/4 == 0);
            myled2 = ((d%12)/4 == 1);
            myled3 = ((d%12)/4 == 2);
            if(!color) {                                //blue
                sprintf(moji, "p:%x f:%02d o:%04d", 
                (short)phase, 
                (short)flags[0] + (short)flags[6]*10,
                (short)order[0] + (short)order[1]*10 + (short)order[2]*100 + (short)order[3]*1000);
            }
            else {                                      //red
                sprintf(moji, "p:%x f:%02d o:%04d", 
                (short)phase, 
                (short)flags[0] + (short)flags[6]*10,
                (short)order[4] + (short)order[5]*10 + (short)order[6]*100 + (short)order[7]*1000);
            }
            
            oled.OLED_printf(moji, 7);
        }
        
        oled.OLED_bar(0, 0, (char)d);                                           //以下文字表示部
        oled.OLED_line_clear(0, (char)d);
        sprintf(moji, "%03d", d);
        oled.OLED_printf(moji, 1);
        d = e.get_counts();
        
        oled.OLED_bar(0, 2, (char)(abs(z)/180*128));
        oled.OLED_line_clear(2, (char)(abs(z)/180*128));
        sprintf(moji, "d:%04.1f, c:%01d", z, color);
        oled.OLED_printf(moji, 3);
        
        sprintf(moji, "x:%4d y:%4d", (int)distance_x, (int)distance_y);
        oled.OLED_printf(moji, 6);
    }
}

//****MAIN END***

void can_transmission(char address, char data) {
    char transmission[4] = {0};
    
    transmission[0] = P_HEADER;
    transmission[1] = S_HEADER;
    transmission[2] = address;
    transmission[3] = data;
    
    for(cnt = 0; cnt <= 3; cnt++)
        serial.putc(transmission[cnt]);
}

void can_reception() {                      //data:aadd dddd
    char reception = 0;                 
    
    if(serial.getc() == P_HEADER){
        if(serial.getc() == S_HEADER){
            reception = serial.getc(); 
            
            if((reception >> 6) >= 0 && (reception >> 6) <= 2)
                can_data[reception >> 6] = (reception & 0x3F);
        }
        else
            return;
    }
    else
        return;
}

void gyro_handler(){
    if(gyro.getc() == 0xA6){
        if(gyro.getc() == 0xA6){
            for(int cnt = 0; cnt < 7; cnt++){
                R6093U_data[cnt] = gyro.getc();
            }
        }
        else
            return;
    }
    else
        return;
}

void pc_handler(){
    for(int i = 0; i < 8; i++){
        pc_data_buckup[i] = pc_data[i];
    }
    pc_data[0] = pc.getc();
    if(pc_data[0] >= 0x80) {
        for(int cnt = 1; cnt < 8; cnt++)
            pc_data[cnt] = pc.getc();
    }
    else
        return;
    
    for(int i = 1; i < 8; i++){
        if(pc_data[i] >= 128){
            for(int j = 0; j < 8; j++){
                pc_data[j] = pc_data_buckup[j];
            }
        }
    }
    
    for(cnt = 0; cnt < 7; cnt++)
        flags[cnt] = (pc_data[1] >> cnt) & 0x01;
    phase = pc_data[2] & 0x3F;
    color = pc_data[2] >> 6;
    distance_x = (short)(((pc_data[3] & 0x0C) << 12) + (pc_data[4] << 7) + pc_data[5]);
    distance_y = (short)(((pc_data[3] & 0x03) << 14) + (pc_data[6] << 7) + pc_data[7]);
    
    // 10001000 0fffffff 0cpppppp 0000xxyy 0xxxxxxx 0xxxxxxx 0yyyyyyy 0yyyyyyy (to send header, f:flags, p:phase, x:distance_x, y:distance_y)
    
    return;
}

double R6093U_get(char data_arr[], char sel) {
    float angle = 0;
    double ang = 0;
    
    switch(sel){
        case 'X': angle = data_arr[1] | (data_arr[2] << 8); break;
        case 'Y': angle = data_arr[3] | (data_arr[4] << 8); break;
        case 'Z': angle = data_arr[5] | (data_arr[6] << 8); break;
        default : angle = 0; break;
    }
    
    ang = (double)angle * 0.01;
    if((abs(ang)-abs(R6093U_act_before)) > 40){
        R6093U_act_before = ang;
        ang = R6093U_before;
    }
    else{
        R6093U_act_before = ang;
    }
    R6093U_before = ang;
    
    return ang;
}
*/
