/*  LibWAV - a library for reading and writing wave sound file data
    - Paul Denny
    - October 2008
*/

/* The maximum size (in bytes) supported */
#define MAX_CHUNKS 10

/* Values in the range 0-255 */
typedef unsigned char byte;

/* Function prototype declarations */
void LoadWavFile(char *filename);
void PrintFormat(void);
void SaveWavFile(char *filename);
int GetSamples(byte *samples);
void SetSamples(byte *samples, int size);
int GetSamplesCount();
int GetBitsPerSample();
int GetChannelCount();
int FillWithNormalizedValues(float* audio);
int ScanWord(byte *p);
int ScanDoubleWord(byte *p);
int ReturnPostConvSamplesCount();
