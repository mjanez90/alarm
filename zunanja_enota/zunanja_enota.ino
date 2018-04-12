//ko najprej napišeš PWok in nato geslo gre naprej
//ko najprej napišeš stanje zaklepa in nato PWok šele nato pa geslo zablokira


#include "Keypad.h"               //knjiznica
#include "Wire.h" // for I2C LCD  //knjiznica
#include <LiquidCrystal_I2C.h>    //knjiznica

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);    //difinicija zaslona

const byte ROWS = 4; //4x vrste
const byte COLS = 4; //4x stolpci
char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte rowPins[ROWS] = {
  22, 24, 26, 28      
}; //Prikljucni pini na arduinu nano za tipkovnico vrstico

byte colPins[COLS] = {
  30, 32, 34, 36
}; //Prikljucni pini na arduinu nano za tipkovnico stolpci

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );   //difinicija tipkovnice

byte ccYes[8] = {    //Znak polni oz. JA
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111
};

byte ccNo[8] = {    //Znak prazni oz. NE
  0b11111,
  0b10001,
  0b10001,
  0b10001,
  0b10001,
  0b10001,
  0b10001,
  0b11111
};

int WaitToStart = 250;        //cakalni cas
int TEST = 5000;              //cakalni cas 
int PrintFromThere = 8;   
int LockP = 5000;    

char keypressed;              //pristisk tipke

String tempPassword;          //shtani geslo za posiljanje
String WrongCorrectPW;        //odgovor od raspbarrypija ali je geslo pravilno
String LockUnlock;
String ChangeStatus;  
String ChangeStatusResponse;

boolean activated = false;    //stanje za zagon programa
boolean PWgoNext = false;     //stanje da lahko vstopi b preverbo gesala na raspbarrypi
boolean keypressedAlowed = true;
boolean CheckSerialPW = false;
boolean CheckABCD = false;  
boolean LockUnlockStatus = false;    
boolean LockA = false;
boolean LockB = false;
boolean LockC = false;
boolean LockD = false;
boolean atest = false;
boolean btest = false;
boolean AllowKeypad = false;


void setup() {
  Serial.begin(9600); //aktivira serial monitor
  lcd.begin(20, 4); //difinicaja zaslona
  lcd.createChar(1, ccYes),
                 lcd.createChar(0, ccNo),
                 Serial.println ("init.start"); //sporoci v serial monitro zagon
  lcd.setCursor(0, 1); //PISI V DRUGO VRSTO NA ZASLONU
  lcd.print("     ZAGANJAM!!     "); //napise na zaslon da zaganja
  delay(WaitToStart); //poĂ„Ĺ¤aka 5sekund
  Serial.println ("init.end"); //sporoci konec zagona v serial monitor
  lcd.clear(); //pocisti zaslon
}

void loop() {
  keypressed = "";
  keypressedAlowed = true;
  tempPassword = "";
  activated = true;
  PrintFromThere = 8;
  


  Serial.println (".cleartempPassword");
  lcd.clear(); //pocisti zaslon
  lcd.setCursor(0, 0); //PISI V DRUGO VRSTO NA ZASLONU
  lcd.print("        BUT!        "); //Pozdrav dokler ne odklenes oz. pritisnes tipke
  lcd.setCursor(0, 1);
  lcd.print("  VNESI GESLO/RFID  "); //Pozdrav dokler ne odklenes oz. pritisnes tipke
  lcd.setCursor(0, 2);
  lcd.print("                    "); //Pozdrav dokler ne odklenes oz. pritisnes tipke
  lcd.setCursor(0, 3);
  lcd.print("* ZAVRNI    POTRDI #"); //Pozdrav dokler ne odklenes oz. pritisnes tipke

  while (activated && keypressedAlowed == true) {
    keypressed = keypad.getKey();
    if (keypressed != NO_KEY) {
      if (keypressed == '0' || keypressed == '1' || keypressed == '2' || keypressed == '3' ||
          keypressed == '4' || keypressed == '5' || keypressed == '6' || keypressed == '7' ||
          keypressed == '8' || keypressed == '9' )      {
        tempPassword += keypressed;
        lcd.setCursor(PrintFromThere, 2);
        lcd.print("*");
        PrintFromThere++;

      }
    }
      if (PrintFromThere > 12 || keypressed == '*') {
      tempPassword = "";
      PrintFromThere = 8;
      PWgoNext = true;
      Serial.println (".cleartempPassword");   //prikaz za vnos gesla
      lcd.setCursor(0, 0);                    //prikaz za vnos gesla
      lcd.print("        BUT!        ");      //prikaz za vnos gesla
      lcd.setCursor(0, 1);                    //prikaz za vnos gesla
      lcd.print("  VNESI GESLO/RFID  ");      //prikaz za vnos gesla
      lcd.setCursor(0, 2);                    //prikaz za vnos gesla
      lcd.print("                    ");      //prikaz za vnos gesla
      lcd.setCursor(0, 3);                    //prikaz za vnos gesla
      lcd.print("* ZAVRNI    POTRDI #");      //prikaz za vnos gesla
    }
    
    if ( keypressed == '#') {
      Serial.println(tempPassword + ".CheckPW");
//      keypressedAlowed = false;
      CheckSerialPW = true;
      lcd.setCursor(0, 3);                    //prikaz za vnos gesla
      lcd.print("# PREVERJAM GESLO! #");      //prikaz za vnos gesla
      PWgoNext = true;
//      PrintFromThere = 8;
//      tempPassword = "";
          }


          
    
//      keypressed = keypad.getKey();
//      if (keypressed != NO_KEY) {
//      if ( keypressed == '*' ) {
//      
//        PrintFromThere = 8;
//        PWgoNext = false;
//        CheckSerialPW = false;
//
//        loop();
//      }
//      }
      

    while (CheckSerialPW){  
    while (Serial.available() > 0) {
      WrongCorrectPW = Serial.readString();
      if (WrongCorrectPW == "WrongPW" && PWgoNext == true) {
        Serial.println(".ArduinoSayWrongPW");
        lcd.clear();
        delay(100);
        lcd.setCursor(0, 0);
        lcd.print("     GESLO/RFID     ");
        lcd.setCursor(0, 1);
        lcd.print("    NEPRAVILNO!!    ");
        lcd.setCursor(0, 2); //PISI V DRUGO VRSTO NA ZASLONU
        lcd.print("                    "); //Pozdrav dokler ne odklenes oz. pritisnes tipke
        lcd.setCursor(0, 3);
        lcd.print("* ZAVRNI            "); //Pozdrav dokler ne odklenes oz. pritisnes tipke
        AllowKeypad = true;
        PWgoNext = false;
        CheckSerialPW = false;
        
        while (AllowKeypad){
          keypressed = keypad.getKey();
          if (keypressed != NO_KEY) {
          if ( keypressed == '*' ) {
            loop();        
        }
          }
        }
      }
              
      if (WrongCorrectPW == "PWok" && PWgoNext == true) {
        Serial.println(".ArduinoSayPWok");
        PWgoNext = false;
        CheckSerialPW = false;
        Zone();
       
      }
    }
  }
}
}


void Zone() {
  
  lcd.clear();
  Serial.println(".Zone");
  lcd.setCursor(0, 0);
  lcd.print("    UPORABLJENO-    ");
  lcd.setCursor(0, 1);
  lcd.print("     GESLO/RFID     ");
  lcd.setCursor(0, 3);
  lcd.print("     PRAVILNO!!     ");


  
  
  LockUnlockStatus = true;  

while (LockUnlockStatus)
  while (Serial.available()){ 
    LockUnlock = Serial.readString();
    Serial.println(LockUnlock + ".Status" );

char ZonaA = LockUnlock.charAt(0);
char ZonaB = LockUnlock.charAt(1);
char ZonaC = LockUnlock.charAt(2);
char ZonaD = LockUnlock.charAt(3);

keypressedAlowed = true;
    
        if(ZonaA == 49){
        LockA=true;
        lcd.setCursor(2, 0);
        lcd.write((byte)1);  
        }
            if(ZonaA == 48){
            LockA=false;
            lcd.setCursor(2, 0);
            lcd.write((byte)0);  
            }
        
        if(ZonaB == 49){
        LockB=true;
        lcd.setCursor(2, 1);
        lcd.write((byte)1); 
        }
            if(ZonaB == 48){
            LockB=false;
            lcd.setCursor(2, 1);
            lcd.write((byte)0); 
            }
        
        if(ZonaC == 49){
        LockC=true;
        lcd.setCursor(2, 2);
        lcd.write((byte)1); 
        }
            if(ZonaC == 48){
            LockC=false;
            lcd.setCursor(2, 2);
            lcd.write((byte)0); 
            }
        
        if(ZonaD == 49){
        LockD=true;
        lcd.setCursor(2, 3);
        lcd.write((byte)1);
        }
            if(ZonaD == 48){
            LockD=false;
            lcd.setCursor(2, 3);
            lcd.write((byte)0);
            }


  lcd.setCursor(14, 0);
  lcd.print("ZAVRNI");
  lcd.setCursor(14, 1);
  lcd.print("   *  ");
  lcd.setCursor(14, 2);
  lcd.print("POTRDI");
  lcd.setCursor(14, 3);
  lcd.print("   #  ");

  lcd.setCursor(0, 0);        //VHOD
  lcd.print("A");             //VHOD
  lcd.setCursor(4, 0);        //VHOD
  lcd.print("VHOD      ");          //VHOD
  lcd.setCursor(0, 1);        //GARAZA
  lcd.print("B");             //GARAZA
  lcd.setCursor(4, 1);        //GARAZA
  lcd.print("GARAZA    ");        //GARAZA
  lcd.setCursor(0, 2);        //KLET
  lcd.print("C");             //KLET
  lcd.setCursor(4, 2);        //KLET
  lcd.print("KLET      ");          //KLET
  lcd.setCursor(0, 3);        //KUHINJA
  lcd.print("D");             //KUHINJA 
  lcd.setCursor(4, 3);        //KUHINJA
  lcd.print("KUHINJA   ");       //KUHINJA
  CheckABCD = true;

 while (CheckABCD) {
keypressed = keypad.getKey();
if (keypressed != NO_KEY) {
          
           if ( keypressed == '*' ) {
                Serial.println(LockUnlock + ".Status");
                  loop();
                  }
           if ( keypressed == 'A' ) {
                if (LockA == true){
                    LockA = false;
                    lcd.setCursor(2, 0);
                    lcd.write((byte)0);  
                    Serial.println(".Aoff");
                }
                    else{
                    LockA = true;
                    lcd.setCursor(2, 0);
                    lcd.write((byte)1);  
                    Serial.println(".Aon");
            }
            }
            if ( keypressed == 'B' ) {
                  if (LockB == true){
                      LockB = false;
                      lcd.setCursor(2, 1);
                      lcd.write((byte)0);  
                      Serial.println(".Boff");
                  }
                      else{
                      LockB = true;
                      lcd.setCursor(2, 1);
                      lcd.write((byte)1);  
                      Serial.println(".Bon");
            }
            }
            if ( keypressed == 'C' ) {
                  if (LockC == true){
                      LockC = false;
                      lcd.setCursor(2, 2);
                      lcd.write((byte)0);  
                      Serial.println(".Coff");
                  }
                      else{
                      LockC = true;
                      lcd.setCursor(2, 2);
                      lcd.write((byte)1);  
                      Serial.println(".Con");           
            }
            }
            if ( keypressed == 'D' ) {
                  if (LockD == true){
                      LockD = false;
                      lcd.setCursor(2, 3);
                      lcd.write((byte)0);  
                      Serial.println(".Doff");
                  }
                      else{
                      LockD = true;
                      lcd.setCursor(2, 3);
                      lcd.write((byte)1);  
                      Serial.println(".Don");             
          }
          }
          if ( keypressed == '#' ) {

            Serial.print(LockA);
            Serial.print(LockB);
            Serial.print(LockC);
            Serial.print(LockD);
            Serial.println(".NewStatus");
           
          lcd.setCursor(14, 2);
          lcd.print ("      ");
          lcd.setCursor(14, 3);
          lcd.print ("      ");  
   
        atest =true;
        
        while (atest)
    while (Serial.available()){ 
        ChangeStatus = Serial.readString();
           if (ChangeStatus == "ChangeOK") {
        
        
        lcd.clear();
          lcd.setCursor(0, 2);
          lcd.print ("  SPREMENIM CEZ:");
          lcd.setCursor(18, 2);
          lcd.print ("3");
          delay(1000);
          lcd.setCursor(18, 2);
          lcd.print ("2");
          delay(1000);
          lcd.setCursor(18, 2);
          lcd.print ("1");
          delay(1000);
          lcd.setCursor(18, 2);
          lcd.print ("0");
          delay(1000);
          btest = true;

                
          while (btest)
          while (Serial.available()){ 
          ChangeStatusResponse = Serial.readString();
          lcd.clear();
            
            
            if (ChangeStatusResponse == "OK.1111") {
              lcd.setCursor(0, 0);
              lcd.print ("####################");    
              lcd.setCursor(0, 2);
              lcd.print ("STANJE SPREMENJENO !");
              delay(3000);  
              loop();  
              }

            
            if (ChangeStatusResponse == "OK.0111") {
              lcd.setCursor(0, 0);
              lcd.print ("#  NAPAKA - VHOD   #");
              delay (LockP);
              loop();          
          }
            if (ChangeStatusResponse == "OK.0011") {
              lcd.setCursor(0, 0);
              lcd.print ("#  NAPAKA - VHOD   #");
              lcd.setCursor(0, 1);
              lcd.print ("# NAPAKA - GARAZA  #");
              delay (LockP);
              loop();          
          }
            if (ChangeStatusResponse == "OK.0001") {
              lcd.setCursor(0, 0);
              lcd.print ("#  NAPAKA - VHOD   #");
              lcd.setCursor(0, 1);
              lcd.print ("# NAPAKA - GARAZA  #");
              lcd.setCursor(0, 2);
              lcd.print ("#  NAPAKA - KLET   #");
              delay (LockP);
              loop();          
          }
            if (ChangeStatusResponse == "OK.0000") {
              lcd.setCursor(0, 0);
              lcd.print ("#  NAPAKA - VHOD   #");
              lcd.setCursor(0, 1);
              lcd.print ("# NAPAKA - GARAZA  #");
              lcd.setCursor(0, 2);
              lcd.print ("#  NAPAKA - KLET   #");
              lcd.setCursor(0, 3);
              lcd.print ("# NAPAKA - KUHINJA #");
              delay (LockP);
              loop();          
          }
            if (ChangeStatusResponse == "OK.0010") {
              lcd.setCursor(0, 2);
              lcd.print ("#  NAPAKA - KLET   #");
              delay (LockP);
              loop();          
          }
            if (ChangeStatusResponse == "OK.0100") {
              lcd.setCursor(0, 1);
              lcd.print ("# NAPAKA - GARAZA  #");
              delay (LockP);
              loop();
          }
            if (ChangeStatusResponse == "OK.0101") {
              lcd.setCursor(0, 1);
              lcd.print ("# NAPAKA - GARAZA  #");
              lcd.setCursor(0, 3);
              lcd.print ("# NAPAKA - KUHINJA #");
              delay (LockP);
              loop();          
          }
            if (ChangeStatusResponse == "OK.0110") {
              lcd.setCursor(0, 1);
              lcd.print ("# NAPAKA - GARAZA  #");
              lcd.setCursor(0, 2);
              lcd.print ("#  NAPAKA - KLET   #");
              delay (LockP);
              loop();          
          }
            if (ChangeStatusResponse == "OK.1000") {
              lcd.setCursor(0, 1);
              lcd.print ("# NAPAKA - GARAZA  #");
              lcd.setCursor(0, 2);
              lcd.print ("#  NAPAKA - KLET   #");
              lcd.setCursor(0, 3);
              lcd.print ("# NAPAKA - KUHINJA #");
              delay (LockP);
              loop();          
          }
            if (ChangeStatusResponse == "OK.1001") {
              lcd.setCursor(0, 1);
              lcd.print ("# NAPAKA - GARAZA  #");
              lcd.setCursor(0, 2);
              lcd.print ("#  NAPAKA - KLET   #");
              delay (LockP);
              loop();
          }
            if (ChangeStatusResponse == "OK.1010") {
              lcd.setCursor(0, 1);
              lcd.print ("# NAPAKA - GARAZA  #");
              lcd.setCursor(0, 3);
              lcd.print ("# NAPAKA - KUHINJA #");
              delay (LockP);
              loop();
          }
            if (ChangeStatusResponse == "OK.1011") {
              lcd.setCursor(0, 1);
              lcd.print ("# NAPAKA - GARAZA  #");
          }
            if (ChangeStatusResponse == "OK.1100") {
              lcd.setCursor(0, 2);
              lcd.print ("#  NAPAKA - KLET   #");
              lcd.setCursor(0, 3);
              lcd.print ("# NAPAKA - KUHINJA #");
              delay (LockP);
              loop();
          }
            if (ChangeStatusResponse == "OK.1101") {
              lcd.setCursor(0, 2);
              lcd.print ("#  NAPAKA - KLET   #");
              delay (LockP);
              loop();
          }
            if (ChangeStatusResponse == "OK.1110") {
              lcd.setCursor(0, 3);
              lcd.print ("# NAPAKA - KUHINJA #");
              delay (LockP);
              loop();
          }
          
          }
          
                               
          if ( keypressed == '*' ) {
            
            Serial.println("Status." + LockUnlock);
            loop();
          }

}
}
}
}
}
}
} 






