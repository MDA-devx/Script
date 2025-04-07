#include <iostream>
#include <cstdlib>
#include <signal.h>
#include <vector>
#include <thread>
#include <wiringSerial.h>
#include <RtMidi.h>

bool should_exit = false;

void signal_handler(int signal) {
  should_exit = true;
}

void forward_uart_to_midi(int uart_fd, RtMidiOut &midi_out) {
  try {
    while (!should_exit) {
      if (serialDataAvail(uart_fd)) {
        unsigned char midi_byte = serialGetchar(uart_fd);

        // Forward the MIDI byte to ALSA MIDI output
        std::vector<unsigned char> message = {midi_byte};
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

int main() {
  RtMidiIn midi_in;
  RtMidiOut midi_out;
  int uart_fd;

  try {
    // Open UART for MIDI input
    const char *uart_device = "/dev/serial0"; // Adjust if necessary
    const int baud_rate = 31250;             // MIDI baud rate
    if ((uart_fd = serialOpen(uart_device, baud_rate)) < 0) {
      std::cerr << "Failed to open UART device: " << uart_device << std::endl;
      return 1;
    }

    // Open the first available MIDI input port
    if (midi_in.getPortCount() == 0) {
      std::cerr << "No MIDI input ports available." << std::endl;
      serialClose(uart_fd);
      return 1;
    }
    midi_in.openPort(0);

    // Open the first available MIDI output port
    if (midi_out.getPortCount() == 0) {
      std::cerr << "No MIDI output ports available." << std::endl;
      serialClose(uart_fd);
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

  } catch (RtMidiError &error) {
    error.printMessage();
    return 1;
  }

  // Cleanup
  serialClose(uart_fd);

  return 0;
}