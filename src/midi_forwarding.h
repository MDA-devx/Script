#ifndef MIDI_FORWARDING_H
#define MIDI_FORWARDING_H

#include <RtMidi.h>

extern bool should_exit;

void forward_uart_to_midi(int uart_fd, RtMidiOut &midi_out);
void forward_midi_to_midi(RtMidiIn &midi_in, RtMidiOut &midi_out);

#endif // MIDI_FORWARDING_H