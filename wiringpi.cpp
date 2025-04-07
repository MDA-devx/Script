#include <iostream>
#include <cstdlib>
#include <signal.h>
#include <vector>
#include <thread>
#include <pigpio.h> // Include pigpio for UART
#include <RtMidi.h>

bool should_exit = false;

void signal_handler(int signal) {
  should_exit = true;
}

void forward_uart_to_midi(int uart_handle, RtMidiOut &midi_out) {
  try {
    while (!should_exit) {
      char midi_byte;
      int bytes_read = gpioSerialRead(uart_handle, &midi_byte, 1); // Read 1 byte from UART
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

int main() {
  RtMidiIn midi_in;
  RtMidiOut midi_out;

  try {
    // Initialize pigpio
    if (gpioInitialise() < 0) {
      std::cerr << "Failed to initialize pigpio." << std::endl;
      return 1;
    }

    // Open UART for MIDI input
    const char *uart_device = "/dev/serial0"; // Adjust if necessary
    const int baud_rate = 31250;             // MIDI baud rate
    int uart_handle = gpioSerialOpen(uart_device, baud_rate);
    if (uart_handle < 0) {
      std::cerr << "Failed to open UART device: " << uart_device << std::endl;
      gpioTerminate();
      return 1;
    }

    // Open the first available MIDI input port
    if (midi_in.getPortCount() == 0) {
      std::cerr << "No MIDI input ports available." << std::endl;
      gpioSerialClose(uart_handle);
      gpioTerminate();
      return 1;
    }
    midi_in.openPort(0);

    // Open the first available MIDI output port
    if (midi_out.getPortCount() == 0) {
      std::cerr << "No MIDI output ports available." << std::endl;
      gpioSerialClose(uart_handle);
      gpioTerminate();
      return 1;
    }
    midi_out.openPort(0);

    // Register signal handler for Ctrl+C
    signal(SIGINT, signal_handler);

    std::cout << "Forwarding MIDI messages from UART and ALSA MIDI. Press Ctrl+C to exit." << std::endl;

    // Create threads for UART-to-MIDI and MIDI-to-MIDI forwarding
    std::thread uart_thread(forward_uart_to_midi, uart_handle, std::ref(midi_out));
    std::thread midi_thread(forward_midi_to_midi, std::ref(midi_in), std::ref(midi_out));

    // Wait for threads to finish
    uart_thread.join();
    midi_thread.join();

    std::cout << "Exiting..." << std::endl;

    // Cleanup
    gpioSerialClose(uart_handle);
    gpioTerminate();

  } catch (RtMidiError &error) {
    error.printMessage();
    return 1;
  }

  return 0;
}