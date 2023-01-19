#ifndef PNEUMO_MODEL_AUTOMATE_H
#define PNEUMO_MODEL_AUTOMATE_H
#include <stdbool.h>


#if defined(__cplusplus)
extern "C"{
#endif

enum PneumoState{
    PneumoState_Init = 0,
    PneumoState_0, PneumoState_1, PneumoState_2,
    PneumoState_3, PneumoState_4, PneumoState_5,
    PneumoState_6, PneumoState_7, PneumoState_8,
    PneumoState_9, PneumoState_10, PneumoState_11,
    PneumoState_12, PneumoState_13, PneumoState_14,
    PneumoState_15, PneumoState_16, PneumoState_17,
    PneumoState_FatalException
};

#define PNEUMO_CYLINDER_SIGNAL_UP     0
#define PNEUMO_CYLINDER_SIGNAL_DOWN   1

struct PneumoCylinder{
    int input_signal[2];
    int output_signal;
};
#define PNEUMO_CYLINDER_Y1  0
#define PNEUMO_CYLINDER_Y2  1
#define PNEUMO_CYLINDER_Y3  2
#define PNEUMO_CYLINDER_Y4  3
#define PNEUMO_CYLINDER_Y5  4
#define PNEUMO_CYLINDER_Y6  5
#define PNEUMO_CYLINDER_Y7  6
#define PNEUMO_CYLINDER_Y8  7

struct PneumoEngine{
    enum PneumoState state;
    int timeout;
    int timeouts[PneumoState_FatalException];
    int delay;
    int delays[PneumoState_FatalException];
    struct PneumoCylinder cylinders[8];
    bool is_error;
};

void set_state(struct PneumoEngine *engine, int state_to_set);

void set_init_state(struct PneumoEngine *engine);

void set_timouts(struct PneumoEngine *engine, int values[]);

void set_delays(struct PneumoEngine *engine, int values[]);

void pneumo_engine_tick_switch(struct PneumoEngine *engine, int count, const int pneumo_cylinders[],
                               const int out_signals[], const int in_signals[], int if_true_state, int if_false_state);

void pneumo_engine_init(struct PneumoEngine *engine);

bool pneumo_engine_tick(struct PneumoEngine *engine);

void pneumo_engine_destroy(struct PneumoEngine *engine);

#if defined(__cplusplus)
}
#endif

#endif //PNEUMO_MODEL_AUTOMATE_H
