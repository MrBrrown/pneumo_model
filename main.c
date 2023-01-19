#include <stdio.h>
#include <stdlib.h>
#include "automate.h"

int main(int argc, char **argv){
    struct PneumoEngine engine = {};
    pneumo_engine_init(&engine);

    bool running = true;

    while (running){
        for (int i = 0; i < 8; i++){
            fprintf(stdout,"Y%d_UP | Y%d_DOWN: ",i+1,i+1);
            scanf("%d %d",
                  ( int *) &engine.cylinders[i].input_signal[PNEUMO_CYLINDER_SIGNAL_UP],
                  ( int *) &engine.cylinders[i].input_signal[PNEUMO_CYLINDER_SIGNAL_DOWN]);
        }
        running = pneumo_engine_tick(&engine);
        if (!running)
            fprintf(stderr, "Остановка из-за критической ошибки автомата\n");
        break;
    }
    pneumo_engine_destroy(&engine);
    return EXIT_SUCCESS;

}
