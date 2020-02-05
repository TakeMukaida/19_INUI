#include "includes.h"
#include "coordinates.h" 
#include "gains.h"

// rotation
#define KP 0.6
#define KI 0.3
#define KD 0.001

DigitalIn sw(PC_4);
DigitalIn button(PB_12);

//DigitalOut myled1(PC_8);
//DigitalOut myled2(PC_6);
//DigitalOut myled3(PC_5);
DigitalOut led_getdata_pc(LED2);
//DigitalOut led_getdata_pc2(LED3);

I2C i2c(PB_7,PB_8);
//I2C i2c(PB_4,PA_8);

Serial pc(USBTX,USBRX);
Serial serial(PB_6,PA_10);
Serial gyro(PA_11,PA_12);

Ticker control_ticker;      // 制御周期生成用

PID pid(KP, KI, KD);
OLED oled(&i2c);
OMNI omni(&i2c, &pid, &oled);

char R6093U_data[24] = {0};
double R6093U_before = 0, R6093U_act_before = 0;

char can_data[3] = {0};

char pc_data[8], pc_data_buckup[8];

signed short distance_x = 0, distance_y = 0;
signed short log_distance_x = 0, log_distance_y = 0;
signed short phase = 0, log_phase = 0;
signed short sub_phase_blue[5] = {0};
signed short sub_phase_red[5] = {0};

bool color = false;
bool flags[7] = {false};

char cnt = 0;
char cnt_phase_b4 = 0;
char cnt_phase_r4 = 0;
int processing_go_ndone = 0;    // 処理開始/完了フラグ（0：完了/1：開始）

char moji[16] = {0};
int main(){
    
    init();
    
//    myled1 = 0;
//    myled2 = 0;
//    myled3 = 0;
    
//    char d = 0;
//    char decoration = 0;
//    char dec_cnt = 0;
//    char dec_color = 0;
    
    float z = 0;
    
//    short cnt_bathtowel = 0;
    
    float angle = 0;
//    float power = 0;
    float keep = 0;
    double pid_x = 0, pid_y = 0;
    
//    bool order[8] = {true};
    
    int test_phase_cnt = 0;
    
    while(1) {
        /*TODO: phaseの値がかわったときだけ送信
        test_phase_cnt++;
        if(test_phase_cnt >= 100) {
            phase++;
            if(phase > 4)
                phase = 0;
            pc.putc((char)(0xD0 | (phase & 0x0F)));
            test_phase_cnt = 0;
            
        }
        */
        
        z = R6093U_get(R6093U_data, 'Z');
        
        switch(color) {
    //***********BLUE ZONE***********//
        case 0:                                             
            switch(phase) {
            case 0:
                keep = 0;
                pid_x = pidy0.get_pid( distance_y, 263);
                pid_y = pidx0.get_pid(-distance_x, -50);
                angle = atan2(pid_x, pid_y)*180/M_PI;
                omni.omni(angle - z, sqrt(pow(pid_y,2)+pow(pid_x,2)), 0, z, keep);
                
                //TODO:バスタオル・シーツ回収
                /*
                移動と回収（一番めんどい）
                */
                
                break;
        
             case 1:
                keep = 0;
                pid_x = pidy0.get_pid( distance_x, -263);
                pid_y = pidx0.get_pid( distance_y, -95);
                angle = atan2(pid_x, pid_y)*180/M_PI;
                omni.omni(angle - z, sqrt(pow(pid_y,2)+pow(pid_x,2)), 0, z, keep);
                
                //TODO:バスタオル・シーツ排出
                /*
                移動と排出
                */
                
                break;
                
             case 2:
                keep = 0;
                pid_x = pidy0.get_pid( distance_y, 263);
                pid_y = pidx0.get_pid(-distance_x, -50);
                angle = atan2(pid_x, pid_y)*180/M_PI;
                omni.omni(angle - z, sqrt(pow(pid_y,2)+pow(pid_x,2)), 0, z, keep);
                
                //TODO:Tシャツ回収
                /*
                移動と回収
                */
                
                break;
                
             case 3:
                keep = 180;
                pid_y = pidy0.get_pid( distance_y, 263);
                pid_x = pidx0.get_pid(-distance_x, -50);
                angle = atan2(pid_x, pid_y)*180/M_PI;
                omni.omni(angle - z, sqrt(pow(pid_y,2)+pow(pid_x,2)), 0, z, keep);
                
                //TODO:Tシャツ排出
                /*
                移動と排出
                */
                
                break;
                
             case 4:
                keep = 0;
                pid_x = pidx0.get_pid(distance_y, -260);
                pid_y = pidy0.get_pid(distance_x, -380);
                angle = atan2(pid_x, pid_y)*180/M_PI;
                omni.omni(angle - z, sqrt(pow(pid_y,2)+pow(pid_x,2)), 0, z, keep);
                        
                //if((coordinate_b4[cnt_phase_b4][0] + distance_y) < 5)        //x座標の目標位置と現在位置の差が５cm以下なら
                 //   if((coordinate_b4[cnt_phase_b4][1] - distance_x) < 5)    //y座標の目標位置と現在位置の差が５cm以下なら
                   //     cnt_phase_b4++;
                
                if(cnt_phase_b4 > 6)
                    cnt_phase_b4 = 6;
                
                break;
                
            }   
            
        break;
 
    //***********RED ZONE***********//    
        case 1:             
            switch(phase) {
            case 0:
                keep = 0;
                pid_y = pidy0.get_pid( distance_y, 263);
                pid_x = pidx0.get_pid(-distance_x, -50);
                angle = atan2(pid_x, pid_y)*180/M_PI;
                omni.omni(angle - z, sqrt(pow(pid_y,2)+pow(pid_x,2)), 0, z, keep);
                
                //TODO:バスタオル・シーツ回収
                /*
                移動と回収（一番めんどい）
                */
                
                break;
        
             case 1:
                keep = 0;
                pid_y = pidy0.get_pid( distance_y, 263);
                pid_x = pidx0.get_pid(-distance_x, -50);
                angle = atan2(pid_x, pid_y)*180/M_PI;
                omni.omni(angle - z, sqrt(pow(pid_y,2)+pow(pid_x,2)), 0, z, keep);
                
                //TODO:バスタオル・シーツ排出
                /*
                移動と排出
                */
                
                break;
                
             case 2:
                keep = 0;
                pid_y = pidy0.get_pid( distance_y, 263);
                pid_x = pidx0.get_pid(-distance_x, -50);
                angle = atan2(pid_x, pid_y)*180/M_PI;
                omni.omni(angle - z, sqrt(pow(pid_y,2)+pow(pid_x,2)), 0, z, keep);
                
                //TODO:Tシャツ回収
                /*
                移動と回収
                */
                
                break;
                
             case 3:
                keep = 0;
                pid_y = pidy0.get_pid( distance_y, 263);
                pid_x = pidx0.get_pid(-distance_x, -50);
                angle = atan2(pid_x, pid_y)*180/M_PI;
                omni.omni(angle - z, sqrt(pow(pid_y,2)+pow(pid_x,2)), 0, z, keep);
                
                //TODO:Tシャツ排出
                /*
                移動と排出
                */
                
                break;
             /*   
             case 4:
                keep = 0;
                for(cnt = 0; cnt <= 6; cnt++) {
                    while(1) {
                        pid_x = pidx0.get_pid(-distance_y, -260);
                        pid_y = pidy0.get_pid( distance_x, -380);
                        angle = atan2(pid_x, pid_y)*180/M_PI;
                        omni.omni(angle - z, sqrt(pow(pid_y,2)+pow(pid_x,2)), 0, z, keep);
                        if((coordinate_b4[cnt][0] + distance_y) < 5)        //x座標の目標位置と現在位置の差が５cm以下なら
                            if((coordinate_b4[cnt][1] - distance_x) < 5)    //y座標の目標位置と現在位置の差が５cm以下なら
                                break;
                    }
                }
                break;
                */
            } 
            break;
        }
        
        //angle = atan2(pid_x, pid_y)*180/M_PI;
        
        if(sw || button){
            gyro.printf("$MIB,RESET*87");
        }
        else{
//           if(!color) {                                //blue
//                sprintf(moji, "p:%x", (short)phase, 
//            else {                                      //red
//                sprintf(moji, "p:%x f:%0

        }
        
        //sprintf(moji, "x:-%03d, y:-%03d", coordinate_b4[cnt][0], coordinate_b4[cnt][1]);
        //oled.OLED_printf(moji, 1);
        sprintf(moji, "%06.1f", angle);
        oled.OLED_printf(moji, 2);
        sprintf(moji, "x:%04d", (int)distance_x);
        oled.OLED_printf(moji, 5);
        sprintf(moji, "y:%04d", (int)distance_y);
        oled.OLED_printf(moji, 6);
        
        sprintf(moji, "d:%06.1f,c:%01d,f:%01d", z, color, phase);
        oled.OLED_printf(moji, 7);
        
        processing_go_ndone = 0;    // フラグを折って，制御処理完了
/*        while(processing_go_ndone == 0){
            //cnt = 0;
            //processing_go_ndone = processing_go_ndone;
            
            pc.puts("0");   //これがないとなぜかエラーが出る。
        }
*/        //led_getdata_pc = !led_getdata_pc;
    }
}

//****MAIN END****//

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
    if(pc_data[0] == 0x8B) {
        //led_getdata_pc = !led_getdata_pc;
        for(cnt = 1; cnt < 8; cnt++) {
            pc_data[cnt] = pc.getc();
            if(pc_data[cnt] >= 128)
                pc_data[cnt] = pc_data_buckup[cnt];
        }
    }
    else
        return;
    /*
    for(int i = 1; i < 8; i++){
        if(pc_data[i] >= 128){
            for(int j = 0; j < 8; j++){
                pc_data[j] = pc_data_buckup[j];
            }
        }
    }
    */
    //led_getdata_pc = !led_getdata_pc;
    //if(phase == 0) 
    //    led_getdata_pc = !led_getdata_pc;
    
    if(color == 0) 
        led_getdata_pc = 0;
    if(color >= 1) 
        led_getdata_pc = 1;
    
    log_distance_x = distance_x; 
    log_distance_y = distance_y;
    
    phase = pc_data[1] & 0x0F;
    //color = pc_data[1] >> 6;
    distance_x = (signed short)(((pc_data[2] & 0x0C) << 12) | ((pc_data[3] & 0x7F) << 7) | (pc_data[4] & 0x7F));
    distance_y = (signed short)(((pc_data[2] & 0x03) << 14) | ((pc_data[5] & 0x7F) << 7) | (pc_data[6] & 0x7F));
    //distance_x = (signed short)(((pc_data[3] << 7) & 0x07) | pc_data[4]);
    //distance_y = (signed short)(((pc_data[5] << 7) & 0x07) | pc_data[6]);
    
    // 10001000 0fffffff 0cpppppp 0000xxyy 0xxxxxxx 0xxxxxxx 0yyyyyyy 0yyyyyyy (to send header, f:flags, p:phase, x:distance_x, y:distance_y)
    
    if(abs(distance_x) >= 1000)
        distance_x = log_distance_x;
    if(abs(distance_y) >= 1000)
        distance_y = log_distance_y;
    
    if(color == 0 && phase == 4) {
        if(distance_x > 0)
            distance_x = log_distance_x;
        if(distance_y > 0)
            distance_y = log_distance_y;
    }
    
    return;
}

double R6093U_get(char data_arr[], char sel) {
    short angle = 0;
    double ang = 0;
    
    switch(sel){
        case 'X': angle = (data_arr[1] | (data_arr[2] << 8)); break;
        case 'Y': angle = (data_arr[3] | (data_arr[4] << 8)); break;
        case 'Z': angle = (data_arr[5] | (data_arr[6] << 8)); break;
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

void init() {
    gyro.baud(38400);
    pc.baud(115200);
    serial.baud(115200);

    oled.OLED_init();
    gyro.printf("$MIB,RESET*87");
//  rotary_encoder_ab_phase e(TIM2, 24);
//    e.start();
    
    serial.attach(can_reception, Serial::RxIrq);
    gyro.attach(gyro_handler, Serial::RxIrq);
    pc.attach(pc_handler, Serial::RxIrq);
    
    control_ticker.attach(&go_processing, CONTROL_CYCLE);   // 制御処理開始
}

// processing_go_ndone のフラグ立て割り込み関数
void go_processing() {
    if(processing_go_ndone == 0) {  // 処理が完了していたら
        //pc.printf("t\n");
        //sprintf(moji, "NP");
        //oled.OLED_printf(moji, 1);
        processing_go_ndone = 1;    // 次の処理開始
        //led_getdata_pc = 0;
    }
    else {                           // 処理が完了していなければ
        //pc.printf("f\n");      // 異常，警告文を出す
        //sprintf(moji, "ER");
        //oled.OLED_printf(moji, 1);
        //led_getdata_pc = 1;
        cnt = 0;
    }
}

