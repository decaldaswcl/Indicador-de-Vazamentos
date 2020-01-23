#include <Nextion.h>
#include <NexText.h>

#include <simplified.h>
#include "RTClib.h"

RTC_DS3231 rtc;

DateTime now;

NexProgressBar initP  = NexProgressBar(0, 6, "initP");

NexText tClock      = NexText(0, 1, "time");
NexText tDate       = NexText(0, 2, "date");
NexText tTitle0    = NexText(0, 4, "TituloInf");
NexText tTitle2     = NexText(2, 10, "TituloInf");

//NexPage pStarting    = NexPage(0, 0, "starting");
NexPage pMain          = NexPage(1, 0, "Main");
NexPage pConfig        = NexPage(2, 0, "Config");
NexPage pClockAjust    = NexPage(3, 0, "ClockAjust");
NexPage pDateAjust    = NexPage(4, 0, "DateAjust");

NexButton bClock          = NexButton(2, 7, "bRelogio");
NexButton bDate           = NexButton(2, 9, "bDate");
NexButton bAlterarH       = NexButton(3, 16, "bAlterarH");
NexButton bAlterarD       = NexButton(4, 14, "bAlterarD");

NexNumber nH           = NexNumber(3, 10, "nH");
NexNumber nM           = NexNumber(3, 11, "nM");
NexNumber nD           = NexNumber(4, 10, "nD");
NexNumber nMo          = NexNumber(4, 11, "nMo");
NexNumber nY           = NexNumber(4, 15, "nY");

NexVariable vHour      = NexVariable(3, 12, "vHour");
NexVariable vMinutes   = NexVariable(3, 13, "vMinutes");
NexVariable vDay      = NexVariable(4, 18, "vDay");
NexVariable vMonth      = NexVariable(4, 19, "vMonth");
NexVariable vYear      = NexVariable(4, 20, "vYear");



long previousMillis = 0;
const int maxTime = 1000;

NexTouch *nex_listen_list[] = 
{
    &bClock,
    &bDate,
    &bAlterarH,
    &bAlterarD,    
    NULL
};

void bClockPopCallback(void *ptr){
  dbSerialPrintln("bClockAjustPopCallback");  
  nH.setValue(now.hour());
  nM.setValue(now.minute());
}
void bDatePopCallback(void *ptr){
  dbSerialPrintln("bdatePopCallback"); 
  uint32_t day, month, year = 0;
  day = now.day();
  month = now.month();
  year = now.year();
  nD.setValue(day);
  nMo.setValue(month);
  nY.setValue(year);
  vDay.setValue(day);
  vMonth.setValue(month);
  vYear.setValue(year);   
}
void bAlterarHPopCallback(void *ptr){
  dbSerialPrintln("bAlterarHPopCallback");
  uint32_t numH, numM, count = 0;
  bool exit=true; 
  do
  {
    vHour.getValue(&numH);
    if(numH>=0&&numH<25){ 
      exit = false;
    }else{
      count++;
    }
    if(count>10) exit= false;
  } while(exit);
  exit = true;  
  do
  {
    vMinutes.getValue(&numM);
    if(numM>=0&&numM<60){ 
      exit = false; 
    }else{
      count++;
    }  
    if(count>10) exit= false;        
  } while (exit);
  exit = true;
  if((count<=10)){
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  rtc.adjust(DateTime(now.year(), now.month(), now.day(), numH,numM, 0));
  tTitle2.setText("Data alterada com sucesso.");
  }else{
    tTitle2.setText("Falha! Tente novamente.");
  }
  pConfig.show();
  
}
void bAlterarDPopCallback(void *ptr){
  dbSerialPrintln("bAlterarHPopCallback");
  uint32_t numD, numMo, numY, count = 0;
  bool exit=true; 
  do
  {
    vYear.getValue(&numY);
    if(numY>2000&&numY<2100){ 
      exit = false;
    }else{
      count++;
    }
    if(count>10) exit= false;
  } while(exit);
  exit = true;  
  do
  {
    vMonth.getValue(&numMo);
    if(numMo>0&&numMo<13){ 
      exit = false; 
    }else{
      count++;
    }  
    if(count>10) exit= false;        
  } while (exit);
  exit = true;  
  do
  {
    vDay.getValue(&numD);
    if(numD>0&&numD<32){
      exit = false;
    }else{
      count++;
    } 
    if(count>10) exit= false;       
  } while (exit);
  
  Serial.println(numD);
  Serial.println(numMo);
  Serial.println(numY);

  if((count<=10)){
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    rtc.adjust(DateTime(numY, numMo, numD, now.hour() ,now.minute(), 0));
    tTitle2.setText("Data alterada com sucesso.");
  }else{
    tTitle2.setText("Falha! Tente novamente.");
  }
  pConfig.show();
  
}

simplified simple;



void setup() {
  Serial.begin(9600);  
  nexInit();
  initP.setValue(20);
  while (millis()<2000)

  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    tTitle0.setText("RTC não encontrado");
    initP.setValue(40);
    while (1);
  }
  
  if (rtc.lostPower()) {
    Serial.println("RTC lost power, lets set the time!");    
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    rtc.adjust(DateTime(2016, 1, 1, 0, 0, 0));
    initP.setValue(50);
  }
  bClock.attachPop(bClockPopCallback);
  bDate.attachPop(bDatePopCallback);
  bAlterarH.attachPop(bAlterarHPopCallback);
  bAlterarD.attachPop(bAlterarDPopCallback);
  initP.setValue(100);
  dbSerialPrintln("setup done");  
  pMain.show();
}
void loop() {
  nexLoop(nex_listen_list);
  if(millis() - previousMillis >= maxTime)
          {
               previousMillis = millis();
  now = rtc.now();
  String dt = String(now.day()) + "/" + String(now.month()) + "/" + String(now.year());  
  String tm = String(now.hour()) + ":" +String(now.minute());
  
  tDate.setText(simple.strToChar(dt));
  tClock.setText(simple.strToChar(tm));
          }
}
