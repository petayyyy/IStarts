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
  
        //---------------- Вращение клешни ---------------
        if (Right_x > 0){
          Pose += 1;
        }else if (Right_x < 0){
          Pose -=1;
        }
        Povorot.write(Pose);
        
        //---------------- Контроль высоты ---------------
        if (Left_y > 0){
          pos = -400;
        }else if (Left_y < 0){
          pos = 400;
        }else{
          pos = 0;
        }
        Motor1.setSpeed(pos);
        Motor2.setSpeed(pos);
        
        //---------------- Открытие клешни ----------
         if( abs(Ps3.event.analog_changed.button.l1)){
             //Serial.print("Pressing the left shoulder button: ");
             //Serial.println(Ps3.data.analog.button.l1, DEC);
             Zachvat.write(Pose_Open);        
         }
         //---------------- Закрытие клешни ----------
         if( abs(Ps3.event.analog_changed.button.r1) ){
             //Serial.print("Pressing the right shoulder button: ");
             //Serial.println(Ps3.data.analog.button.r1, DEC);
             Zachvat.write(Pose_Close);    
         }
           
         //--------------- Шаговое управление высотой --------------
         if( Ps3.event.button_down.up ){
             //Serial.println("Started pressing the up button");
             if( Ps3.event.button_up.up && pos == 0){
                //Serial.println("Released the up button");
                pos = -400;}
         }else if( Ps3.event.button_down.down ){
             //Serial.println("Started pressing the down button");
             if( Ps3.event.button_up.down && pos == 0){
                //Serial.println("Released the down button");
                pos = -400;}
         }else{
             pos = 0;
         }
         Motor1.setSpeed(pos);
         Motor2.setSpeed(pos);
         
         //--------------- Шаговое управление поворотом --------------
         if( Ps3.event.button_down.right )
             //Serial.println("Started pressing the right button");
         if( Ps3.event.button_up.right )
            //Serial.println("Released the right button");
            Pose += 1;
         if( Ps3.event.button_down.left )
             //Serial.println("Started pressing the left button");
         if( Ps3.event.button_up.left )
             //Serial.println("Released the left button");
             Pose -= 1;
         Povorot.write(Pose);}
           
    Motor1.tick();
    Motor2.tick();
    delay(10);
}
