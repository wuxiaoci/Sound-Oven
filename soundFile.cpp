/*	Gamma - Generic processing library
	See COPYRIGHT file for authors and license information
	
	Example:		Audio / IO
	Description:	Writing and reading a sound file.
*/

#include "Gamma/Gamma.h"

#include "Gamma/SoundFile.h"

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
		for(int i=0; i<numFramesTotal; i++){
	
			
			bufout[i*2  ] = buf[i];		// store left channel
			bufout[i*2+1] = bufIR[i];		// store right channel
		}
		
		
		// write entire buffer contents to file
		printf("Write samples to file... ");
		int n = sf.write(buf, numFrames);
		if(n == numFrames){	printf("OK\n"); }
		else{printf("fail\n"); exit(-1); }
		int n = sfir.write(buf, numFrames);
		if(n == numFrames){	printf("OK\n"); }
		else{printf("fail\n"); exit(-1); }
		
        sf.close();
	    sfir.close();
		
	}	

	//test
/////////////////////////////////////////////////////////////////////////////////////

	
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
