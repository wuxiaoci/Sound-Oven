/*	Gamma - Generic processing library
	See COPYRIGHT file for authors and license information
	
	Example:		Audio / IO
	Description:	Writing and reading a sound file.
*/

#include "Gamma/Gamma.h"

#include "Gamma/SoundFile.h"

#include <Gamma/FFT.h>
#include <cmath>


using namespace gam;

int main(int argc, char* argv[]){

    if (argc < 4) {
    	printf("Usage: %s input1 input2 output\n", argv[0]);
    	return -1;
    }
	const char * path = argv[1];
	const char * pathIR = argv[2];
	const char * pathOUT = argv[3];
	SoundFile sf(path);
	SoundFile sfir(pathIR);
	SoundFile sfout(pathOUT);

	printf("Open file for reading... ");
	if(sf.openRead()){	printf("OK\n"); }
	else{				printf("fail\n"); exit(-1); }
	if(sfir.openRead()){	printf("OK\n"); }
	else{				printf("fail\n"); exit(-1); }
	int numFrames = sf.frames();
		float buf[sf.samples()];
	int numFramesIR = sfir.frames();
		float bufIR[sfir.samples()];

	printf("Read samples from file... ");
		if(sf.readAll(buf) == numFrames){	printf("OK\n"); }
		else{								printf("fail\n"); exit(-1); }

		sf.close();
	printf("Read samples from file... ");
		if(sfir.readAll(bufIR) == numFramesIR){	printf("OK\n"); }
		else{	
									printf("fail\n"); exit(-1); }
	
/////////////////////////////////////////////////////////////////////////
	const float sampleRate = sf.frameRate();
		
		const int numFramesTotal = numFrames +numFramesIR-1;
    	float bufout[numFramesTotal];
    	
	printf("Open new file for writing... ");
		if(sfout.openWrite()){	printf("OK\n"); }
		else{				printf("fail\n"); exit(-1); }
        sfout.format(SoundFile::AIFF);
		sfout.encoding(SoundFile::PCM_16);
		sfout.channels(2);
		sfout.frameRate(sampleRate);

//  use numFramesTotal to find the next power of two   int NextPowerof2 = numFramesTotal (do some math) size of the FFT
		//step 2: create two new buffers at the twice of the size of the NextPowerof2 to put fft of each sound input
		//step3: fill all with zeros :bufout[i*2  ] = 0 ;		// store left channel
		//step4: write the original buffers into the new buffers, not all
			
int expon = ceil(log2(numFramesTotal));
long int  size = pow(2, expon);
long int twice = 2*size;
float buffft1[twice];
float buffft2[twice];

for(int i=0; i<twice; i++){
	
			
			buffft1[i] = 0;		
			buffft2[i] = 0;		
		}

 for (int i =0; i<numFrames; i++){
 	buffft1[2*i] = buf[i];
 }
 for (int i =0; i<numFramesIR; i++){
 	buffft2[2*i] = bufIR[i];
 }

 CFFT<float> ft(size);
 ft.forward(buffft1);
 ft.forward(buffft2);
 float real_a, imag_a, real_b, imag_b;

//convolution
for (int i=0; i<size; i++){

	real_a = buffft1[2*i];
	imag_a = buffft1[2*i+1];

	real_b = buffft2[2*i];
	imag_b = buffft2[2*i+1];

	buffft1[2*i] = (real_a*real_b-imag_a*imag_b);
	buffft1[2*i+1] = (real_a*imag_b+imag_a*real_b); 
}

ft.inverse(buffft1);

		
		
		// write entire buffer contents to file
		printf("Write samples to file... ");
		sfout.openWrite();
		int n = sfout.write(buffft1, numFramesTotal);
		if(n == numFramesTotal){	printf("OK\n"); }
		else{printf("fail\n"); exit(-1); }
		sfout.close();
        sf.close();
	    sfir.close();


	return 0;
		
	}	

/*/////////////////////////////////////////////////////////////////////////////////////

	
	// Write a beating sine wave to file
	{
		const float sampleRate = 44100;
		const float lenSec = 2;
		const int numFrames = sampleRate * lenSec;
		
		float freq = 440;
		float buf[numFrames*2];		// Buffer for storing samples.
									// The samples are stored interleaved as 
									// sequential stereo frames.
		sf.format(SoundFile::AIFF);
		sf.encoding(SoundFile::PCM_16);
		sf.channels(2);
		sf.frameRate(sampleRate);
		
		printf("Open new file for writing... ");
		if(sf.openWrite()){	printf("OK\n"); }
		else{				printf("fail\n"); exit(-1); }

		for(int i=0; i<numFrames; i++){
			
			float s = i/float(numFrames) * lenSec;	// compute time in seconds
			float p1 = s * freq * M_2PI;
			float p2 = s * (freq+4) * M_2PI;
			
			buf[i*2  ] = sin(p1)*0.5;		// store left channel
			buf[i*2+1] = sin(p2)*0.5;		// store right channel
		}
		
		
		// write entire buffer contents to file
		printf("Write samples to file... ");
		int n = sf.write(buf, numFrames);
		if(n == numFrames){	printf("OK\n"); }
		else{										printf("fail\n"); exit(-1); }
		
		sf.close();
	}


	// Read samples from the file we just wrote...
	{
		printf("Open file for reading... ");
		if(sf.openRead()){	printf("OK\n"); }
		else{				printf("fail\n"); exit(-1); }
		if(sfir.openRead()){	printf("OK\n"); }
		else{				printf("fail\n"); exit(-1); }
	
		//sf.print();
		
		int numFrames = sf.frames();
		float buf[sf.samples()];
		
		//sf.readAll(buf);		// read all samples from sound file into buffer

		printf("Read samples from file... ");
		if(sf.readAll(buf) == numFrames){	printf("OK\n"); }
		else{								printf("fail\n"); exit(-1); }

		sf.close();
	

		// Rewrite sound file to make sure we read samples correctly
		printf("Open existing file for rewriting... ");
		if(sf.openWrite()){	printf("OK\n"); }
		else{				printf("fail\n"); exit(-1); }

		printf("Rewrite samples to file... ");
		if(sf.write(buf, numFrames) == numFrames){	printf("OK\n"); }
		else{										printf("fail\n"); exit(-1); }

		sf.close();
	}

	return 0;
}
*/