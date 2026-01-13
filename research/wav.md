# RIFF shape of a wav file: (source from wikipedia)

[Master RIFF chunk]
FileTypeBlocID (4 bytes) : Identifier « RIFF » (0x52, 0x49, 0x46, 0x46)
FileSize (4 bytes) : Overall file size minus 8 bytes
FileFormatID (4 bytes) : Format = « WAVE » (0x57, 0x41, 0x56, 0x45)

[Chunk describing the data format]
FormatBlocID (4 bytes) : Identifier « fmt␣ » (0x66, 0x6D, 0x74, 0x20)
BlocSize (4 bytes) : Chunk size minus 8 bytes, which is 16 bytes here (0x10)
AudioFormat (2 bytes) : Audio format (1: PCM integer, 3: IEEE 754 float)
NbrChannels (2 bytes) : Number of channels
Frequency (4 bytes) : Sample rate (in hertz)
BytePerSec (4 bytes) : Number of bytes to read per second (Frequency _BytePerBloc).
BytePerBloc (2 bytes) : Number of bytes per block (NbrChannels_ BitsPerSample / 8).
BitsPerSample (2 bytes) : Number of bits per sample

[Chunk containing the sampled data]
DataBlocID (4 bytes) : Identifier « data » (0x64, 0x61, 0x74, 0x61)
DataSize (4 bytes) : SampledData size
SampledData
