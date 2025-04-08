#include "midi_forwarding.h"
#include <iostream>
#include <vector>
#include <unistd.h>

bool should_exit = false;

void forward_uart_to_midi(int uart_fd, RtMidiOut &midi_out) {
  try {
    while (!should_exit) {
      char midi_byte;
      int bytes_read = read(uart_fd, &midi_byte, 1); // Read 1 byte from UART
      if (bytes_read > 0) {
        // Forward the MIDI byte to ALSA MIDI output
        std::vector<unsigned char> message = {static_cast<unsigned char>(midi_byte)};
        midi_out.sendMessage(&message);
      }
    }
  } catch (std::exception &e) {
    std::cerr << "Error in UART to MIDI forwarding: " << e.what() << std::endl;
  }
}

void forward_midi_to_midi(RtMidiIn &midi_in, RtMidiOut &midi_out) {
  try {
    std::vector<unsigned char> message;
    while (!should_exit) {
      double timestamp = midi_in.getMessage(&message);
      if (!message.empty()) {
        // Forward the MIDI message to ALSA MIDI output
        midi_out.sendMessage(&message);
      }
    }
  } catch (RtMidiError &error) {
    error.printMessage();
  }
}