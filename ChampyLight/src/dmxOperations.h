#ifndef DMXOPERATIONS_H
#define DMXOPERATIONS_H

// Enum of all transitions types
enum TYPE {            // TYPE               | Modelisation [0..100]
                       // ===================+===================== 
    
    // Regular types - defines transition for parameters: 
    //  # start   : initial value
    //  # end     : end value
    //  # fade_ti : time of transition
    //  # f(t)    : coef in function of time
    //
    //  At time t (t is in [0..fade_ti]), current value is given by
    //  
    //   F(t) = start + (end - start) * f(t / fade_ti)
    //
    LINEAR      = 0,   // Linear transition  | f(x) = x
    CRES        = 1,   // Crescendo          | f(x) = x^2.85
    DRES        = 2,   // Decrescendo        | f(x) = x^0.35
    CRES_DECRES = 3,   // Cres - Decres      | pending...
    
    // Special effects
    STROBE      = 4,   // Strobe effect      | fade_ti is the period
};

// Represents a decoded dmx input
typedef struct dmx_exec {
    enum TYPE type;        // Type of transition
    uint8_t   fade_ti;     // Transition time interval (seconds)
    uint8_t*  values;      // Pointer to the dmx channels
} dmx_exec_t;

// PROTOTYPES

// Decoding
dmx_exec_t* decodeDMX(uint8_t values[]);

// Execution transition
void executeDMX(dmx_exec_t *in, uint8_t startValues[]);

void executeRegular(dmx_exec_t *in, 
                    uint8_t startValues[],
                    double (*coef_func)(const double));
void executeStrobe(dmx_exec_t *in, uint8_t startValues[]);

// Helpers
void startTime(struct timeval *t0);
int updateTime(struct timeval *t0, struct timeval *t1, 
                                         unsigned int sleepTime);
#endif
