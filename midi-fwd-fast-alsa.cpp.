#include <iostream>
#include <cstdlib>
#include <signal.h>
#include "RtMidi.h"

bool should_exit = false;

void signal_handler(int signal) {
  should_exit = true;
}

int main() {
  RtMidiIn midi_in;
  RtMidiOut midi_out;

  try {
    // Abre el primer puerto de entrada MIDI disponible
    if (midi_in.getPortCount() == 0) {
      std::cout << "No hay puertos de entrada MIDI disponibles." << std::endl;
      return 1;
    }
    midi_in.openPort(0);

    // Abre el primer puerto de salida MIDI disponible
    if (midi_out.getPortCount() == 0) {
      std::cout << "No hay puertos de salida MIDI disponibles." << std::endl;
      return 1;
    }
    midi_out.openPort(0);

    // Registra el manejador de señales para la interrupción (Ctrl+C)
    signal(SIGINT, signal_handler);

    std::cout << "Reenviando mensajes MIDI. Presiona Ctrl+C para salir." << std::endl;

    // Bucle principal para leer y reenviar mensajes MIDI
    std::vector<unsigned char> message;
    while (!should_exit) {
      double timestamp = midi_in.getMessage(&message);
      if (message.size() > 0) {
        midi_out.sendMessage(&message);
      }
    }

    std::cout << "Saliendo..." << std::endl;

  } catch (RtMidiError &error) {
    error.printMessage();
    return 1;
  }

  return 0;
}