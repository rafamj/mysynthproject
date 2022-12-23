//wav.h
#include <string>
using namespace std;

typedef struct wav_header {
    char riff_header[4]; // Contains "RIFF"
    int wav_size; // Size of the wav portion of the file, which follows the first 8 bytes. File size - 8
    char wave_header[4]; // Contains "WAVE"
    // uint8_t bytes[]; // Remainder of wave file is bytes
} wav_header;

typedef struct fmt {
    short audio_format; // Should be 1 for PCM. 3 for IEEE Float
    short num_channels;
    int sample_rate;
    int byte_rate; // Number of bytes per second. sample_rate * num_channels * Bytes Per Sample
    short sample_alignment; // num_channels * Bytes Per Sample
    short bit_depth; // Number of bits per sample
} fmt;

typedef struct  data {
  char data_header[4]; // Contains "data"
  int data_bytes; // Number of bytes in data. Number of samples * num_channels * sample byte size
} data;

typedef struct junk {
  char junk_header[4];
  int junk_bytes;
} junk;

unsigned char *readWav(size_t &l ,int &type, string filename);
