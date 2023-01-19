#include <stdio.h>
#include <limits.h>
#include "automate.h"

#if defined(PNEUMO_SIMULATE)
#define TIMOUT_DELTA(timeout) timeout
#define DELAY_DELTA(delay)    delay
#else
#define TIMOUT_DELTA(timeout) ((timeout) * 1000)
#define DELAY_DELTA(delay)    ((delay) * 1000)
#endif

#define TIMEOUT_GE(engine) ( (engine)->timeout > (engine)->timeouts[(engine)->state] )
#define DELAY_GE(engine) ( (engine)->delay > (engine)->delays[(engine)->state] )

void set_state(struct PneumoEngine *engine, int state_to_set){
    switch (state_to_set){
        case 0:{
            engine->state = PneumoState_0;
            break;
        }
        case 1:{
            engine->state = PneumoState_1;
            break;
        }
        case 2:{
            engine->state = PneumoState_2;
            break;
        }case 3:{
            engine->state = PneumoState_3;
            break;
        }
        case 4:{
            engine->state = PneumoState_4;
            break;
        }
        case 5:{
            engine->state = PneumoState_5;
            break;
        }
        case 6:{
            engine->state = PneumoState_6;
            break;
        }
        case 7:{
            engine->state = PneumoState_7;
            break;
        }
        case 8:{
            engine->state = PneumoState_8;
            break;
        }
        case 9:{
            engine->state = PneumoState_9;
            break;
        }
        case 10:{
            engine->state = PneumoState_10;
            break;
        }
        case 11:{
            engine->state = PneumoState_11;
            break;
        }
        case 12:{
            engine->state = PneumoState_12;
            break;
        }
        case 13:{
            engine->state = PneumoState_13;
            break;
        }
        case 14:{
            engine->state = PneumoState_14;
            break;
        }
        case 15:{
            engine->state = PneumoState_15;
            break;
        }
        case 16:{
            engine->state = PneumoState_16;
            break;
        }
        case 17:{
            engine->state = PneumoState_17;
            break;
        }
        case -1:{
            engine->state = PneumoState_FatalException;
            break;
        }
        default:
            break;
    }
}

void set_init_state(struct PneumoEngine *engine){
    engine->state = PneumoState_Init;
    for (int i = PNEUMO_CYLINDER_Y1; i <= PNEUMO_CYLINDER_Y8; i++){
        engine->cylinders[i].input_signal[PNEUMO_CYLINDER_SIGNAL_DOWN] = 0;
        engine->cylinders[i].input_signal[PNEUMO_CYLINDER_SIGNAL_UP] = 0;
        engine->cylinders[i].output_signal = 0;
    }
}

void set_timouts(struct PneumoEngine *engine, int values[]){
    for (int i = PneumoState_Init; i <= PneumoState_17; i++)
        engine->timeouts[i] = TIMOUT_DELTA(values[i]);
}

void set_delays(struct PneumoEngine *engine, int values[]){
    for (int i = PneumoState_Init; i <= PneumoState_17; i++)
        engine->delays[i] = DELAY_DELTA(values[i]);
}

void pneumo_engine_tick_switch(struct PneumoEngine *engine, int count, const int pneumo_cylinders[],
                               const int out_signals[], const int in_signals[], int if_true_state, int if_false_state){
    for (int i = 0; i < count; i++)
        engine->cylinders[pneumo_cylinders[i]].output_signal = out_signals[i];

    bool flag = true;
    for (int i = 0; i < count; i++){
        if (!engine->cylinders[pneumo_cylinders[i]].input_signal[in_signals[i]])
            flag = false;
    }
    if (flag && DELAY_GE(engine)){
        set_state(engine, if_true_state);
        engine->delay = 0;
        engine->timeout = 0;
    } else if (flag) {
        engine->delay++;
    } else if (TIMEOUT_GE(engine) || engine->is_error){
        set_state(engine, if_false_state);
        for (int i = PNEUMO_CYLINDER_Y1; i <= PNEUMO_CYLINDER_Y8; i++)
            engine->cylinders[i].output_signal = 0;
        engine->delay = 0;
        engine->timeout = 0;
    }
}

void pneumo_engine_init(struct PneumoEngine *engine){
    if (0 != engine){
        set_init_state(engine);
        int timeout_values[] = {INT_MAX, 56, 56, 45, 30, 45, 60,
                                120, 60, 56, 120, 45, 45,
                                120, 120, 56, 120, 56, 56};
        int delays_values[] = {INT_MAX, 60, 60, 70, 45, 45, 33,
                               60, 33, 70, 45, 78, 33,
                               70, 33, 70, 78, 45, 45};
        set_timouts(engine, timeout_values);
        set_delays(engine,delays_values);
    }
}

bool pneumo_engine_tick(struct PneumoEngine *engine){
    bool ret = true;
    if (0 == engine)
        return false;

    switch (engine->state){
        case PneumoState_Init: {
            engine->state = PneumoState_0;
            engine->timeout = 0;
            engine->delay = 0;
            break;
        }
        case PneumoState_0: {
            int pneumo_cylinders[] = {PNEUMO_CYLINDER_Y1, PNEUMO_CYLINDER_Y2, PNEUMO_CYLINDER_Y3, PNEUMO_CYLINDER_Y4,
                                    PNEUMO_CYLINDER_Y5, PNEUMO_CYLINDER_Y6, PNEUMO_CYLINDER_Y7, PNEUMO_CYLINDER_Y8};
            int out_signals[] = {0, 0, 0, 0, 0, 0, 0, 0};
            int in_signals[] = {PNEUMO_CYLINDER_SIGNAL_DOWN, PNEUMO_CYLINDER_SIGNAL_DOWN, PNEUMO_CYLINDER_SIGNAL_DOWN,
                                PNEUMO_CYLINDER_SIGNAL_DOWN, PNEUMO_CYLINDER_SIGNAL_DOWN, PNEUMO_CYLINDER_SIGNAL_DOWN,
                                PNEUMO_CYLINDER_SIGNAL_DOWN, PNEUMO_CYLINDER_SIGNAL_DOWN};
            pneumo_engine_tick_switch(engine, 8, pneumo_cylinders, out_signals,
                                      in_signals, 1, -1); //-1 = переход в состояние ощибки
            break;
        }
        case PneumoState_1: {
            int pneumo_cylinders[] = {PNEUMO_CYLINDER_Y8, PNEUMO_CYLINDER_Y1, PNEUMO_CYLINDER_Y2,
                                      PNEUMO_CYLINDER_Y3, PNEUMO_CYLINDER_Y7};
            int out_signals[] = {1, 1, 1, 1, 1};
            int in_signals[] = {PNEUMO_CYLINDER_SIGNAL_UP, PNEUMO_CYLINDER_SIGNAL_UP, PNEUMO_CYLINDER_SIGNAL_UP,
                                PNEUMO_CYLINDER_SIGNAL_UP, PNEUMO_CYLINDER_SIGNAL_UP};
            pneumo_engine_tick_switch(engine, 5, pneumo_cylinders, out_signals,
                                      in_signals, 2, 11);
            break;
        }
        case PneumoState_2: {
            int pneumo_cylinders[] = {PNEUMO_CYLINDER_Y8, PNEUMO_CYLINDER_Y1, PNEUMO_CYLINDER_Y2,
                                      PNEUMO_CYLINDER_Y3, PNEUMO_CYLINDER_Y4};
            int out_signals[] = {0, 0, 0, 0, 1};
            int in_signals[] = {PNEUMO_CYLINDER_SIGNAL_DOWN, PNEUMO_CYLINDER_SIGNAL_DOWN, PNEUMO_CYLINDER_SIGNAL_DOWN,
                                PNEUMO_CYLINDER_SIGNAL_DOWN, PNEUMO_CYLINDER_SIGNAL_UP};
            pneumo_engine_tick_switch(engine, 5, pneumo_cylinders, out_signals,
                                      in_signals, 3, -1);
            break;
        }
        case PneumoState_3: {
            int pneumo_cylinders[] = {PNEUMO_CYLINDER_Y3, PNEUMO_CYLINDER_Y4,
                                      PNEUMO_CYLINDER_Y6, PNEUMO_CYLINDER_Y7};
            int out_signals[] = {1, 0, 1, 0};
            int in_signals[] = {PNEUMO_CYLINDER_SIGNAL_UP, PNEUMO_CYLINDER_SIGNAL_DOWN,
                                PNEUMO_CYLINDER_SIGNAL_UP, PNEUMO_CYLINDER_SIGNAL_DOWN};
            pneumo_engine_tick_switch(engine, 4, pneumo_cylinders, out_signals,
                                      in_signals, 4, -1);
            break;
        }
        case PneumoState_4: {
            int pneumo_cylinders[] = {PNEUMO_CYLINDER_Y1, PNEUMO_CYLINDER_Y2, PNEUMO_CYLINDER_Y3, PNEUMO_CYLINDER_Y4,
                                      PNEUMO_CYLINDER_Y5, PNEUMO_CYLINDER_Y6, PNEUMO_CYLINDER_Y7, PNEUMO_CYLINDER_Y8};
            int out_signals[] = {1, 1, 0, 1, 1, 0, 1, 1};
            int in_signals[] = {PNEUMO_CYLINDER_SIGNAL_UP, PNEUMO_CYLINDER_SIGNAL_UP, PNEUMO_CYLINDER_SIGNAL_DOWN,
                                PNEUMO_CYLINDER_SIGNAL_UP, PNEUMO_CYLINDER_SIGNAL_UP, PNEUMO_CYLINDER_SIGNAL_DOWN,
                                PNEUMO_CYLINDER_SIGNAL_UP, PNEUMO_CYLINDER_SIGNAL_UP};
            pneumo_engine_tick_switch(engine, 8, pneumo_cylinders, out_signals,
                                      in_signals, 5, -1);
            break;
        }
        case PneumoState_5: {
            int pneumo_cylinders[] = {PNEUMO_CYLINDER_Y2, PNEUMO_CYLINDER_Y3,
                                      PNEUMO_CYLINDER_Y7};
            int out_signals[] = {0, 1, 0};
            int in_signals[] = {PNEUMO_CYLINDER_SIGNAL_DOWN, PNEUMO_CYLINDER_SIGNAL_UP,
                                PNEUMO_CYLINDER_SIGNAL_DOWN};
            pneumo_engine_tick_switch(engine, 3, pneumo_cylinders, out_signals,
                                      in_signals, 6, -1);
            break;
        }
        case PneumoState_6: {
            int pneumo_cylinders[] = {PNEUMO_CYLINDER_Y2, PNEUMO_CYLINDER_Y4,
                                      PNEUMO_CYLINDER_Y6, PNEUMO_CYLINDER_Y7};
            int out_signals[] = {1, 0, 1, 1};
            int in_signals[] = {PNEUMO_CYLINDER_SIGNAL_UP, PNEUMO_CYLINDER_SIGNAL_DOWN,
                                PNEUMO_CYLINDER_SIGNAL_UP, PNEUMO_CYLINDER_SIGNAL_UP};
            pneumo_engine_tick_switch(engine, 4, pneumo_cylinders, out_signals,
                                      in_signals, 7, 15);
            break;
        }
        case PneumoState_7: {
            int pneumo_cylinders[] = {PNEUMO_CYLINDER_Y8};
            int out_signals[] = {0};
            int in_signals[] = {PNEUMO_CYLINDER_SIGNAL_DOWN};
            pneumo_engine_tick_switch(engine, 1, pneumo_cylinders, out_signals,
                                      in_signals, 8, -1);
            break;
        }
        case PneumoState_8: {
            int pneumo_cylinders[] = {PNEUMO_CYLINDER_Y2, PNEUMO_CYLINDER_Y3, PNEUMO_CYLINDER_Y5,
                                      PNEUMO_CYLINDER_Y6, PNEUMO_CYLINDER_Y7, PNEUMO_CYLINDER_Y8};
            int out_signals[] = {0, 0, 0, 0, 0, 1};
            int in_signals[] = {PNEUMO_CYLINDER_SIGNAL_DOWN, PNEUMO_CYLINDER_SIGNAL_DOWN, PNEUMO_CYLINDER_SIGNAL_DOWN,
                                PNEUMO_CYLINDER_SIGNAL_DOWN, PNEUMO_CYLINDER_SIGNAL_DOWN, PNEUMO_CYLINDER_SIGNAL_UP};
            pneumo_engine_tick_switch(engine, 6, pneumo_cylinders, out_signals,
                                      in_signals, 9, -1);
            break;
        }
        case PneumoState_9: {
            int pneumo_cylinders[] = {PNEUMO_CYLINDER_Y8, PNEUMO_CYLINDER_Y3};
            int out_signals[] = {0, 1};
            int in_signals[] = {PNEUMO_CYLINDER_SIGNAL_DOWN, PNEUMO_CYLINDER_SIGNAL_UP};
            pneumo_engine_tick_switch(engine, 2, pneumo_cylinders, out_signals,
                                      in_signals, 10, -1);
            break;
        }
        case PneumoState_10: {
            int pneumo_cylinders[] = {PNEUMO_CYLINDER_Y2, PNEUMO_CYLINDER_Y3, PNEUMO_CYLINDER_Y5,
                                      PNEUMO_CYLINDER_Y6, PNEUMO_CYLINDER_Y7, PNEUMO_CYLINDER_Y8};
            int out_signals[] = {1, 0, 1, 1, 1, 1};
            int in_signals[] = {PNEUMO_CYLINDER_SIGNAL_UP, PNEUMO_CYLINDER_SIGNAL_DOWN, PNEUMO_CYLINDER_SIGNAL_UP,
                                PNEUMO_CYLINDER_SIGNAL_UP, PNEUMO_CYLINDER_SIGNAL_UP, PNEUMO_CYLINDER_SIGNAL_UP};
            pneumo_engine_tick_switch(engine, 6, pneumo_cylinders, out_signals,
                                      in_signals, 11, -1);
            break;
        }
        case PneumoState_11: {
            int pneumo_cylinders[] = {PNEUMO_CYLINDER_Y2, PNEUMO_CYLINDER_Y4,
                                      PNEUMO_CYLINDER_Y5, PNEUMO_CYLINDER_Y6};
            int out_signals[] = {0, 1, 0, 0};
            int in_signals[] = {PNEUMO_CYLINDER_SIGNAL_DOWN, PNEUMO_CYLINDER_SIGNAL_UP,
                                PNEUMO_CYLINDER_SIGNAL_DOWN, PNEUMO_CYLINDER_SIGNAL_DOWN};
            pneumo_engine_tick_switch(engine, 4, pneumo_cylinders, out_signals,
                                      in_signals, 12, -1);
            break;
        }
        case PneumoState_12: {
            int pneumo_cylinders[] = {PNEUMO_CYLINDER_Y1, PNEUMO_CYLINDER_Y2, PNEUMO_CYLINDER_Y3,
                                      PNEUMO_CYLINDER_Y5, PNEUMO_CYLINDER_Y6, PNEUMO_CYLINDER_Y7};
            int out_signals[] = {0, 1, 1, 1, 1, 0};
            int in_signals[] = {PNEUMO_CYLINDER_SIGNAL_DOWN, PNEUMO_CYLINDER_SIGNAL_UP, PNEUMO_CYLINDER_SIGNAL_UP,
                                PNEUMO_CYLINDER_SIGNAL_UP, PNEUMO_CYLINDER_SIGNAL_UP, PNEUMO_CYLINDER_SIGNAL_DOWN};
            pneumo_engine_tick_switch(engine, 6, pneumo_cylinders, out_signals,
                                      in_signals, 13, -1);
            break;
        }
        case PneumoState_13: {
            int pneumo_cylinders[] = {PNEUMO_CYLINDER_Y8, PNEUMO_CYLINDER_Y2};
            int out_signals[] = {0, 0};
            int in_signals[] = {PNEUMO_CYLINDER_SIGNAL_DOWN, PNEUMO_CYLINDER_SIGNAL_DOWN};
            pneumo_engine_tick_switch(engine, 2, pneumo_cylinders, out_signals,
                                      in_signals, 14, -1);
            break;
        }
        case PneumoState_14: {
            int pneumo_cylinders[] = {PNEUMO_CYLINDER_Y1, PNEUMO_CYLINDER_Y2, PNEUMO_CYLINDER_Y3, PNEUMO_CYLINDER_Y4,
                                      PNEUMO_CYLINDER_Y5, PNEUMO_CYLINDER_Y6, PNEUMO_CYLINDER_Y7, PNEUMO_CYLINDER_Y8};
            int out_signals[] = {1, 1, 0, 0, 0, 0, 1, 1};
            int in_signals[] = {PNEUMO_CYLINDER_SIGNAL_UP, PNEUMO_CYLINDER_SIGNAL_UP, PNEUMO_CYLINDER_SIGNAL_DOWN,
                                PNEUMO_CYLINDER_SIGNAL_DOWN, PNEUMO_CYLINDER_SIGNAL_DOWN, PNEUMO_CYLINDER_SIGNAL_DOWN,
                                PNEUMO_CYLINDER_SIGNAL_UP, PNEUMO_CYLINDER_SIGNAL_UP};
            pneumo_engine_tick_switch(engine, 8, pneumo_cylinders, out_signals,
                                      in_signals, 15, -1);
            break;
        }
        case PneumoState_15: {
            int pneumo_cylinders[] = {PNEUMO_CYLINDER_Y1, PNEUMO_CYLINDER_Y2, PNEUMO_CYLINDER_Y3,
                                      PNEUMO_CYLINDER_Y6, PNEUMO_CYLINDER_Y7};
            int out_signals[] = {0, 0, 1, 1, 0};
            int in_signals[] = {PNEUMO_CYLINDER_SIGNAL_DOWN, PNEUMO_CYLINDER_SIGNAL_DOWN, PNEUMO_CYLINDER_SIGNAL_UP,
                                PNEUMO_CYLINDER_SIGNAL_UP, PNEUMO_CYLINDER_SIGNAL_DOWN};
            pneumo_engine_tick_switch(engine, 5, pneumo_cylinders, out_signals,
                                      in_signals, 16, -1);
            break;
        }
        case PneumoState_16: {
            int pneumo_cylinders[] = {PNEUMO_CYLINDER_Y5};
            int out_signals[] = {1};
            int in_signals[] = {PNEUMO_CYLINDER_SIGNAL_UP};
            pneumo_engine_tick_switch(engine, 1, pneumo_cylinders, out_signals,
                                      in_signals, 17, -1);
            break;
        }
        case PneumoState_17: {
            int pneumo_cylinders[] = {PNEUMO_CYLINDER_Y1, PNEUMO_CYLINDER_Y2, PNEUMO_CYLINDER_Y3, PNEUMO_CYLINDER_Y4,
                                      PNEUMO_CYLINDER_Y5, PNEUMO_CYLINDER_Y6, PNEUMO_CYLINDER_Y8};
            int out_signals[] = {1, 1, 0, 1, 0, 0, 0};
            int in_signals[] = {PNEUMO_CYLINDER_SIGNAL_UP, PNEUMO_CYLINDER_SIGNAL_UP, PNEUMO_CYLINDER_SIGNAL_DOWN,
                                PNEUMO_CYLINDER_SIGNAL_UP, PNEUMO_CYLINDER_SIGNAL_DOWN, PNEUMO_CYLINDER_SIGNAL_DOWN,
                                PNEUMO_CYLINDER_SIGNAL_DOWN};
            pneumo_engine_tick_switch(engine, 7, pneumo_cylinders, out_signals,
                                      in_signals, 0, -1);
            break;
        }
        case PneumoState_FatalException: {
            ret = false;
            break;
        }
    }
    engine->timeout++;
    return ret;
}

void pneumo_engine_destroy(struct PneumoEngine *engine){
    if (0 != engine){
        //Освобождене ресурсов
    }
}
