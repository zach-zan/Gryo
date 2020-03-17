//
//  Gyro_Move.c
//  Gyrotest2
//
//  Created by Zan on 2020/3/13.
//  Copyright © 2020 Zan. All rights reserved.
//


#include <kipr/botball.h>
double bias;
int rightSpeed, leftSpeed;
double devAngle;
int speed = 800 ;
double targetAngle = 0;
double currentAngle = 0;
double oldAngle = 0;
double newGyro ;
double oldGyro ;
int timeInterval = 10;
//找到陀螺仪的偏差值
int findBias(){
    ao();
    msleep(1000);
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
// 校准陀螺仪的数值
int readGyro(){
    return gyro_z() - bias;
}
int main()
{
    findBias();
    readGyro();
    oldGyro = gyro_z();
    newGyro = readGyro();
    targetAngle = readGyro()*timeInterval/1000;    //确定一个目标角度，是当前陀螺仪测定的角度，依次作为目标参照
    while(digital(0)!=1)
    {
        devAngle = currentAngle - oldAngle;       //偏差角度是当前角度-上一次测量角度
        oldGyro = newGyro;
        newGyro = readGyro();
        
        //纠偏速度是基础速度 +- 偏差角度*比例
        rightSpeed = speed - currentAngle*100;
        leftSpeed = speed + currentAngle*100;
        
        //判断偏移角度和目标角度
        if(devAngle > targetAngle)
        {
            
            mav(2,rightSpeed);
            mav(0,speed-100);
        }
        if (devAngle < targetAngle)
        {
            
            mav(2,speed-100);
            mav(0,leftSpeed);
        }
        
        mav(2,speed);
        mav(0,speed);
        //偏移角度的比较
        oldAngle = currentAngle ;
        currentAngle += (newGyro-oldGyro)*timeInterval/1000 + oldGyro * timeInterval/1000;
        
    }
    return 0 ;
}
