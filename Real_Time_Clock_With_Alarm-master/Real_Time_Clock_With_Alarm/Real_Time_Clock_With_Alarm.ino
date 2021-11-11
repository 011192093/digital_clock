#include <DS3231.h>// RTC3231 Library
#include <Wire.h>  // i2C Conection Library
#include <LiquidCrystal.h> 
#include <EEPROM.h>

LiquidCrystal lcd(2, 3, 4, 5, 6, 7); //Arduino pins to lcd

#define bt_time   A0
#define bt_up     A1
#define bt_down   A2
#define bt_alarm  A3

#define buzzer 8
#define led 13

DS3231  rtc(SDA, SCL);

Time  t; 

int hh = 0, mm = 0, ss = 0;


int AlarmHH  = 2, AlarmMM  = 6, AlarmSS  = 56, setMode = 0, setAlarm = 0, alarmMode=0;
String AlarmAP="PM";
String ap;
int stop =0, mode=0, flag=0;

//Eeprom Store Variable
uint8_t HH;
uint8_t MM;

void setup(){
  Serial.begin(9600);
   rtc.begin(); 

 pinMode(bt_time,  INPUT_PULLUP);
 pinMode(bt_up,    INPUT_PULLUP);
 pinMode(bt_down,  INPUT_PULLUP);
 pinMode(bt_alarm, INPUT_PULLUP);

 pinMode(buzzer, OUTPUT);
 pinMode(led, OUTPUT);
 
  lcd.begin(16, 2); 
  lcd.setCursor(0,0);  //Show "TIME" on the LCD

stop=EEPROM.read(50);
if(stop==0){  
}else{WriteEeprom ();}

EEPROM.write(50,0); 

ReadEeprom(); 
}


void loop(){  
t = rtc.getTime();

if (setMode == 0){//if set time is not pressed 
hh = t.hour,DEC;
mm = t.min,DEC;
ss = t.sec,DEC;
} 

if(hh==13){
hh=1;
}
if(hh==14){
hh=2;
}
if(hh==15){
hh=3;
}
if(hh==16){
hh=4;
}
if(hh==17){
hh=5;
}
if(hh==18){
hh=6;
}
if(hh==19){
hh=7;
}
if(hh==20){
hh=8;
}
if(hh==21){
hh=9;
}
if(hh==22){
hh=10;
}
if(hh==23){
hh=11;
}
if(hh==00){
hh=12;
}
  if(setAlarm==0){//if set alarm is not pressed show timing
  lcd.setCursor(0,0); 
  lcd.print((hh/10)%10);
  lcd.print(hh % 10); 
  lcd.print(":");
  lcd.print((mm/10)%10);
  lcd.print(mm % 10);
  lcd.print(":");
  lcd.print((ss/10)%10);
  lcd.print(ss % 10);
  lcd.print(" ");  
  
 if(t.hour<12){
  if(ap=="PM"){
    lcd.print("PM"); 
    ap="PM";
  }
  else{
  lcd.print("AM"); 
  ap="AM"; 
  }
  }
 else{
 if(ap=="AM"){
    lcd.print("AM"); 
    ap="AM";
  }
  else{
  lcd.print("PM"); 
  ap="PM"; 
  }
  }
  }


setupClock();//work of setting time
setTimer();//alarm
delay (100);
blinking();

//Alarm ,when the buzzer will go off
if (alarmMode==1 && mode==1 && hh==AlarmHH && mm==AlarmMM && ss<=30 && ap==AlarmAP) {
  digitalWrite(led, HIGH);
  digitalWrite(buzzer, HIGH);

delay (300);
digitalWrite(led, LOW);
digitalWrite(buzzer, LOW);

}
else{digitalWrite(led, LOW);digitalWrite(buzzer, LOW);}


delay (100);
}


void blinking (){
//BLINKING SCREEN
if (setAlarm <2 && setMode == 1){lcd.setCursor(0,0);  lcd.print("  ");}
if (setAlarm <2 && setMode == 2){lcd.setCursor(3,0);  lcd.print("  ");}
if (setAlarm <2 && setMode == 3){lcd.setCursor(6,0);  lcd.print("  ");}
if (setAlarm <2 && setMode == 4){lcd.setCursor(9,0);  lcd.print("  ");}
//Alarm
if (setMode == 0 && setAlarm == 1){lcd.setCursor(6,0); lcd.print("           "); }
if (setMode == 0 && setAlarm == 2){lcd.setCursor(4,1); lcd.print("  "); }
if (setMode == 0 && setAlarm == 3){lcd.setCursor(7,1); lcd.print("  "); }
if (setMode == 0 && setAlarm == 4){lcd.setCursor(10,1);lcd.print("  "); }
if (setMode == 0 && setAlarm == 5){lcd.setCursor(13,1);lcd.print("  "); }
}

//work of setting time
void setupClock (void) {
   //after setting time confirmation the lcd will show upated clock time
    if (setMode == 5){
    lcd.setCursor (0,0);
    lcd.print (F("Set Time Finish "));
    delay (600);
    rtc.setTime (hh, mm, ss); 
    lcd.clear();
    setMode = 0;
    }
    //after setting alarm confirmation the lcd will show originalclock time
    if (setAlarm == 6){
    lcd.setCursor (0,0);
    lcd.print (F("Set Alarm Finish"));
    lcd.setCursor (0,1);
    lcd.print (F("               "));
    WriteEeprom();
    delay (600); 
    lcd.clear();
    setAlarm=0;
    alarmMode=1;
    }
   //if alarm is set alarmmode will be 0 so that alarm can be set again
 if (setAlarm >0){ alarmMode=0;}
    // avoid button crash
 if(digitalRead (bt_time) == 0 && flag==0) {flag=1;
 if(setAlarm>0){setAlarm=6;}
 else{setMode = setMode+1;}
 }
  // avoid button crash
 if(digitalRead (bt_alarm) == 0 && flag==0){flag=1;
 if(setMode>0){setMode=5;}
  else{setAlarm = setAlarm+1;} 
  lcd.clear();} 

if(digitalRead (bt_time) == 1 && digitalRead (bt_alarm) == 1){flag=0;}
  //up button
 if(digitalRead (bt_up) == 0){                          
            if (setAlarm<2 && setMode==1)hh=hh+1; 
            if (setAlarm<2 && setMode==2)mm=mm+1;
            if (setAlarm<2 && setMode==3)ss=ss+1;
            if (setAlarm<2 && setMode==4 && ap=="PM"){ap="AM";}
            //Alarm
            if (setMode==0 && setAlarm==1)mode=1;
            if (setMode==0 && setAlarm==2 )AlarmHH=AlarmHH+1;
            if (setMode==0 && setAlarm==3 )AlarmMM=AlarmMM+1;
            if (setMode==0 && setAlarm==4 )AlarmSS=AlarmSS+1;
            if (setMode==0 && setAlarm==5 && AlarmAP=="PM")AlarmAP="AM";

if(hh>23)hh=0;
if(mm>59)mm=0;
if(ss>59)ss=0;
if(AlarmHH>23)AlarmHH=0;
if(AlarmMM>59)AlarmMM=0;
if(AlarmSS>59)AlarmSS=0;
}        
//down button
if(digitalRead (bt_down) == 0){                                      
            if (setAlarm<2 && setMode==1)hh=hh-1; 
            if (setAlarm<2 && setMode==2)mm=mm-1;
            if (setAlarm<2 && setMode==3)ss=ss-1;
            if (setAlarm<2 && setMode==4 && ap=="AM"){ap="PM";}
            //Alarm
            if (setMode==0 && setAlarm==1 )mode=0;
            if (setMode==0 && setAlarm==2 )AlarmHH=AlarmHH-1;
            if (setMode==0 && setAlarm==3 )AlarmMM=AlarmMM-1;
            if (setMode==0 && setAlarm==4 )AlarmSS=AlarmSS-1;
            if (setMode==0 && setAlarm==5 && AlarmAP=="AM")AlarmAP="PM";
if(hh<0)hh=23;
if(mm<0)mm=59;
if(ss<0)ss=59;
if(AlarmHH<0)AlarmHH=23;
if(AlarmMM<0)AlarmMM=59;
if(AlarmSS<0)AlarmSS=59;
 }
 
}


void setTimer (){
  //Alarm

if(AlarmHH==13){
AlarmHH=1;
}
if(AlarmHH==14){
AlarmHH=2;
}
if(AlarmHH==15){
AlarmHH=3;
}
if(AlarmHH==16){
AlarmHH=4;
}
if(AlarmHH==17){
AlarmHH=5;
}
if(AlarmHH==18){
AlarmHH=6;
}
if(AlarmHH==19){
AlarmHH=7;
}
if(AlarmHH==20){
AlarmHH=8;
}
if(AlarmHH==21){
AlarmHH=9;
}
if(AlarmHH==22){
AlarmHH=10;
}
if(AlarmHH==23){
AlarmHH=11;
}
if(AlarmHH==00){
AlarmHH=12;
}
 if (setMode == 0 && setAlarm >0){
  lcd.setCursor (0,0);
  lcd.print("Alarm ");
 if(mode==0){lcd.print("Deactivate");}
        else{lcd.print("Activated ");}
        
  lcd.setCursor (4,1);
  lcd.print((AlarmHH/10)%10);
  lcd.print(AlarmHH % 10);
  lcd.print(":");
  lcd.print((AlarmMM/10)%10);
  lcd.print(AlarmMM % 10);
  lcd.print(":");
  lcd.print((AlarmSS/10)%10);
  lcd.print(AlarmSS % 10);
  lcd.print(" "); 
  lcd.print(AlarmAP);
 }
}

void ReadEeprom () {
  AlarmHH=EEPROM.read(1);
  AlarmMM=EEPROM.read(2);
  AlarmSS=EEPROM.read(3);
  
  mode=EEPROM.read(4); 
}

void WriteEeprom () {
  EEPROM.write(1,AlarmHH);
  EEPROM.write(2,AlarmMM);
  EEPROM.write(3,AlarmSS);
  
  EEPROM.write(4,mode);
}
