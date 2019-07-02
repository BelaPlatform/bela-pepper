#include <Bela.h>
#include <CapTouch.h>

CapTouch capTouchBreakout;
CapTouch capTouch2D;
CapTouch capTouch1D_1;
CapTouch capTouch1D_2;
CapTouch capTouch1D_3;

// Arrays for Raw Readings
int gBreakoutRaw[7];
int g2DRawXAxis[15];
int g2DRawYAxis[15];
int g1D_1Raw[26];
int g1D_2Raw[26];
int g1D_3Raw[26];

// Calculate the centroid for 1D slider 1
float g1D_1PositionWeighted = 0;
float g1D_1PositionTotal = 0;
float g1D_1Positions = 0;

// Calculate the centroid for 1D slider 2
float g1D_2PositionWeighted = 0;
float g1D_2PositionTotal = 0;
float g1D_2Positions = 0;

// Calculate the centroid for 1D slider 3
float g1D_3PositionWeighted = 0;
float g1D_3PositionTotal = 0;
float g1D_3Positions = 0;

// Calculate x/y position of the 2D slider
float g2D_PositionWeighted[2];
float g2D_PositionTotal[2];
float g2D_Positions[2];

// Sleep interval for reading the sensors
unsigned int capTouchSleepIntervalUs = 50000;

// Print sensor values
int gPrintCounter = 0;
float gPrintInterval = 0.5; // in seconds

float clip( float n, float lower, float upper )
{
    n = ( n > lower ) * n + !( n > lower ) * lower;
    return ( n < upper ) * n + !( n < upper ) * upper;
}

void readCapSensorLoop(void*)
{
    while(!gShouldStop)
    {
        // Read from Breakout (1st 7 channels)
        if (capTouchBreakout.ready()) {
            capTouchBreakout.readI2C();
            // printf("capTouchBreakout: ");
            for(unsigned int i = 0; i < 7; i++){
                gBreakoutRaw[i] = capTouchBreakout.rawData[i];
                // printf("%5d ", capTouchBreakout.rawData[i]);
            }
            // printf("\n");
        }
        
        // Read from the 2D sensor
        if (capTouch2D.ready()) {
            capTouch2D.readI2C();
            // printf("capTouch2D: ");
            for(unsigned int i = 0; i < 15; i++){
                g2DRawXAxis[i] = clip(capTouch2D.rawData[i+15],0,1000);
                g2DRawYAxis[i] = clip(capTouch2D.rawData[i],0,1000);
                // printf("%5d ", capTouch2D.rawData[i]);
            }
            // printf("\n");
        }

        // Read from the 1D_1 sensor (TOP)
        if (capTouch1D_1.ready()) {
            capTouch1D_1.readI2C();
            // printf("capTouch1D_1: ");
            for(unsigned int i = 0; i < 26; i++){
                g1D_1Raw[i] = clip(capTouch1D_1.rawData[i],0,1000);
                // printf("%5d ", capTouch1D_1.rawData[i]);
            }
            // printf("\n");
        }
        
        // Read from the 1D_2 sensor (MIDDLE)
        if (capTouch1D_2.ready()) {
            capTouch1D_2.readI2C();
            // printf("capTouch1D_2: ");
            for(unsigned int i = 0; i < 26; i++){
                g1D_2Raw[i] = clip(capTouch1D_2.rawData[i],0,1000);
                // printf("%5d ", capTouch1D_2.rawData[i]);
            }
            // printf("\n");
        }
        
        // Read from the 1D_3 sensor (BOTTOM)
        if (capTouch1D_3.ready()) {
            capTouch1D_3.readI2C();
            // printf("capTouch1D_3: ");
            for(unsigned int i = 0; i < 26; i++){
                g1D_3Raw[i] = clip(capTouch1D_3.rawData[i],0,1000);
                // printf("%5d ", capTouch1D_3.rawData[i]);
            }
            // printf("\n");
        }

        usleep(capTouchSleepIntervalUs);
    }
}

bool setup(BelaContext *context, void *userData)
{
    // Addresses for each sensor
    capTouchBreakout.setup(1,0x18);
    capTouch1D_1.setup(1,0x1F);
    capTouch1D_2.setup(1,0x19);
    capTouch1D_3.setup(1,0x1A);
    capTouch2D.setup(1,0x1B);
    Bela_scheduleAuxiliaryTask(Bela_createAuxiliaryTask(readCapSensorLoop, 50, "capTouchRead", NULL));
    return true;
}

void render(BelaContext *context, void *userData)
{
    
    // Calculate the centroid of the 1D sensors
    //-----------------------------------------

    g1D_1PositionWeighted = 0;
    g1D_1PositionTotal = 0;
    g1D_2PositionWeighted = 0;
    g1D_2PositionTotal = 0;
    g1D_3PositionWeighted = 0;
    g1D_3PositionTotal = 0;

    for(unsigned int j = 0; j < 26; j++) {
        g1D_1PositionWeighted += g1D_1Raw[j]*(j+1);
        g1D_1PositionTotal += g1D_1Raw[j];
        
        g1D_2PositionWeighted += g1D_2Raw[j]*(j+1);
        g1D_2PositionTotal += g1D_2Raw[j];
        
        g1D_3PositionWeighted += g1D_3Raw[j]*(j+1);
        g1D_3PositionTotal += g1D_3Raw[j];
    }
    
    if(g1D_1PositionTotal > 0){
        g1D_1Positions = g1D_1PositionWeighted/g1D_1PositionTotal;
    }
    
    if(g1D_2PositionTotal > 0){
        g1D_2Positions = g1D_2PositionWeighted/g1D_2PositionTotal;
    }
    
    if(g1D_3PositionTotal > 0){
        g1D_3Positions = g1D_3PositionWeighted/g1D_3PositionTotal;
    }
    //-----------------------------------------

    // Calculate the x + y position of the 2D sensor
    //-----------------------------------------
    g2D_PositionWeighted[0] = 0;
    g2D_PositionWeighted[1] = 0;
    g2D_PositionTotal[0] = 0;
    g2D_PositionTotal[1] = 0;

    for(unsigned int k = 0; k < 15; k++) {
        g2D_PositionWeighted[0] += g2DRawXAxis[k]*(k+1);
        g2D_PositionWeighted[1] += g2DRawYAxis[k]*(k+1);
        
        g2D_PositionTotal[0] += g2DRawXAxis[k];
        g2D_PositionTotal[1] += g2DRawYAxis[k];
    }

    if(g2D_PositionTotal[0] > 0 || g2D_PositionTotal[1] > 0){
        g2D_Positions[0] = g2D_PositionWeighted[0]/g2D_PositionTotal[0];
        g2D_Positions[1] = g2D_PositionWeighted[1]/g2D_PositionTotal[1];
    }
    //-----------------------------------------



    for(unsigned int n = 0; n < context->audioFrames; n++) {
        
        gPrintCounter++;

        // Print values of all capacitive sensors
        if(gPrintCounter > context->audioSampleRate*gPrintInterval){
            gPrintCounter = 0;
            for(int j = 0; j < 7; j++){
                rt_printf("Touch %d = %d   ", j, gBreakoutRaw[j]);
            }
            
            rt_printf("Slider 1: %f   ", g1D_1Positions);
            rt_printf("Slider 2: %f   ", g1D_2Positions);
            rt_printf("Slider 3: %f   ", g1D_3Positions);
            
            rt_printf("XAxis: %f   ", g2D_Positions[0]);
            rt_printf("YAxis: %f   ", g2D_Positions[1]);
            rt_printf("\n");
        }
    }
}

void cleanup(BelaContext *context, void *userData)
{

}