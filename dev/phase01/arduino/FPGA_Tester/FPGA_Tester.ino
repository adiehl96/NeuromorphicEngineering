
#include "FPGA_Controller.h"

void setup() {

	Serial.begin(115200); // Serial.begin() should be called before uploadBitstream

	// Upload the bitstream to the FPGA
	uploadBitstream();

	while(!Serial);

	Serial.println("Welcome!");

	initJTAG();
}

char rx_byte = 0;
uint32_t initial_vertex = 0;
uint32_t terminal_vertex = 1;
uint32_t clr = 0;

void loop() {

  clr = 1;
  writeJTAG(2, clr);

  while(Serial.available() == 0) {
  }

  //////////////////////////////////////////////////////// Choose Initial Vertex
  rx_byte = Serial.read();       // get the character

  // check if a number was received
  if ((rx_byte >= '0') && (rx_byte <= '9')) {
    Serial.print("Number received: ");
    Serial.println(rx_byte);
    initial_vertex = rx_byte - 48;
  }
  else {
    Serial.println("Not a number.");
    initial_vertex = 0;
  }

  Serial.print("start vertex: "); Serial.println(initial_vertex);
  writeJTAG(1, initial_vertex);

  //////////////////////////////////////////////////////// Choose Terminal Vertex
  rx_byte = Serial.read();       // get the character
  
  // check if a number was received
  if ((rx_byte >= '0') && (rx_byte <= '9')) {
    Serial.print("Number received: ");
    Serial.println(rx_byte);
    terminal_vertex = rx_byte - 48;
  }
  else {
    Serial.println("Not a number.");
    terminal_vertex = 1;
  }

  while(Serial.available()) {Serial.read();}
  
  Serial.print("terminal_vertex: "); Serial.println(terminal_vertex);
  writeJTAG(0, terminal_vertex);

  //////////////////////////////////////////////////////// Start the circuit

  clr = 0;
  writeJTAG(2, clr);
  
	delay(1); // given that we are using a 120 MegaHertz clock, this delay expressed in milliseconds is too large even if it's only set to 1.

  //////////////////////////////////////////////////////// Read out the shortest path length
  uint32_t shortestLength = readJTAG(0);
  Serial.print("Shortest path length from v"); Serial.print(initial_vertex); Serial.print(" to v"); Serial.print(terminal_vertex); Serial.print(": "); Serial.println(shortestLength);
}
