Exercise 2: Process Full Audio File

Goal: Handle the entire audio stream, not just 100 bytes.

Tasks:

     - Decode ALL A-law samples from the WAV file into int16_t PCM
     - Store decoded samples in a dynamically allocated buffer
     - Calculate statistics: min/max/average amplitude for each channel
     - Write decoded PCM data to a new .raw file

     # Compare file sizes
     ls -l audiosamples/M1F1-AlawWE-AFsp.wav src/data.raw

     # Decode with ffmpeg to compare
     ffmpeg -f alaw -ar 8000 -ac 2 -i src/data.raw -f s16le reference.raw

Key Learning: Memory management for large buffers, efficient file I/O

---

Exercise 3: Understand Audio Frames & Resampling

Goal: MP3 encoders work with specific frame sizes and sample rates.

Tasks:

     - Group your PCM samples into 1152-sample frames (standard MP3 frame size)
     - Research: Your WAV is 8kHz - MP3 typically uses 44.1kHz. Implement basic upsampling (duplicate samples or linear interpolation)
     - Organize samples by channel (de-interleave stereo: separate left/right)

Key Learning: Audio frame structure, sample rate conversion basics

---

Exercise 4: Integrate LAME MP3 Encoder

Goal: Actually create MP3 files!

Tasks:

     - Install LAME library: sudo apt-get install libmp3lame-dev
     - Initialize LAME encoder with your audio parameters
     - Feed your decoded PCM frames to lame_encode_buffer()
     - Write MP3 output to file
     - Verify with: ffplay output.mp3

Key Learning: Third-party library integration, codec APIs

---

Exercise 5: Command-Line Tool

Goal: Make it a real utility.

Tasks:

     - Add argument parsing: ./wav2mp3 input.wav output.mp3
     - Add error handling for unsupported formats
     - Add progress indicator (percentage complete)
     - Support both A-law and μ-law WAV files

Key Learning: User interface, robustness

---

Quick Fixes for Current Code:

     - Line 97: You're calling alaw() on int16_t but it expects uint8_t - cast to (uint8_t)
     - Line 133: You dereference header after freeing it - move readaudiodata() call before free(header)
     - Line 10 in decoding_algorithm.c: mantissma should use the actual mantissa bits: alaw_byte & 0x0F
