// Okay, we need to include some system headers:
#include <SPI.h>
#include <SD.h>

// Right, now my headers:
#include <opcode-base.h>
#include <boot-splash.h>

void setup() {
  // Boot init, kernel loading
  boot();
}

void loop() {
  // Nothing to see here...
}
