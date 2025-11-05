#include <TinyGPS++.h>
#include <TinyGPSPlus.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_BMP085.h>
#include <HardwareSerial.h>

#define oled_address 0x3C
#define GPS_RX 4
#define GPS_TX 5

Adafruit_SSD1306 display(128, 64, &Wire, -1 );
Adafruit_BMP085 bmp;
TinyGPSPlus gps;

HardwareSerial Hs(1);

const unsigned char Hribi_bitmap [] PROGMEM = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x06, 0xe0, 0x00, 0x00, 
	0x0c, 0x10, 0x00, 0x00, 0x0f, 0xf2, 0x01, 0x80, 0x0f, 0xf7, 0x07, 0xf0, 0x00, 0x0d, 0x04, 0x18, 
	0x00, 0x09, 0xb7, 0xf8, 0x00, 0x00, 0xf8, 0x00, 0x00, 0x10, 0xc8, 0x00, 0x00, 0x30, 0x48, 0x00, 
	0x00, 0x36, 0x64, 0x00, 0x00, 0x7d, 0xe4, 0x00, 0x00, 0x48, 0x3e, 0x00, 0x00, 0xc0, 0x1f, 0x00, 
	0x00, 0x80, 0x09, 0x00, 0x01, 0xb0, 0x41, 0x80, 0x01, 0x30, 0xe0, 0x80, 0x03, 0x70, 0xa0, 0xc0, 
	0x02, 0x58, 0xb0, 0x40, 0x06, 0x78, 0xb0, 0x60, 0x04, 0x58, 0xb0, 0x20, 0x0c, 0x79, 0xf0, 0x30, 
	0x18, 0x70, 0x60, 0x10, 0x1f, 0xff, 0xff, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

const unsigned char Satelit_bitmap2 [] PROGMEM = { // 32x32
	0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x04, 0x80, 0x00, 0x00, 0x0d, 0xc0, 0x00, 0x00, 
	0x17, 0x20, 0x00, 0x00, 0x27, 0x70, 0x00, 0x00, 0x3d, 0xd8, 0x00, 0x00, 0x19, 0xdc, 0x06, 0x00, 
	0x0f, 0x64, 0x3d, 0x00, 0x06, 0x7c, 0x64, 0x80, 0x03, 0x98, 0xc3, 0x80, 0x01, 0xbd, 0x81, 0x00, 
	0x00, 0xe7, 0x01, 0x00, 0x00, 0x06, 0x03, 0x00, 0x00, 0x0c, 0x06, 0x00, 0x00, 0x18, 0x0c, 0x00, 
	0x00, 0x10, 0x18, 0x00, 0x00, 0x10, 0x30, 0x00, 0x00, 0x18, 0x7b, 0x80, 0x07, 0xcc, 0xce, 0xc0, 
	0x04, 0x37, 0x8e, 0xe0, 0x02, 0x10, 0x1b, 0x30, 0x01, 0x08, 0x13, 0xe8, 0x00, 0x88, 0x1c, 0xcc, 
	0x01, 0x48, 0x0d, 0xfa, 0x48, 0x38, 0x07, 0x32, 0x48, 0x18, 0x03, 0x7c, 0x26, 0x00, 0x01, 0xc8, 
	0x30, 0x00, 0x00, 0xd0, 0x18, 0x00, 0x00, 0x60, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

const unsigned char Map_bitmap [] PROGMEM = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3c, 0x00, 0x00, 0x00, 0x26, 0x00, 
	0x1e, 0x00, 0x6a, 0x00, 0x1b, 0xe7, 0x3e, 0xe0, 0x18, 0x3e, 0x1c, 0x38, 0x18, 0x18, 0x00, 0x08, 
	0x18, 0x08, 0x08, 0x08, 0x18, 0x08, 0x08, 0x08, 0x18, 0x08, 0x08, 0x08, 0x1f, 0x99, 0xc1, 0x88, 
	0x19, 0xff, 0x88, 0xf8, 0x00, 0x18, 0x08, 0x18, 0x00, 0x08, 0x00, 0x08, 0x3c, 0x08, 0x00, 0x08, 
	0x6c, 0x08, 0x08, 0x08, 0x64, 0x00, 0x08, 0x08, 0x7d, 0x00, 0x61, 0xe8, 0x38, 0x36, 0x00, 0x78, 
	0x00, 0x00, 0x08, 0x08, 0x10, 0x00, 0x08, 0x08, 0x18, 0x08, 0x08, 0x08, 0x18, 0x08, 0x08, 0x08, 
	0x1e, 0x08, 0x7e, 0x08, 0x07, 0xfb, 0xef, 0xf8, 0x00, 0x7e, 0x0e, 0xf8, 0x00, 0x00, 0x0f, 0xd8, 
	0x00, 0x00, 0x0f, 0x48, 0x00, 0x00, 0x07, 0xf8, 0x00, 0x00, 0x00, 0x70, 0x00, 0x00, 0x00, 0x00
};

const unsigned char SatDish_bitmap [] PROGMEM = {
	0x00, 0x07, 0x00, 0x00, 0x00, 0x01, 0xc0, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x06, 0x10, 0x00, 
	0x30, 0x03, 0x88, 0x00, 0x38, 0x00, 0xcc, 0x00, 0x6c, 0x04, 0x64, 0x00, 0x46, 0x0e, 0x24, 0x00, 
	0x43, 0x11, 0x24, 0x00, 0x41, 0x93, 0x00, 0x00, 0x40, 0xde, 0x00, 0x00, 0x40, 0x70, 0x00, 0x00, 
	0x60, 0x30, 0x00, 0x00, 0x20, 0x18, 0x00, 0x00, 0x30, 0x0c, 0x00, 0x00, 0x10, 0x06, 0x00, 0x00, 
	0x18, 0x03, 0x00, 0x00, 0x1c, 0x01, 0x80, 0x00, 0x33, 0x00, 0xc0, 0x00, 0x21, 0x80, 0xc0, 0x00, 
	0x20, 0x7b, 0x80, 0x00, 0x60, 0x6e, 0x00, 0x00, 0x40, 0x20, 0x00, 0x00, 0xc0, 0x30, 0x00, 0x00, 
	0x7f, 0xf0, 0x00, 0x00
};

const unsigned char Weather_bitmap [] PROGMEM = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x10, 0x00, 
	0x00, 0x00, 0x10, 0x10, 0x00, 0x18, 0x00, 0x30, 0x00, 0x0c, 0x00, 0x60, 0x00, 0x00, 0x7e, 0x00, 
	0x00, 0x01, 0xc7, 0x00, 0x00, 0x03, 0x81, 0x80, 0x00, 0x3f, 0xc0, 0xc0, 0x00, 0x70, 0x60, 0xc0, 
	0x00, 0xc0, 0x30, 0x4e, 0x00, 0x80, 0x18, 0x46, 0x01, 0x80, 0x0c, 0xc0, 0x0f, 0x00, 0x0f, 0xc0, 
	0x3b, 0x00, 0x0d, 0xc0, 0x31, 0x00, 0x0c, 0x60, 0x60, 0x00, 0x00, 0x60, 0x60, 0x00, 0x00, 0x20, 
	0x60, 0x00, 0x00, 0x20, 0x60, 0x6c, 0x00, 0x60, 0x30, 0x38, 0x00, 0x60, 0x1e, 0xce, 0x03, 0xc0, 
	0x04, 0xee, 0x03, 0x00, 0x00, 0x38, 0xd0, 0x00, 0x00, 0x28, 0xf0, 0x00, 0x00, 0x01, 0x9c, 0x00, 
	0x00, 0x01, 0x9c, 0x00, 0x00, 0x00, 0xf0, 0x00, 0x00, 0x00, 0xd0, 0x00, 0x00, 0x00, 0x00, 0x00
};

const unsigned char* slike[] =  {Hribi_bitmap, Weather_bitmap, Satelit_bitmap2, Map_bitmap };
const char* modes[] = {"Hikeing", "Weather", "Gps", "Map"};                               // selector_maxIndex++
int selector;
int selector_maxIndex;

void setup() {
  Serial.begin(9600);
	Hs.begin(9600, SERIAL_8N1, GPS_RX, GPS_TX);
  Wire.begin(6, 7); // I2C
	
  
  if(!display.begin(SSD1306_SWITCHCAPVCC, oled_address)){
    Serial.println(F("Zagon ekrana neuspešen"));
    while(1) {};
  }
	if (!bmp.begin()) {
	Serial.println("Zagon bmp180 neuspešen");
	while (1) {}
  }

	selector=0;
	selector_maxIndex=3;

  display.clearDisplay();
 
  //display.drawBitmap(0,0, Satelit_bitmap1, 128, 64, SSD1306_WHITE);
	//display.display();
	
	display.drawRoundRect(44,12,40,40,10,SSD1306_WHITE);
	
	display.display();
	delay(250);
	animacija_slike(Hribi_bitmap, 32, 32, 48, 16, 1, false, false);
	
}

void loop() {
	if(selector>selector_maxIndex) selector=0;
	int trenutni_selector = selector;
	selector++;

	if(selector>selector_maxIndex) selector=0;
	int naslednji_selector = selector;
		
	//Gps();
	//Bmp180();
	Start_menu(slike[trenutni_selector], modes[trenutni_selector]);
	delay(2000);
 	ModeSwitch(slike[trenutni_selector], slike[naslednji_selector], 32, 32, 48, 10, 3);
	Start_menu(slike[trenutni_selector+1], modes[trenutni_selector+1]);
	delay(2000);
	Bmp180();
	delay(4000);
	Start_menu(slike[trenutni_selector+1], modes[trenutni_selector+1]);
	delay(1000);
	ModeSwitch(slike[trenutni_selector+1], slike[naslednji_selector+1], 32, 32, 48, 10, 3);
	Start_menu(slike[trenutni_selector+2], modes[trenutni_selector+2]);
	delay(1000);
	Gps();
	delay(3000);
}


//funkcije-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	void Start_menu (const unsigned char* Slika, const char* Mode ){
		display.clearDisplay();
		display.drawRoundRect(0, 0, 128, 64, 5, SSD1306_WHITE);
		display.setTextColor(SSD1306_WHITE);
		display.setTextSize(1);
  	int16_t x1, y1;
  	uint16_t w1, h1, w2, h2;
		display.getTextBounds("MODE: ", 0, 0, &x1, &y1, &w1, &h1);
  	display.getTextBounds(Mode, 0, 0, &x1, &y1, &w2, &h2);
		int center = (128 - (w1+w2))/2;
		display.setCursor(center, 51);
		display.print("MODE: ");
		display.println(Mode);
		display.drawRoundRect(44,6,40,40,10,SSD1306_WHITE);
		animacija_slike(Slika, 32, 32, 48, 10, 3, false, false);
		display.drawTriangle(100, 18, 100, 32, 114, 25, SSD1306_WHITE);
		display.drawTriangle(28, 18, 28, 32, 14, 25, SSD1306_WHITE);

		display.display();
	}
	
	
	void animacija_slike (const unsigned char * ImeSlike, int sirina, int visina, int offset_x, int offset_y, int mode, bool ekran_clear, bool invert_image_colour){  // mode=1 -> vsak pixel posebaj, mode=2 -> vrstično, mode=3 -> v kosu
		
		if(ekran_clear == true){
			display.clearDisplay();
		}

		int stBajtov = (sirina*visina) / 8;
		for(int i=0; i<stBajtov; i++){
			uint8_t bajt = ImeSlike[i];

			for(int j=7;j>=0;j--){
				int bit = (bajt & (1<<j));
				int x = ((i*8) + 7-j) % sirina + offset_x;
				int y= (i*8/sirina) + offset_y;
				if(bit != 0 && invert_image_colour == false){
					display.drawPixel(x,y,SSD1306_WHITE);
					if(mode==1){
						display.display();
					}
				}else if(bit == 0 && invert_image_colour == true){
					display.drawPixel(x,y,SSD1306_WHITE);
					if(mode==1){
						display.display();
					}
				}

			}
			if(((i*8)%sirina==0) && mode==2){
				display.display();
			}
		}
		if(mode==3){
			display.display();
		}
		
	}

	void AnimacijaSlike_LevoDesno(const unsigned char* Slika1, int sirina, int visina, int offset_x, int offset_y, int hitrost_premika )  {  //misljeno za 32x32 sliko, hitrost 1-3
		int StartOffset = offset_x-4;
		while((offset_x-4) > 0){
			display.clearDisplay();
			display.drawRoundRect(offset_x-4,offset_y-4,visina+8,sirina+8,10,SSD1306_WHITE);
			display.drawBitmap(offset_x, offset_y, Slika1, sirina, visina, SSD1306_WHITE);
			display.display();
			if((offset_x-4-hitrost_premika) > 0){
				offset_x -= hitrost_premika ;
			}else {
				break;
			}
			
		}

		while((offset_x-4) < 128-(sirina+8)){
			display.clearDisplay();
			display.drawRoundRect(offset_x-4,offset_y-4,visina+8,sirina+8,10,SSD1306_WHITE);
			display.drawBitmap(offset_x, offset_y, Slika1, sirina, visina, SSD1306_WHITE);
			display.display();
			if(offset_x-4 + hitrost_premika < 128-(sirina+8)){
				offset_x += hitrost_premika;
			}else{
				break;
			}
			
		}

		while((offset_x-4) > StartOffset ){
			display.clearDisplay();
			display.drawRoundRect(offset_x-4,offset_y-4,visina+8,sirina+8,10,SSD1306_WHITE);
			display.drawBitmap(offset_x, offset_y, Slika1, sirina, visina, SSD1306_WHITE);
			display.display();
			if((offset_x-4-hitrost_premika) > 0){
				offset_x -= hitrost_premika ;
			}else {
				break;
			}
		}
	}

	void ModeSwitch(const unsigned char* Slika1, const unsigned char* Slika2, int sirina, int visina, int offset_x, int offset_y, int hitrost_premika){
		int Slika2_offset_x = (2*offset_x)+sirina+4;
		while((Slika2_offset_x-4) >= ((128-(sirina+8))/2)){
			display.clearDisplay();
			display.drawRoundRect(0, 0, 128, 64, 5, SSD1306_WHITE);
			display.drawRoundRect(offset_x-4,offset_y-4,visina+8,sirina+8,10,SSD1306_WHITE);
			display.drawBitmap(offset_x, offset_y, Slika1, sirina, visina, SSD1306_WHITE);
			display.drawRoundRect(Slika2_offset_x-4,offset_y-4,visina+8,sirina+8,10,SSD1306_WHITE);
			display.drawBitmap(Slika2_offset_x, offset_y, Slika2, sirina, visina, SSD1306_WHITE);
			display.display();
			offset_x -= hitrost_premika;
			Slika2_offset_x -= hitrost_premika;
			
		}
	}

	void Bmp180(){
		display.clearDisplay();
		display.setTextSize(1);
		display.setTextColor(SSD1306_WHITE);
		display.setCursor(0, 0);
		display.print("Temperatura: ");
		display.println(bmp.readTemperature()-7);
		display.print("RawTemp: : "); display.println(bmp.readRawTemperature());
		display.print("Pritisk: "); display.println(bmp.readPressure());
		display.print("Alt: "); display.println(bmp.readAltitude());
		display.display();
	}

	void Gps() {
		while(Hs.available() > 0){
			gps.encode(Hs.read());

				display.clearDisplay();
				display.setTextSize(1);
				display.setTextColor(SSD1306_WHITE);
				int16_t x1, y1;
				display.drawRoundRect(0, 0, 128, 64, 5, SSD1306_WHITE);
				
				
			if( gps.location.isValid()){
				
				uint16_t w2, h2, w3, h3, w4, h4, w5, h5, w6, h6, w7, h7;
				display.getTextBounds("S/", 0, 0, &x1, &y1, &w2, &h2);
				display.getTextBounds("A/m", 0, 0, &x1, &y1, &w3, &h3);
				display.getTextBounds("H/", 0, 0, &x1, &y1, &w4, &h4);
				display.getTextBounds("8", 0, 0, &x1, &y1, &w5, &h5);
				display.getTextBounds("300", 0, 0, &x1, &y1, &w6, &h6);
				display.getTextBounds("10", 0, 0, &x1, &y1, &w7, &h7);
				int razmak = ((128-(w2+w3+w4+w5+w6+w7))/4);
				int visina = 5 ;
				display.setCursor(razmak, visina);
				display.print("S/"); display.print(gps.satellites.value()); 
				display.setCursor(razmak+w2+w5+razmak, visina);
				display.print("A/"); display.print((int)gps.altitude.meters()); display.print("m ");
				display.setCursor(3*razmak+w2+w5+w3+w6, visina);
				display.print("H/"); display.println(gps.hdop.value());
				display.println("---------------------");
				display.print(" LAT "); display.print(gps.location.lat(), 4); display.println("N"); display.println(" ");
				display.print(" LON "); display.print(gps.location.lng(), 4);display.println("E");
			}else{
				uint16_t w2, h2, w3, h3, w4, h4, w5, h5, w6, h6, w7, h7;
				display.getTextBounds("S/", 0, 0, &x1, &y1, &w2, &h2);
				display.getTextBounds("A/m", 0, 0, &x1, &y1, &w3, &h3);
				display.getTextBounds("H/", 0, 0, &x1, &y1, &w4, &h4);
				display.getTextBounds("8", 0, 0, &x1, &y1, &w5, &h5);
				display.getTextBounds("300", 0, 0, &x1, &y1, &w6, &h6);
				display.getTextBounds("10", 0, 0, &x1, &y1, &w7, &h7);
				int razmak = ((128-(w2+w3+w4+w5+w6+w7))/4);
				int visina = 5 ;
				display.setCursor(razmak, visina);
				display.print("S/"); display.print(gps.satellites.value()); 
				display.setCursor(razmak+w2+w5+razmak, visina);
				display.print(" A/"); display.print((int)gps.altitude.meters()); display.print("m ");
				display.setCursor(3*razmak+w2+w5+w3+w6, visina);
				display.print("H/"); display.println(gps.hdop.value());
				display.println("---------------------");
				display.print(" LAT "); display.print("//.////"); display.println("N");display.println(" ");
				display.print(" LON "); display.print("//.////");display.println("E");
			}

			display.println("---------------------"); display.println("");

			if(gps.date.isValid()){
				uint16_t w8, w9, h8, h9;
				display.getTextBounds("29/10/2025", 0, 0, &x1, &y1, &w8, &h8);
				display.getTextBounds("23:48:15", 0, 0, &x1, &y1, &w9, &h9);
				int raz= (128-w8-w9)/3;
				int vis= 53;
				display.setCursor(raz, vis);
				display.print(gps.date.day()); display.print("/");
				display.print(gps.date.month()); display.print("/");
				display.print(gps.date.year());
				display.setCursor(raz+w8+raz, vis);
				display.print(gps.time.hour()+1); display.print(":");
				display.print(gps.time.minute()); display.print(":");
				display.println(gps.time.second()); 
				display.drawBitmap(93 ,20 ,SatDish_bitmap, 25, 25, SSD1306_WHITE);
			}else{
				uint16_t w8, w9, h8, h9;
				display.getTextBounds("29/10/2025", 0, 0, &x1, &y1, &w8, &h8);
				display.getTextBounds("23:48:15", 0, 0, &x1, &y1, &w9, &h9);
				int raz= (128-w8-w9)/3;
				int vis= 53;
				display.setCursor(raz, vis);
				display.print("::/::/::::");
				display.setCursor(raz+w8+raz, vis);
				display.println("//://://"); 
				display.drawBitmap(93 ,20 ,SatDish_bitmap, 25, 25, SSD1306_WHITE);
			}
			display.display();
		}
	
	}
