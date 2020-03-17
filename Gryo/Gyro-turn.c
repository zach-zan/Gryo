//
//  Gyro-turn.c
//  
//
//  Created by Zan on 2020/3/13.
//

double bias;
double conversion = 4.01; //这个值可以控制转向角度
int findBias(){
    //机器人静止，等待陀螺仪的值稳定
    ao();
    msleep(300);
    //Find the average noise
    int i = 0;
    double avg = 0;
    while(i < 50){
        avg += gyro_z();
        msleep(1);
        i++;
    }
    bias = avg / 50.0;
    return bias;
}
int readGyro(){
    return gyro_z() - bias ;
}
void turn(int angle, int speed){
    int targetAngle = angle*conversion;    //真实角度和变量数值转换
    int currentAngle = 0;
    int timeInterval = 10;
    double oldGyro ;
    double newGyro = readGyro();
    while(abs(currentAngle) <= targetAngle){
        oldGyro = newGyro;
        if(currentAngle<targetAngle){
            mav(0,-speed);
            mav(2,speed);
        }
        
        else{
            mav(0,speed);
            mav(2,-speed);
        }
        msleep(timeInterval);
        newGyro = readGyro();
        //角度=陀螺仪的数值*时间，下方的思路是陀螺仪变化的增长量*时间+上一次陀螺仪的值*时间，等于当前角度的值
        currentAngle += 0.5*(newGyro-oldGyro)*timeInterval/1000 + oldGyro * timeInterval/1000 ;
    }
}

