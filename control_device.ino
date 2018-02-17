#include <Wire.h>
#include <MCP23xxx.h>
#include <LiquidCrystal_MCP23x17.h>

CMCP23017 MCP;

LiquidCrystal_MCP23x17 lcd;

int currentKm;
int currentPk;
char string[16];

void updateString() {
  string[0] = 'K';
  string[1] = 'M';
  string[2] = '=';
  string[3] = '[';

  char buffer[3];
  buffer[0] = '0';
  buffer[1] = '0';
  buffer[2] = '0';
  itoa(currentKm,buffer,10);
  for(int i=0; i<3; ++i) {
    if(buffer[i]=='\0') {
      int k=3-i;
      for(int j=0; j<3; ++j) {
        string[4+j] = ' ';
      }
      for(int j=0; j<i; ++j) {
        string[4+j+k] = buffer[j];
      }
    }
  }
  
  string[7] = ']';
  string[8] = 'P';
  string[9] = 'K';
  string[10] = '=';
  string[11] = '[';
  buffer[0] = '0';
  buffer[1] = '0';
  buffer[2] = '0';
  itoa(currentPk,buffer,10);
  for(int i=0; i<3; ++i) {
    if(buffer[i]=='\0') {
      int k=3-i;
      for(int j=0; j<3; ++j) {
        string[12+j] = ' ';
      }
      for(int j=0; j<i; ++j) {
        string[12+j+k] = buffer[j];
      }
    }
  }
  
  string[15] = ']';
}

void setup() {
  currentKm = 0;
  currentPk = 0;
  updateString();

  Wire.begin();
  MCP.init(0);

  lcd.init(MCP);
  lcd.Backlight(1);

  lcd.begin(16, 2);
  lcd.print("================");
  MCP.pinMode(12, INPUT);
  MCP.pinMode(10, INPUT);
  MCP.pinMode(11, INPUT);
  MCP.pinMode(8, INPUT);
  MCP.pinMode(9, INPUT);
}

void loop() {
  
  lcd.setCursor(0, 0);
  updateString();
  lcd.print(string);
  delay(100);

  if (MCP.digitalRead(10)==0)
    currentPk-=1;

  if (MCP.digitalRead(11)==0)
    currentPk+=1;
    
  if (MCP.digitalRead(8)==0)
    currentKm+=1;

  if (MCP.digitalRead(9)==0)
    currentKm-=1;
    
  if (MCP.digitalRead(12)==0) {
    if(currentPk==10) {
      currentPk=0;
      currentKm+=1;
    }
    currentPk+=1;
    lcd.setCursor(0, 1);
    lcd.print("Coord set!");
  } else {
    lcd.setCursor(0, 1);
    lcd.print("          ");
  }
  
}
