#include <iostream>
#include <cstdlib>
#include <RtMidi.h>

// Function to handle incoming MIDI messages
void midiCallback(double deltatime, std::vector<unsigned char> *message, void *userData) {
    RtMidiOut *midiOut = static_cast<RtMidiOut*>(userData);
    midiOut->sendMessage(message);
}

int main() {
    RtMidiIn *midiIn = nullptr;
    RtMidiOut *midiOut = nullptr;

    try {
        // Initialize MIDI input and output
        midiIn = new RtMidiIn();
        midiOut = new RtMidiOut();

        // Check available input ports.
        unsigned int nPorts = midiIn->getPortCount();
        if (nPorts == 0) {
            std::cout << "No MIDI input ports available!\n";
            goto cleanup;
        }

        // Open the first available input port.
        midiIn->openPort(0);

        // Check available output ports.
        nPorts = midiOut->getPortCount();
        if (nPorts == 0) {
            std::cout << "No MIDI output ports available!\n";
            goto cleanup;
        }

        // Open the first available output port.
        midiOut->openPort(0);

        // Set our callback function to handle incoming MIDI messages
        midiIn->setCallback(&midiCallback, midiOut);

        // Ignore sysex, timing, and active sensing messages
        midiIn->ignoreTypes(true, true, true);

        std::cout << "Reading MIDI input and sending to output... Press Ctrl+C to quit.\n";

        // Keep the application running
        while (true) {
            // Sleep to prevent high CPU usage
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }

    cleanup:
        delete midiIn;
        delete midiOut;
    } catch (RtMidiError &error) {
        error.printMessage();
        if (midiIn) delete midiIn;
        if (midiOut) delete midiOut;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}