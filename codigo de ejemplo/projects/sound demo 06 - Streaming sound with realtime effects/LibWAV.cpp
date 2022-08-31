#define _CRT_SECURE_NO_DEPRECATE

#include "LibWAV.h"
#include <stdio.h>
#include <stdlib.h>


/* Prototype declartions for input functions.  GetByte() returns a 1-byte sequence as an int,
   GetWord() returns a 2-byte sequence as an int, GetDoubleWord() returns a 4-byte sequence as an int.
   These functions assume little-endianness of file
*/
static int GetByte(FILE *fp);
static int GetWord(FILE *fp);
static int GetDoubleWord(FILE *fp);
int ScanWord(byte *p);
int ScanDoubleWord(byte *p);
static void SaveDoubleWord(int value, byte *p);

/*
	The global data for storing the WAVE chunks
*/
static byte *chunk[MAX_CHUNKS];
static int chunkSize[MAX_CHUNKS];
static char chunkID[MAX_CHUNKS][4];
static int chunkCount;
static int formatChunk, dataChunk;


bool float_fmt_file_used;
bool isFloatFmtUsed()
{
	return  float_fmt_file_used;
}
FILE *fptr;
void LoadWavFile(char* filename)
{
  float_fmt_file_used = false;


	
	int bytePos, fileSize;

	chunkCount = 0;
	formatChunk = -1;
	dataChunk = -1;

	fptr = fopen(filename, "rb");
	if (fptr == NULL) 
	{
		printf("Could not open input file: \"%s\"\n", filename);
		exit(EXIT_FAILURE);
	}
	

	// Read the initial four bytes - which should contain "RIFF":
	(void)fread(chunkID[chunkCount], 1, 4, fptr);

	// Get file size
	(void)fseek(fptr, 4, SEEK_SET);
	fileSize = GetDoubleWord(fptr) + 8;	// Add 8 to get the total length in bytes


	// Read the initial chunk into memory
	(void)fseek(fptr, 0, SEEK_SET);
	chunk[chunkCount] = (byte*)malloc((size_t)12);
	(void)fread(chunk[chunkCount], 1, (size_t)12, fptr);
	chunkSize[chunkCount] = 12;
	chunkCount++;






	// Read all chunks in file
	bytePos = 12;
	while (bytePos < fileSize) {

		// Read chunk ID, and record index of format chunk and data chunk
		(void)fseek(fptr, bytePos, SEEK_SET);
		(void)fread(chunkID[chunkCount], 1, 4, fptr);
		if (chunkID[chunkCount][0] == 'f' && chunkID[chunkCount][1] == 'm' && chunkID[chunkCount][2] == 't' && chunkID[chunkCount][3] == ' ')
			formatChunk = chunkCount;
		if (chunkID[chunkCount][0] == 'd' && chunkID[chunkCount][1] == 'a' && chunkID[chunkCount][2] == 't' && chunkID[chunkCount][3] == 'a')
			dataChunk = chunkCount;

		// Read chunk size
		(void)fseek(fptr, bytePos + 4, SEEK_SET);
		chunkSize[chunkCount] = GetDoubleWord(fptr) + 8;

		// Read chunk data
		(void)fseek(fptr, bytePos, SEEK_SET);
		chunk[chunkCount] = (byte*)malloc((size_t)chunkSize[chunkCount]);
		(void)fread(chunk[chunkCount], 1, (size_t)chunkSize[chunkCount], fptr);
		bytePos = bytePos + chunkSize[chunkCount];

		// If this is the format chunk, then read the encoding - must ensure no compression is used:
		if (formatChunk == chunkCount) {
			if (ScanWord(chunk[formatChunk] + 8) != 1) 
			{
			float_fmt_file_used = true;

			//	printf("This is not a PCM encoded WAVE file: \"%s\"\n", filename);
	//			exit(EXIT_FAILURE);
			}
		}
		chunkCount++;
	}

	(void)fclose(fptr);
}

int GetBitsPerSample()
{
  return  ScanWord(chunk[formatChunk] + 22);
}

int GetChannelCount()
{
  return ScanWord(chunk[formatChunk] + 10);
}

void PrintFormat(void)
{
	int i;

	int format, channels, sampleRate, bitsPerSample;

	printf("This .wav file consists of the following chunks:\n");

	for (i = 0; i < chunkCount; i++) {
		printf("  %c%c%c%c\n", chunkID[i][0], chunkID[i][1], chunkID[i][2], chunkID[i][3]);
	}

	format = ScanWord(chunk[formatChunk] + 8);
	printf("Audio format:       %d (this should be 1 for uncompressed PCM encoding)\n", format);

	channels = ScanWord(chunk[formatChunk] + 10);
	printf("Number of channels: %d\n", channels);

	sampleRate = ScanDoubleWord(chunk[formatChunk] + 12);
	printf("Sample rate:        %d Hz\n", sampleRate);

	bitsPerSample = ScanWord(chunk[formatChunk] + 22);
	printf("Bits Per Sample:    %d\n", bitsPerSample);
}

int GetSamplesCount()
{
	return chunkSize[dataChunk] - 8;
}

int GetSamples(byte* samples)
{
	int i;
	int size = chunkSize[dataChunk] - 8;

	for (i = 0; i < size; i++) {
		samples[i] = chunk[dataChunk][i + 8];
	}

	return size;
}

void SetSamples(byte* samples, int size)
{
	byte* temp;
	int i;
	temp = (byte*)malloc((size_t)size + 8);

	// Set the initial 8 bytes of this chunk:
	for (i = 0; i < 4; i++)
		temp[i] = chunk[dataChunk][i];
	SaveDoubleWord(size, temp + 4);

	// Copy remaining data
	for (i = 0; i < size; i++)
		temp[i+8] = samples[i];

	chunk[dataChunk] = temp;
	chunkSize[dataChunk] = size + 8;
}


void SaveWavFile(char* filename)
{
	FILE* fptr;
	int i;
	int fileSize;

	// Write the chunks out to file:
	fptr = fopen(filename, "wb");
	if (fptr == NULL) {
		printf("Could not open output file: \"%s\"\n", filename);
		exit(EXIT_FAILURE);
	}

	// The file size may have changed if the data chunk was updated.  The data chunk size field
	// will have been updated already - we may need to adjust the RIFF chunk size field:

	fileSize = 0;
	for (i = 0; i < chunkCount; i++) {
		fileSize += chunkSize[i];
	}

	printf("Output file size:   %d\n\n", fileSize);
	SaveDoubleWord(fileSize - 8, chunk[0] + 4);

	for (i = 0; i < chunkCount; i++)
		(void)fwrite(chunk[i], 1, chunkSize[i], fptr);

	/* Close files */
	(void)fclose(fptr);

	/* Release memory */
	for (i = 0; i < chunkCount; i++)
		free(chunk[i]);

}


void GetSampleIndices(byte* wavData, int *start, int* end)
{

	char chunkID[4];
	int numBytes, dw;
	int pos;


	pos = 4;

	dw =  (int) (wavData[pos] & 0xFF);
	dw |= ((int) (wavData[pos+1] & 0xFF) << 0x08);
	dw |= ((int) (wavData[pos+2] & 0xFF) << 0x10);
	dw |= ((int) (wavData[pos+3] & 0xFF) << 0x18);

	numBytes = dw + 8;

	pos = 12;

	while (pos < numBytes) {

		chunkID[0] = wavData[pos];
		chunkID[1] = wavData[pos+1];
		chunkID[2] = wavData[pos+2];
		chunkID[3] = wavData[pos+3];

		printf("Chunk ID: %c%c%c%c ", chunkID[0], chunkID[1], chunkID[2], chunkID[3]);

		dw =  (int) (wavData[pos+4] & 0xFF);
		dw |= ((int) (wavData[pos+5] & 0xFF) << 0x08);
		dw |= ((int) (wavData[pos+6] & 0xFF) << 0x10);
		dw |= ((int) (wavData[pos+7] & 0xFF) << 0x18);

		printf("Chunk size = %d\n", dw);

		if ((chunkID[0] == 'd') && (chunkID[1] == 'a') && (chunkID[2] == 't') && (chunkID[3] == 'a')) {
			*start = pos + 8;
			*end = pos + dw + 8;
			return;
		}

		pos = pos + dw + 8;
	}
}

static int GetByte(FILE *fp)
{
	int w;
	w =  (int) (fgetc(fp) & 0xFF);
	return(w);
}

static int GetWord(FILE *fp)
{
	int w;
	w =  (int) (fgetc(fp) & 0xFF);
	w |= ((int) (fgetc(fp) & 0xFF) << 0x08);
	return(w);
}

static int GetDoubleWord(FILE *fp)
{
	int dw;
	dw =  (int) (fgetc(fp) & 0xFF);
	dw |= ((int) (fgetc(fp) & 0xFF) << 0x08);
	dw |= ((int) (fgetc(fp) & 0xFF) << 0x10);
	dw |= ((int) (fgetc(fp) & 0xFF) << 0x18);
	return(dw);
}

int ScanWord(byte *p)
{
	int w;
	w =  (int) (p[0] & 0xFF);
	w |= ((int) (p[1] & 0xFF) << 0x08);
	return(w);
}

int ScanDoubleWord(byte *p)
{
	int dw;
	dw =  (int) (p[0] & 0xFF);
	dw |= ((int) (p[1] & 0xFF) << 0x08);
	dw |= ((int) (p[2] & 0xFF) << 0x10);
	dw |= ((int) (p[3] & 0xFF) << 0x18);
	return(dw);
}

static void SaveDoubleWord(int value, byte *p)
{
	p[3] = (byte)((value >> 0x18) & 0xFF);
	p[2] = (byte)((value >> 0x10) & 0xFF);
	p[1] = (byte)((value >> 0x08) & 0xFF);
	p[0] = (byte)((value) & 0xFF);
}

int ReturnPostConvSamplesCount()
{
	int smpcnt =  GetSamplesCount();
    int out_smpcnt;
	//byte *data = new byte[smpcnt];
	//GetSamples(data);

	if (GetChannelCount() == 1)
	{
	  if (GetBitsPerSample() == 8)
	  {
		  out_smpcnt = smpcnt;
	  }
	  else if (GetBitsPerSample() == 16)
  	  {
		int f_out = 0;
	    for (int f = 0; f < smpcnt; f+=2)
		{
          f_out++;
		}
	    out_smpcnt = f_out;
	  }
	  else if (GetBitsPerSample() == 24)
  	  {
		  printf("mono, bits per sample: 24\n");
			  
		  int f_out = 0;
		  for (int f = 0; f < smpcnt; f+=3)
		  {
  		   f_out++;
		  }
		  out_smpcnt = f_out;
	  }
	  else if (GetBitsPerSample() == 32)
	  {
	
		  if (isFloatFmtUsed() == false)
		  {				
			  int f_out = 0;
			  for (int f = 0; f < smpcnt; f+=4)
			  {
				  f_out++;
			  }
			  out_smpcnt = f_out;
		  }
		  else
		  {
			  int f_out = 0;
			  for (int f = 0; f < smpcnt; f+=4)
			  {
				  f_out++;
			  }
			  out_smpcnt = f_out;				
		  }
		  
	  }
	}
	else if (GetChannelCount() == 2)
	{
		if (GetBitsPerSample() == 8)
		{
		    int f_out = 0;
			for (int f = 0; f < smpcnt; f+=2)
				f_out++;
			
			out_smpcnt = f_out;
		}	
		else if (GetBitsPerSample() == 16)
		{
			int f_out = 0;
			for (int f = 0; f < smpcnt; f+=4)
			{
				f_out++;
			}
			out_smpcnt = f_out;
		}
		else if (GetBitsPerSample() == 24)
		{
			int f_out = 0;
			for (int f = 0; f < smpcnt; f+=3*2)
			{
				f_out++;
			}
			out_smpcnt = f_out;
		}	
		else if (GetBitsPerSample() == 32)
		{
			if (isFloatFmtUsed() == false)
			{				
			  int f_out = 0;
			  for (int f = 0; f < smpcnt; f+=4*2)
			  {
				f_out++;
			  }
			  out_smpcnt = f_out;
			}
			else
			{
				int f_out = 0;
				for (int f = 0; f < smpcnt; f+=4*2)
				{
					f_out++;
				}
				out_smpcnt = f_out;				
			}
		}		
	}
	else
	{
		printf("Only 1 and 2 channels supported for .wav files\n");
	}
	
	return out_smpcnt;
}

// returns sample cnt;
int FillWithNormalizedValues(float* audio)
{
	int smpcnt =  GetSamplesCount();
    int out_smpcnt;
	byte *data = new byte[smpcnt];
	GetSamples(data);

	if (GetChannelCount() == 1)
	{
	  if (GetBitsPerSample() == 8)
	  {
	    for (int f = 0; f < smpcnt; f++)
	     audio[f] = data[f] /255.0f;
		  
		  out_smpcnt = smpcnt;
	  }
	  else if (GetBitsPerSample() == 16)
  	  {
		printf("mono, bits per sample: 16\n");
		int f_out = 0;
	    for (int f = 0; f < smpcnt; f+=2)
		{
  		  short int *si;
		  si = (short int*) &data[f];
		  audio[f_out++] = *si / (255.0f*255.0f);
		}
	    out_smpcnt = f_out;
	  }
	  else if (GetBitsPerSample() == 24)
  	  {
		  printf("mono, bits per sample: 24\n");
			  
		  int f_out = 0;
		  for (int f = 0; f < smpcnt; f+=3)
		  {
			  char ccc[4];
			  // f0, f1, f2, 0,
			  // 0, f2, f1, f0
			  ccc[0] = 0;
			  ccc[1] = data[f+0];
			  ccc[2] = data[f+1];
			  ccc[3] = data[f+2];
			  int *ii = (int*) &ccc[0];
  		    audio[f_out++] = *ii / (255.0f*255.0f*255.0f*255.0f);
		  }
		  out_smpcnt = f_out;
	  }
	  else if (GetBitsPerSample() == 32)
	  {
	    printf("mono, bits per sample: 32\n");
		/*
		 int f_out = 0;
		for (int f = 0; f < smpcnt; f+=4)
		{
		  int *si;
		  si = (int*) &data[f];
		  audio[0][f_out++] = *si / (255.0f*255.0f*255.0f*255.0f);
		}
	    out_smpcnt = f_out;
		 */
		  if (isFloatFmtUsed() == false)
		  {				
			  int f_out = 0;
			  for (int f = 0; f < smpcnt; f+=4)
			  {
				  int *si;
				  si = (int*) &data[f];
				  audio[f_out++] = *si / (255.0f*255.0f*255.0f*255.0f);
			  }
			  out_smpcnt = f_out;
		  }
		  else
		  {
			  printf("float format used...\n");
			  int f_out = 0;
			  for (int f = 0; f < smpcnt; f+=4)
			  {
				  float *si;
				  si = (float*) &data[f];
				  audio[f_out++] = *si;
			  }
			  out_smpcnt = f_out;				
		  }
		  
	  }
	}
	else if (GetChannelCount() == 2)
	{
		if (GetBitsPerSample() == 8)
		{
			printf("stereo, bits per sample: 8\n");
		    int f_out = 0;
			for (int f = 0; f < smpcnt; f+=2)
				audio[f_out++] = data[f] /255.0f;
			
			out_smpcnt = f_out;
		}	
		else if (GetBitsPerSample() == 16)
		{
			printf("stereo, bits per sample: 16\n");
			int f_out = 0;
			for (int f = 0; f < smpcnt; f+=4)
			{
				short int *si;
				si = (short int*) &data[f];
				audio[f_out++] = *si / (255.0f*255.0f);
			}
			out_smpcnt = f_out;
		}
		else if (GetBitsPerSample() == 24)
		{
			printf("stereo, bits per sample: 24\n");
			
			int f_out = 0;
			for (int f = 0; f < smpcnt; f+=3*2)
			{
				char ccc[4];
				// f0, f1, f2, 0,
				// 0, f2, f1, f0
				ccc[0] = 0;
				ccc[1] = data[f+0];
				ccc[2] = data[f+1];
				ccc[3] = data[f+2];
				int *ii = (int*) &ccc[0];
				audio[f_out++] = *ii / (255.0f*255.0f*255.0f*255.0f);
			}
			out_smpcnt = f_out;
		}	
		else if (GetBitsPerSample() == 32)
		{
			printf("stereo, bits per sample: 32\n");
			
			if (isFloatFmtUsed() == false)
			{				
			  int f_out = 0;
			  for (int f = 0; f < smpcnt; f+=4*2)
			  {
				int *si;
				si = (int*) &data[f];
				audio[f_out++] = *si / (255.0f*255.0f*255.0f*255.0f);
			  }
			  out_smpcnt = f_out;
			}
			else
			{
				printf("float format used...\n");
				int f_out = 0;
				for (int f = 0; f < smpcnt; f+=4*2)
				{
					float *si;
					si = (float*) &data[f];
					audio[f_out++] = *si;
				}
				out_smpcnt = f_out;				
			}
		}		
	}
	else
	{
		printf("Only 1 and 2 channels supported for .wav files\n");
	}
	
	return out_smpcnt;
}
