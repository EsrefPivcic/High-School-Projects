// UKLJUÄŚIVANJE LIBRARYA

#include <DHT.h> // Library za DHT (Vlaga i temp)
#include "U8glib.h" //Library za OLED
#include <Keypad.h> //Keypad Library

#define METRIC 

//DEKLARACIJA PINOVA
#define DHTPIN A0 //Senzor za vlagu i temp. na A0
int ventilatori = 6;
int led2 = 10;
int grijanje = 11;
int hladnjak = 13;
int nevlazenje = 12;
int tempp = 0;
int vlagg = 0;
int uvjet = 0;

#define DHTTYPE DHT22 //Vrsta DHT senzora

DHT dht(DHTPIN, DHTTYPE); //DHT

U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NO_ACK); //Display


// Deklaracija ostalih stvari

unsigned long previousMillis = 0;
const long interval = 3600000;
unsigned long time;
int tajmer = 0;


// DHT + OLED

bool first = true;
float hum = 0.0;
float temp = 0.0;
bool dht_test(float* humPerc, float* tempC);

// Deklaracija za keypad

const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte rowPins[ROWS] = {5, 4, 3, 2}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {9, 8, 7, 6}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
char key;

//******************************************************

void setup() {

  pinMode(ventilatori, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(hladnjak, OUTPUT);
  pinMode(grijanje, OUTPUT);
  pinMode(nevlazenje, OUTPUT);


//*****************LOGO****************** 

//***************************************
  
  

  //DHT SETUP
  Serial.begin(9600);
  dht.begin();
  first = true;
  

// ********ODREDJIVANJE BOJA**********

  
  if (u8g.getMode() == U8G_MODE_R3G3B2)
  {
    u8g.setColorIndex(255);     // white
  }
  else if (u8g.getMode() == U8G_MODE_GRAY2BIT)
  {
    u8g.setColorIndex(3);         // max intensity
  }
  else if (u8g.getMode() == U8G_MODE_BW)
  {
    u8g.setColorIndex(1);         // pixel o n
  }
  else if (u8g.getMode() == U8G_MODE_HICOLOR)
  {
    u8g.setHiColorByRGB(255, 255, 255);
  }

  dht_test(&hum, &temp); // DHT

}
//********************FUNKCIJE*********************

//****************** TEMP I VLAGA *****************

void HumMeter(float* humPerc, float* temp) 
{
  u8g.setFont(u8g_font_fub11);
  u8g.setFontRefHeightExtendedText();
  u8g.setDefaultForegroundColor();
  u8g.setFontPosTop();
  u8g.drawStr(18, 0, "Shitake SSK");
  u8g.drawStr(4, 20, "Vlaga:");
  u8g.setPrintPos(60, 20);
  u8g.drawStr(105, 20, "%");
  u8g.print(*humPerc);
#ifdef METRIC
  // if metric system, display Celsius
  u8g.drawStr(4, 40, "Temp:");
#else
  //display Farenheit
  u8g.drawStr(4, 20, "Temp F");
#endif

  u8g.setPrintPos(60, 40);
  u8g.print(*temp);
  u8g.drawStr(105, 40, "*C");
}

//********* OCITAVANJE TEMP I VLAGE *************


bool dht_test(float* humPerc, float* temp)
{
  // Wait a few seconds between measurements.

  delay(1000);
  *humPerc = 0;
  *temp = 0;

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f))
  {
    Serial.println("Failed to read from DHT sensor!");
    return false;
  }

  *humPerc = h;

  float hi = dht.computeHeatIndex(f, h);

  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.print(" *C ");

#ifdef METRIC
  // metric system, load degree celsius
  *temp = t;
#else
  *temp = f;
#endif
  return true;
}

void clearOLED(){
   u8g.firstPage();  
   do {} 
   while( u8g.nextPage() );
}
//***********************************************


//********************LOOP***********************

void loop(){

//***************PROMJENJIVI MODOVI******************
key = keypad.getKey();
if (key == '1'){
  uvjet = 1;
  Serial.println("uvjet:");
   Serial.println(uvjet);
  }
/*if (key == '2'){
  uvjet = 2;
  Serial.println("uvjet:");
   Serial.println(uvjet);
  }*/
if (key == '3'){
  uvjet = 3;
  Serial.println("uvjet:");
   Serial.println(uvjet);
  }
if (key == '4'){
  uvjet = 4;
  Serial.println("uvjet:");
   Serial.println(uvjet);
  }



//*********************** PRVI UVJET ****************************




if ( uvjet == 1 )
    {
      
   Serial.println("uvjet:");
   Serial.println(uvjet);

   //************ DHT + OLED **************

  bool result = dht_test(&hum, &temp);

  if (first)
  {
    // skip displaying readings first time, as its stale data.
    first = false;
  }
  else
  {
    if (result == false)
    {
      u8g.firstPage();
      do
      {
        // sensor error
        u8g.setFont(u8g_font_fub11);
        u8g.setFontRefHeightExtendedText();
        u8g.setDefaultForegroundColor();
        u8g.setFontPosTop();
        u8g.drawStr(10, 30, "Sensor Error");
      }
      while (u8g.nextPage());
    }
    else
    {
      u8g.firstPage();
      do
      {
        HumMeter(&hum, &temp);
      }
      while (u8g.nextPage());
    }
  }

//**********TAJMER 12H i promjena svjetla****************

  if (tajmer < 11) {
    digitalWrite(led2, HIGH);  //SVIJETLO - OFF
    Serial.println("SVIJETLO OFF");
  }
  else if (tajmer >= 11 && tajmer <= 23) {
    digitalWrite(led2, LOW);  //SVIJETLO - ON
     Serial.println("SVIJETLO ON");
  }
  else if (tajmer > 23) {
    tajmer = 0;
  }
  Serial.println("Tajmer: ");
  Serial.println(tajmer);
  
//************TAJMER 1H i promjena zraka*************
  
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    tajmer++;
    previousMillis = currentMillis;
  }

  Serial.print("Time: ");
  time = millis();
  Serial.println(time);//prints time since program started
  delay(1000);
  
//*************************Grijanje************************

float t = dht.readTemperature();
  
if ( t < 18){
    digitalWrite(grijanje, LOW);
    digitalWrite(ventilatori, HIGH);
    Serial.println("VENTILACIJA OFF");
  Serial.println("Grijanje1-HIGH");
   digitalWrite(hladnjak, HIGH);
    Serial.println("HLADĹ˝ENJE OFF");
  }
  if (t > 20){
    digitalWrite(grijanje, HIGH);
    Serial.println("Grijanje1-LOW");
    digitalWrite(ventilatori, LOW);
    Serial.println("VENTILACIJA ON");
    digitalWrite(hladnjak, LOW);
    Serial.println("HLADĹ˝ENJE ON"); 
  }
  if ( t > 18 && t < 20)
  {
    digitalWrite(grijanje, HIGH);
    Serial.println("Grijanje1-LOW - TEMP OK");
      digitalWrite(ventilatori, HIGH);
    Serial.println("VENTILACIJA OFF - TEMP OK");
       digitalWrite(hladnjak, HIGH);
    Serial.println("HLADĹ˝ENJE OFF-TEMP OK");
    }

//*************************Vlazenje************************
 
float h = dht.readHumidity();
if ( h < 40){
  digitalWrite(nevlazenje, LOW);
  Serial.println("NEvlazenje LOW");
  digitalWrite(ventilatori, HIGH);
    Serial.println("VENTILACIJA OFF");
  }
  if (h>50){
       digitalWrite(ventilatori, LOW);
    Serial.println("VENTILACIJA ON");
      digitalWrite(nevlazenje, HIGH);
  Serial.println("NEvlazenje HIGH");
    }  
    if ( h > 40 && h < 50){
      digitalWrite(ventilatori, HIGH);
    Serial.println("VENTILACIJA OFF");
      digitalWrite(nevlazenje, LOW);
  Serial.println("NEvlazenje LOW");
      }  



//***************** Ventilacija *************
  
   /*if ( grijanje == LOW || vlazenje == LOW ){
    digitalWrite(ventilatori, LOW);
    Serial.println("VENTILACIJA ON");
    }*/
    
    } 




//*********************** DRUGI UVJET ****************************



/*
        
        
if (uvjet == 2){
Serial.println("uvjet:");
Serial.println(uvjet);

//************ DHT + OLED **************

  bool result = dht_test(&hum, &temp);
  if (first)
  {
    // skip displaying readings first time, as its stale data.
    first = false;
  }
  else
  {
    if (result == false)
    {
      u8g.firstPage();
      do
      {
        // sensor error
        u8g.setFont(u8g_font_fub11);
        u8g.setFontRefHeightExtendedText();
        u8g.setDefaultForegroundColor();
        u8g.setFontPosTop();
        u8g.drawStr(10, 30, "Sensor Error");
      }
      while (u8g.nextPage());
    }
    else
    {
      u8g.firstPage();
      do
      {
        HumMeter(&hum, &temp);
      }
      while (u8g.nextPage());
    }
  }

//**********TAJMER 12H i promjena svjetla****************

  if (tajmer < 11) {
    digitalWrite(led2, HIGH);  //SVIJETLO - OFF
    Serial.println("SVIJETLO OFF");
  }
  else if (tajmer >= 11 && tajmer <= 23) {
    digitalWrite(led2, LOW);  //SVIJETLO - ON
     Serial.println("SVIJETLO ON");
  }
  else if (tajmer > 23) {
    tajmer = 0;
  }
  Serial.println("Tajmer: ");
  Serial.println(tajmer);
        
//************TAJMER 1H i promjena zraka*************
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    tajmer++;
    previousMillis = currentMillis;
  }

  Serial.print("Time: ");
  time = millis();
  Serial.println(time);//prints time since program started
  delay(1000);
  
//*************************Grijanje************************

 float t = dht.readTemperature(); 
if ( t < 18){
    digitalWrite(grijanje, LOW);
    digitalWrite(ventilatori, HIGH);
    Serial.println("VENTILACIJA OFF");
  Serial.println("Grijanje1-HIGH");
  }
  if (t > 20){
    digitalWrite(grijanje, HIGH);
    Serial.println("Grijanje1-LOW");
    digitalWrite(ventilatori, LOW);
    Serial.println("VENTILACIJA ON");
  }
  if ( t > 18 && t < 20)
  {
    digitalWrite(grijanje, HIGH);
    Serial.println("Grijanje1-LOW - TEMP OK");
      digitalWrite(ventilatori, HIGH);
    Serial.println("VENTILACIJA OFF");
    }
 
//*************************Vlazenje************************

float h = dht.readHumidity();
if ( h < 40){
  Serial.println("Vlazenje2-HIGH");
  digitalWrite(ventilatori, LOW);
    Serial.println("VENTILACIJA ON");
    digitalWrite(vlazenje, LOW);
  }
  if ( h > 50 ){
    Serial.println("Vlazenje2-LOW");
      digitalWrite(ventilatori, HIGH);
    Serial.println("VENTILACIJA OFF");
    digitalWrite(vlazenje, HIGH);
    }

}

*/




//*********************** TRECI UVJET ****************************






if (uvjet == 3){
Serial.println("uvjet:");
Serial.println(uvjet);

//************ DHT + OLED **************

  bool result = dht_test(&hum, &temp);
  if (first)
  {
    // skip displaying readings first time, as its stale data.
    first = false;
  }
  else
  {
    if (result == false)
    {
      u8g.firstPage();
      do
      {
        // sensor error
        u8g.setFont(u8g_font_fub11);
        u8g.setFontRefHeightExtendedText();
        u8g.setDefaultForegroundColor();
        u8g.setFontPosTop();
        u8g.drawStr(10, 30, "Sensor Error");
      }
      while (u8g.nextPage());
    }
    else
    {
      u8g.firstPage();
      do
      {
        HumMeter(&hum, &temp);
      }
      while (u8g.nextPage());
    }
  }

//**********TAJMER 12H i promjena svjetla****************

  if (tajmer < 11) {
    digitalWrite(led2, HIGH);  //SVIJETLO - OFF
    Serial.println("SVIJETLO OFF");
  }
  else if (tajmer >= 11 && tajmer <= 23) {
    digitalWrite(led2, LOW);  //SVIJETLO - ON
     Serial.println("SVIJETLO ON");
  }
  else if (tajmer > 23) {
    tajmer = 0;
  }
  Serial.println("Tajmer: ");
  Serial.println(tajmer);
        
//************TAJMER 1H i promjena zraka*************
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
  tajmer++;
    previousMillis = currentMillis;
  }

  Serial.print("Time: ");
  time = millis();
  Serial.println(time);//prints time since program started
  delay(1000);
  
//*************************Grijanje************************
float t = dht.readTemperature(); 

if (led2 == LOW){
if ( t < 12){
    digitalWrite(grijanje, LOW);
    digitalWrite(ventilatori, HIGH);
    digitalWrite(hladnjak, HIGH);
    Serial.println("VENTILACIJA OFF");
    Serial.println("Grijanje2-HIGH");
    Serial.println("Hladnjak-LOW");
  }
if ( t > 12 && t < 13){
    Serial.println("Grijanje2-LOW");
      digitalWrite(ventilatori, HIGH);
    Serial.println("VENTILACIJA OFF");
    digitalWrite(grijanje, HIGH);
      digitalWrite(hladnjak, HIGH);
        Serial.println("Hladnjak-LOW");
  }

  if ( t > 13){
    Serial.println("Grijanje2-LOW");
      digitalWrite(ventilatori, LOW);
    Serial.println("VENTILACIJA ON");
    digitalWrite(grijanje, HIGH);
      Serial.println("Hladnjak-HIGH");
      digitalWrite(hladnjak, LOW);
  }
}

if (led2 == HIGH){
if ( t < 18){
    digitalWrite(grijanje, LOW);
    digitalWrite(ventilatori, HIGH);
    Serial.println("VENTILACIJA OFF");
  Serial.println("Grijanje1-HIGH");
  Serial.println("Hladnjak-LOW");
  digitalWrite(hladnjak, HIGH);
  }
  if (t > 20){
    digitalWrite(grijanje, HIGH);
      digitalWrite(ventilatori, LOW);
    Serial.println("VENTILACIJA ON");
    Serial.println("Grijanje1-LOW");
     Serial.println("Hladnjak-HIGH");
  digitalWrite(hladnjak, LOW);
  }
  if ( t > 18 && t < 20)
  {
    digitalWrite(grijanje, HIGH);
      digitalWrite(ventilatori, HIGH);
    Serial.println("VENTILACIJA OFF");
    Serial.println("Grijanje1-LOW - TEMP OK");
     Serial.println("Hladnjak-LOW");
  digitalWrite(hladnjak, HIGH);
    }
}
//*************************Vlazenje************************

float h = dht.readHumidity();
if ( h < 80){
  Serial.println("nevlazenje LOW");
    digitalWrite(nevlazenje, HIGH);
       digitalWrite(ventilatori, HIGH);
    Serial.println("VENTILACIJA OFF");
  }
else {
  Serial.println("Nevlazenje-HIGH");
    digitalWrite(nevlazenje, LOW);
       digitalWrite(ventilatori, LOW);
    Serial.println("VENTILACIJA ON");
  }
 /* if ( h > 50 ){
    Serial.println("Vlazenje2-LOW");
    digitalWrite(vlazenje, LOW);
    }*/
    
   
    
    }






//*********************** CETVRTI UVJET ****************************




if (uvjet == 4){
Serial.println("uvjet:");
Serial.println(uvjet);

//************ DHT + OLED **************

  bool result = dht_test(&hum, &temp);
  if (first)
  {
    // skip displaying readings first time, as its stale data.
    first = false;
  }
  else
  {
    if (result == false)
    {
      u8g.firstPage();
      do
      {
        // sensor error
        u8g.setFont(u8g_font_fub11);
        u8g.setFontRefHeightExtendedText();
        u8g.setDefaultForegroundColor();
        u8g.setFontPosTop();
        u8g.drawStr(10, 30, "Sensor Error");
      }
      while (u8g.nextPage());
    }
    else
    {
      u8g.firstPage();
      do
      {
        HumMeter(&hum, &temp);
      }
      while (u8g.nextPage());
    }
  }

//**********TAJMER 12H i promjena svjetla****************

  if (tajmer < 11) {
    digitalWrite(led2, HIGH);  //SVIJETLO - OFF
    Serial.println("SVIJETLO OFF");
  }
  else if (tajmer >= 11 && tajmer <= 23) {
    digitalWrite(led2, LOW);  //SVIJETLO - ON
     Serial.println("SVIJETLO ON");
  }
  else if (tajmer > 23) {
    tajmer = 0;
  }
  Serial.println("Tajmer: ");
  Serial.println(tajmer);
        
//************TAJMER 1H i promjena zraka*************
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    tajmer++;
    previousMillis = currentMillis;
  }

  Serial.print("Time: ");
  time = millis();
  Serial.println(time);//prints time since program started
  delay(1000);
  
//*************************Grijanje************************
float t = dht.readTemperature(); 
//**************************8H 13C*************************
if (tajmer < 8){
if ( t < 13){
    digitalWrite(grijanje, LOW);
  Serial.println("Grijanje1-HIGH");
  digitalWrite(ventilatori, HIGH);
    Serial.println("VENTILACIJA OFF");
     Serial.println("Hladnjak-LOW");
  digitalWrite(hladnjak, HIGH);
  }
if ( t > 13 && t < 14){
    digitalWrite(grijanje, HIGH);
  Serial.println("Grijanje1-LOW");
  digitalWrite(ventilatori, HIGH);
    Serial.println("VENTILACIJA OFF");
     Serial.println("Hladnjak-LOW");
  digitalWrite(hladnjak, HIGH);
  }
  
  if (t > 14){
    digitalWrite(grijanje, HIGH);
      digitalWrite(ventilatori, LOW);
    Serial.println("VENTILACIJA ON");
    Serial.println("Grijanje1-LOW");
     Serial.println("Hladnjak-HIGH");
  digitalWrite(hladnjak, LOW);
  }
}
//**********************8H 15 - 18C************************
else if (tajmer >= 8 && tajmer <= 16){
  if ( t < 15){
    digitalWrite(grijanje, LOW);
  Serial.println("Grijanje1-HIGH");
  digitalWrite(ventilatori, HIGH);
    Serial.println("VENTILACIJA OFF");
     Serial.println("Hladnjak-LOW");
  digitalWrite(hladnjak, HIGH);
  }
  if (t > 18){
    digitalWrite(grijanje, HIGH);
      digitalWrite(ventilatori, LOW);
    Serial.println("VENTILACIJA ON");
    Serial.println("Grijanje1-LOW");
     Serial.println("Hladnjak-HIGH");
  digitalWrite(hladnjak, LOW);
  }
  if ( t > 15 && t < 18)
  {
    digitalWrite(grijanje, HIGH);
      digitalWrite(ventilatori, HIGH);
    Serial.println("VENTILACIJA OFF");
    Serial.println("Grijanje1-LOW - TEMP OK");
     Serial.println("Hladnjak-LOW");
  digitalWrite(hladnjak, HIGH);
    }
}
//**************************8H 20C*************************
else if (tajmer >= 16 && tajmer <= 23){
  if ( t < 20){
    digitalWrite(grijanje, LOW);
  Serial.println("Grijanje1-HIGH");
  digitalWrite(ventilatori, HIGH);
    Serial.println("VENTILACIJA OFF");
     Serial.println("Hladnjak-LOW");
  digitalWrite(hladnjak, HIGH);
  }
if ( t > 20 && t < 21){
    digitalWrite(grijanje, HIGH);
  Serial.println("Grijanje1-LOW");
  digitalWrite(ventilatori, HIGH);
    Serial.println("VENTILACIJA OFF");
     Serial.println("Hladnjak-LOW");
  digitalWrite(hladnjak, HIGH);
  }
  
  if (t > 21){
    digitalWrite(grijanje, HIGH);
      digitalWrite(ventilatori, LOW);
    Serial.println("VENTILACIJA ON");
    Serial.println("Grijanje1-LOW");
     Serial.println("Hladnjak-HIGH");
  digitalWrite(hladnjak, LOW);
  }
  }

//*************************Vlazenje************************

float h = dht.readHumidity();
if ( h < 85){
  digitalWrite(ventilatori, HIGH);
    Serial.println("VENTILACIJA OFF");
  Serial.println("Nevlazenje-LOW");
    digitalWrite(nevlazenje, HIGH);
  }
  if ( h > 95 ){
  Serial.println("Nevlazenje-HIGH");
    digitalWrite(nevlazenje, LOW);
    digitalWrite(ventilatori, LOW);
    Serial.println("VENTILACIJA ON");
    }

     if ( h < 95 && h > 85 ){
    digitalWrite(ventilatori, HIGH);
    Serial.println("VENTILACIJA OFF");
      Serial.println("Nevlazenje-LOW");
    digitalWrite(nevlazenje, HIGH);
    }

}
      
}
