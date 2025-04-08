#include <iostream>
#include <signal.h>
#include <thread>
#include "uart_utils.h"
#include "midi_forwarding.h"
#include <RtMidi.h>

void signal_handler(int signal) {
  should_exit = true;
}

int main() {
  RtMidiIn midi_in;
  RtMidiOut midi_out;

  try {
    // Open UART for MIDI input
    const char *uart_device = "/dev/serial0"; // Adjust if necessary
    const int baud_rate = B31250;            // MIDI baud rate
    int uart_fd = setup_uart(uart_device, baud_rate);
    if (uart_fd < 0) {
      return 1;
    }

    // Open the first available MIDI input port
    if (midi_in.getPortCount() == 0) {
      std::cerr << "No MIDI input ports available." << std::endl;
      close(uart_fd);
      return 1;
    }
    midi_in.openPort(0);

    // Open the first available MIDI output port
    if (midi_out.getPortCount() == 0) {
      std::cerr << "No MIDI output ports available." << std::endl;
      close(uart_fd);
      return 1;
    }
    midi_out.openPort(0);

    // Register signal handler for Ctrl+C
    signal(SIGINT, signal_handler);

    std::cout << "Forwarding MIDI messages from UART and ALSA MIDI. Press Ctrl+C to exit." << std::endl;

    // Create threads for UART-to-MIDI and MIDI-to-MIDI forwarding
    std::thread uart_thread(forward_uart_to_midi, uart_fd, std::ref(midi_out));
    std::thread midi_thread(forward_midi_to_midi, std::ref(midi_in), std::ref(midi_out));

    // Wait for threads to finish
    uart_thread.join();
    midi_thread.join();

    std::cout << "Exiting..." << std::endl;

    // Cleanup
    close(uart_fd);

  } catch (RtMidiError &error) {
    error.printMessage();
    return 1;
  }

  return 0;
}