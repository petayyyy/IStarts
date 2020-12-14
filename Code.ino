#include <Ps3Controller.h>
#include <ESP32_Servo.h>
#include <GyverStepper.h>

GStepper<STEPPER4WIRE> Motor1(2048, 5, 3, 4, 2);
GStepper<STEPPER4WIRE> Motor2(2048, 11, 10, 9, 8);
Servo Zachvat;
Servo Povorot;

int Left_y,Right_x;
int Pose_Open = 0;
int Pose_Close = 90;
int Pose = 0;
int pos = 0;
int last_pos = 0;
int last_Pose = 0;

void setup()
{
    Serial.begin(115200);

    Zachvat.attach(6);
    Povorot.attach(7);
    Ps3.begin("8c:7c:b5:0a:d6:f9");
    Serial.println("Controller PS3 Ready.");

    Zachvat.write(Pose_Close);
    Povorot.write(Pose);
    Motor1.setRunMode(KEEP_SPEED); // режим поддержания скорости
    Motor1.setSpeedDeg(80);
    Motor2.setRunMode(KEEP_SPEED); // режим поддержания скорости
    Motor2.setSpeedDeg(80);
    Serial.println("Zachvat Ready.");
}

void loop()
{
    if(Ps3.isConnected()){
        Left_y = Ps3.data.analog.stick.ly;
        Right_x = Ps3.data.analog.stick.rx;
        Serial.print("Left_y "+String(Left_y));
        Serial.println("  Right_x "+String(Right_x));
  
        //---------------- Вращение клешни ---------------
        if (Right_x > 10){
          if (Pose != 180){
          Pose += 1;}
        }else if (Right_x < -10){
          if (Pose != -180){
          Pose -= 1;}
        }        
        Povorot.write(Pose);
        
        //---------------- Контроль высоты ---------------
        if (Left_y > 10){
          pos = -400;
        }else if (Left_y < -10){
          pos = 400;
        }else{
          pos = 0;
        }
        Motor1.setSpeed(pos);
        Motor2.setSpeed(pos);
        
        //---------------- Открытие клешни ----------
         if( abs(Ps3.event.analog_changed.button.l1)){
             Serial.println("Pose_Open "+String(Pose_Open));

         }         
        //---------------- Закрытие клешни ----------
         if( abs(Ps3.event.analog_changed.button.r1) ){
             Serial.println("Pose_Close "+String(Pose_Close));
    
         }           
        //--------------- Шаговое управление поворотом --------------
         if( abs(Ps3.event.analog_changed.button.right) ){
              Serial.println(Ps3.data.analog.button.right, DEC);
              if (Ps3.data.analog.button.right > 130 && last_Pose == 0) {
                  if (Pose != 180){
                      Pose += 0.1;}
                  last_Pose = 1;
         }
         else{
                last_Pose = 0;  }}

         if( abs(Ps3.event.analog_changed.button.left)){
              Serial.println(Ps3.data.analog.button.left, DEC);
              if (Ps3.data.analog.button.left > 130 && last_Pose == 0) {
                  if (Pose != -180){
                      Pose -= 0.1;} 
                  last_Pose = 1; }
         else{
                last_Pose = 0;  }}
         
         Povorot.write(Pose);
         Serial.println("Pose "+String(Pose));
         Serial.println("pos "+String(pos));
         Serial.println();}
           
    Motor1.tick();
    Motor2.tick();
    delay(10);
}
