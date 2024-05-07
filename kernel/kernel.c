// Default libs

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
 
// drivers
#include "../drivers/io/io.h"
#include "../drivers/vga/vga.h"


// for dev, will move later
void err(char *str) {
  print("Error: ");
  printf(str);
}




#include "../drivers/keyboard/keyboard.h"

void main() {
  // disable cursor?
  vga_init();
  
  printf("Welcome to Nox!");
  printf("\nThis is the beginnings of an amateur-made OS. At the moment, it can't do much.");
  printf("\nHopefully soon it can support some kind of filesystem, and commands, but in the meantime, enjoy.\n");
  print("> ");
  enableKeyboard();

  while (true) {
    loop();
  }
}

void loop() {
  //getKeyboardInput();
  handleKeyboardInput();
}
