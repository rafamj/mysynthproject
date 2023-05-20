I am putting here my ALSA program in order to have a backup.

This also serves as an Arduino library for the SAMD21 board with the PCM5102A DAC

    PCM5102A   SAMD21
    Vcc        5V
    GND        GND
    FLT
    DMP
    SCL        GND
    BCK        D1
    DIN        D9
    LCK        D0
    FMT        GND
    XMT        3.3 V

    MIDI IN  D11
    
    ./modular <program> to exec the program
    ./modular -c <program> to list the SAMD21 synth.ino program
    ./modular -p <program> to list the synth.cc program
