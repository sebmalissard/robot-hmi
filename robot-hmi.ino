#include "Max7219.h"

Max7219 display(4);

void setup() {
  // put your setup code here, to run once:

}

void loop() {
  display.printString("pret");
  delay(2000);
  display.setIntensity(255);
  delay(1000);
  display.setIntensity(0);
  delay(1000);
  display.setIntensity(60);
  delay(1000);
  display.setIntensity(128);
}
