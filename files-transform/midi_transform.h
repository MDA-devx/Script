#ifndef MIDI_TRANSFORM_H
#define MIDI_TRANSFORM_H

#include <vector>

// Function to transform MIDI messages
std::vector<unsigned char> transform_midi_message(const std::vector<unsigned char> &message);

#endif // MIDI_TRANSFORM_H