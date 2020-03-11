#ifndef Animation_h
#define Animation_h

byte shower1[8] = {  0b11100,  0b00100,  0b01110,  0b11111,  0b00000,  0b10101,  0b10101,  0b10101};
byte shower2[8] = {  0b11100,  0b00100,  0b01110,  0b11111,  0b00000,  0b01010,  0b01010,  0b00000};

byte arrowDown1[8] = { 0b00100, 0b00100,  0b00100,  0b11111,  0b01110,  0b00100,  0b10001,  0b11111};
byte arrowDown2[8] = { 0b00000, 0b00100,  0b00100,  0b00100,  0b11111,  0b01110,  0b10101,  0b11111};

byte alarm1[8] = {  0b01010,  0b10001,  0b00000,  0b01110,  0b10101,  0b10111,  0b10001,  0b01110};
byte alarm2[8] = {  0b00000,  0b01010,  0b10001,  0b01110,  0b10101,  0b10111,  0b10001,  0b01110};

byte wifi1[8] = {  0b00010,  0b01001,  0b00101,  0b10101,  0b10101,  0b00101,  0b01001,  0b00010};
byte wifi2[8] = {  0b00100,  0b10010,  0b01010,  0b01010,  0b01010,  0b01010,  0b10010,  0b00100};

byte clearChar[8] = {	0b00000,	0b00000,	0b00000,	0b00000,	0b00000,	0b00000,	0b00000,	0b00000};

const int position_wifi_icon = 12;
const int position_ducha_icon = 13;
const int position_alarma_icon = 14;
const int position_datos_icon = 15;

bool animationWifi = true;
bool animationDucha = true;
bool animationAlarma = true;
bool animationDatos = true;


void createAllChars(){
	lcd.createChar(0, clearChar);
  lcd.createChar(1, wifi2);
	lcd.createChar(2, shower1);  lcd.createChar(3, shower2);
	lcd.createChar(4, alarm1);
  lcd.createChar(5, arrowDown1);  lcd.createChar(6, arrowDown2); 

}

void cleanNoMenu(){
	for (int i = 0; i < 12; i++){
		lcd.setCursor(i,0);
		lcd.write(0);
	}
}

void stableWifi_icon(){
	lcd.setCursor(position_wifi_icon, 0);
	lcd.write(1);
	animationWifi = false;
}

void stableAlarm_icon(){
	lcd.setCursor(position_alarma_icon, 0);
	lcd.write(4);
	animationAlarma = false;
}

void clear_icon(int pos){
	lcd.setCursor(pos, 0);
	lcd.write(0);
}

void handleIconWifi(){
  if (animationWifi)    stableWifi_icon();
  else { 
  	clear_icon(position_wifi_icon);
  	animationWifi = true;
  }
}

void handleIconAlarma(){
  if (animationAlarma)    stableAlarm_icon();
  else { 	
  	clear_icon(position_alarma_icon);
  	animationAlarma = true;
  }
}

void handleIconDucha(){
  lcd.setCursor(position_ducha_icon, 0);
  if (animationDucha){
  	lcd.write(2);
  	animationDucha = false;
  }
  else { 	
  	lcd.write(3);  
  	animationDucha = true;
  }
}

// void handleIconDucha(){
//   if (animationDucha > 3)
//     animationDucha = 2;
    
//   lcd.setCursor(position_ducha_icon,0);
//   lcd.write(animationDucha);
//   animationDucha++;
// }

// void handleIconAlarma(){
//   if (animationAlarma > 5)
//     animationAlarma = 4;
    
//   lcd.setCursor(position_alarma_icon,0);
//   lcd.write(animationAlarma);
//   animationAlarma++;
// }

// void handleIconDatos(){
//   if (animationDatos > 7)
//     animationDatos = 6;
    
//   lcd.setCursor(position_datos_icon,0);
//   lcd.write(animationDatos);
//   animationDatos++;
// }
#endif
