/* Marta Szukalska
 * assignment1_crossover
 * RTDSP 2016
 *
 * First assignment for ECS732 RTDSP, to implement a 2-way audio crossover
 * using the BeagleBone Black.
 *
 * Andrew McPherson and Victor Zappi
 * Modified by Becky Stewart
 * Queen Mary, University of London
 
 
 */

#include <BeagleRT.h>
#include <cmath>
#include <Utilities.h>
#include <rtdk.h>

/* TASK: declare any global variables you need here */

// setup() is called once before the audio rendering starts.
// Use it to perform any initialisation and allocation which is dependent
// on the period size or sample rate.
//
// userData holds an opaque pointer to a data structure that was passed
// in from the call to initAudio().
//
// Return true on success; returning false halts the program.

float gPi = 3.14;
float gSamplingFrequency = 44100; //Sampling Frequency
float gLastSample =0; // Store a1x(n-1) for LPF
float gOneBeforeLast = 0; // Store a2x(n-2) for LPF

float gLastOutput = 0; //store b1y(n-1) for LPF
float gBeforeLastOutput = 0; //store b2y(n-2) for LPF


float gLastSample1 =0; // Store a1x(n-1) for HPF
float gOneBeforeLast1 = 0; // Store a2x(n-2) for HPF

float gLastOutput1 = 0; //store b1y(n-1) for HPF
float gBeforeLastOutput1 = 0; //store b2y(n-2) for HPF

//Declare coofficients of a differential equation. 
float C;
float C1;
float a;
float a1;
float a2;

float d;
float d1;
float d2;

float b1;
float b2;

float e1;
float e2;

bool setup(BeagleRTContext *context, void *userData)
{
	float crossoverFrequency;
	// Retrieve a parameter passed in from the initAudio() call
	if(userData != 0)
		crossoverFrequency = *(float *)userData;

	/* TASK:
	 * Calculate the filter coefficients based on the given
	 * crossover frequency.
	 *
	 * Initialise any previous state (clearing buffers etc.)
	 * to prepare for calls to render()
	 */
	 
	 
	 //Calculation of cooficients of LPF(Low Pass Filter) - a0, a1, a2, b1, b2.
	 
	 C = 1/(tan(gPi*crossoverFrequency/gSamplingFrequency));
	 
	 a = 1/(1+ (sqrt(2)*C)+(C*C)); 
	
	 a1 = 2*a;
	
	 a2 = a;

	 b1 = 2*a*(1-(C*C));
	 b2 = a*(1 - (sqrt(2)*C) + (C*C));
	 
	 //Calculation of cooficients of HPF() - d0,d1,d2,e1,e2.
	 
	 C1 = tan(gPi*crossoverFrequency/gSamplingFrequency);
	 d = 1/(1+ (sqrt(2)*C1)+(C1*C1));
	 d1 = -2*d;
	 d2 = d;
	 e1 = 2*d*((C1*C1)-1);
	 e2 = d*(1 - (sqrt(2)*C1) + (C1*C1));
	 

	return true;
}

// render() is called regularly at the highest priority by the audio engine.
// Input and output are given from the audio hardware and the other
// ADCs and DACs (if available). If only audio is available, numMatrixFrames
// will be 0.

void render(BeagleRTContext *context, void *userData)
{
	/* TASK:
	 * Mix the two input channels together.
	 *
	 * Apply a lowpass filter and a highpass filter, sending the
	 * lowpass output to the left channel and the highpass output
	 * to the right channel.
	 */
	 //Difference Equation:
	 //y(n) = a0x(n) + a1x(n-1) + a2x(n-2) - b1y(n-1) - b2y(n-2)
	 //MIX 
for(unsigned int n =0; n< context->audioFrames;n++){	

	 // The sampples from the left and the right channels are mixed and stored in monoSignal variable
	 float monoSignal =( 0.5*(context ->audioIn[n*context->audioChannels+0])    +  0.5* (context ->audioIn[n*context->audioChannels+1]) )/2;
	 
	 //LPF -the differential equation is applied to the signal.
	 float LeftChannel = a*monoSignal + a1*gLastSample + a2*gOneBeforeLast - b1*gLastOutput - b2*gBeforeLastOutput;
	 
	 gOneBeforeLast = gLastSample; //The sample before the last one  (x[n-1] is saved.
	 gLastSample = monoSignal;     //Save the current sample x[n]
	 gBeforeLastOutput = gLastOutput;//save y[n-1]
	 gLastOutput = LeftChannel; //save y[n]
	 
	 context->audioOut[n*context->audioChannels +0] = LeftChannel;//output the audio signal to the left channel(low frequencies)
	 
	 //HPF-the differential equation is applied to the signal.
	 
	 float RightChannel = d*monoSignal + d1*gLastSample1 + d2*gOneBeforeLast1 - e1*gLastOutput1 - e2*gBeforeLastOutput1;
	 
	 gOneBeforeLast1 = gLastSample1;//The sample before the last one  (x[n-1] is saved.
	 gLastSample1 = monoSignal;     //Save the current sample x[n]
	 gBeforeLastOutput1 = gLastOutput1; //save y[n-1]
	 gLastOutput1 = RightChannel;//save y[n]
	 
	 context->audioOut[n*context->audioChannels +1] = RightChannel; //output the audio signal to the right channel(high frequencies)
	 
	 
}	 
}

// cleanup_render() is called once at the end, after the audio has stopped.
// Release any resources that were allocated in initialise_render().

void cleanup(BeagleRTContext *context, void *userData)
{
	/* TASK:
	 * If you allocate any memory, be sure to release it here.
	 * You may or may not need anything in this function, depending
	 * on your implementation.
	 */
}
