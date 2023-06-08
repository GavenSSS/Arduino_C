 /**
*************************************************************************************************
* @author         沁和智能--qhebot
* @version        V2.0
* @note    ´      https://qhebot.taobao.com/
*************************************************************************************************
*/
#include <SoftwareSerial.h>
#include <Wire.h> 
#include "LiquidCrystal_I2C.h"
#include "QhDHT11.h"
#include "IRremote.h"

SoftwareSerial mySerial(4, 5);  

#define  DHT11_PIN       A1   
#define  WETPIN          A0   
#define  LED_GREEN_PIN   6
#define  LED_RED_PIN     7
#define  MOTOR_INA1      3
#define  MOTOR_INA2      8
#define  Water_Pump_ON   0X01
#define  Water_Pump_OFF  0X02

//湿度打开关闭水泵阈值
#define LOW_TH 30
#define HIGH_TH (LOW_TH+5)

int pump_sta = 0;  //0代表水泵已经关闭 1代表水泵已经开启
float wetValue = 0;   //土壤湿度值
int serialData;

LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display
QhDHT11 qhDHT;  

void setup() 
{
  Serial.begin(9600);
  mySerial.begin(9600);
  led_init();
    
  lcd.init();                      // initialize the lcd
  lcd.backlight();
  lcd.setCursor(2,0); // go to start of 2nd line
  lcd.print("Hello, world");
  lcd.setCursor(4,1); // go to start of 2nd line
  lcd.print("QHEBOT");
  
  pumpInit();
  
  Serial.println("Hello , QHEBOT");
}

unsigned char green_led_sta = 0;
unsigned long int time_cnt = 0;

void loop() 
{
  if( millis() - time_cnt >= 500 )
  {
    time_cnt = 0;
    getSensor();               //得到传感器数据
    update_lcd1602();          //更新1602液晶数据
  }
    
  if( mySerial.available()>0 )
  {     
    serialData =  mySerial.read(); 
    Serial.println(serialData, HEX);  
    if (serialData == Water_Pump_ON ) 
    {       
      openPump();  
    } 
    else if (serialData == Water_Pump_OFF )
    {    
      closePump();    
    }      
  }
}

void getSensor(){
  wetValue = analogRead(WETPIN);
  wetValue *= 100;
  wetValue = wetValue/1023;
    
  qhDHT.getTHValue(DHT11_PIN);

  Serial.print(wetValue);
  Serial.print(",\t");
  Serial.print(qhDHT.humidity,1);  
  Serial.print(",\t");  
  Serial.println(qhDHT.temperature,1);  
}

void update_lcd1602()
{
  lcd.setCursor(1,0); 
  lcd.print("TH ");
  lcd.print(LOW_TH);
  lcd.print(" P ");
  if(pump_sta == 1){ 
    lcd.print("ON    ");
  }else{
    lcd.print("OFF   ");
  }
      
  lcd.setCursor(0,1);
  lcd.print("T ");    
  lcd.print(qhDHT.temperature,0);   
  lcd.print(" H ");    
  lcd.print(qhDHT.humidity,0); 
  lcd.print(" W ");    
  lcd.print(wetValue,0);   
}

void pumpInit()
{
  pinMode( MOTOR_INA1,OUTPUT );
  pinMode( MOTOR_INA2,OUTPUT );
  analogWrite( MOTOR_INA1, 0 );
  analogWrite( MOTOR_INA2 , 0 );
}

void openPump()
{
  pump_sta = 1;
  set_red_led(1);
  analogWrite(MOTOR_INA1, 200);
  analogWrite(MOTOR_INA2 , 0);
}

void closePump()
{
  pump_sta = 0;
  set_red_led(0);
  analogWrite(MOTOR_INA1, 0);
  analogWrite(MOTOR_INA2 , 0);
}

void led_init(){
  pinMode( LED_RED_PIN,OUTPUT ); 
  pinMode( LED_GREEN_PIN,OUTPUT ); 
  digitalWrite(LED_GREEN_PIN, LOW );
  digitalWrite(LED_RED_PIN, LOW );
}

void set_red_led(unsigned char sta){
  if( sta == 0 ){
    digitalWrite(LED_RED_PIN, LOW );
  }else{
    digitalWrite(LED_RED_PIN, HIGH );
  }
}
