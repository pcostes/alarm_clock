#include "helpers.h"

void Helpers::blink_reps(int ms, int reps)
{
    bool state = true;
    for (int i = 0; i < reps * 2; i++)
    {
        digitalWrite(LED_BUILTIN, state);
        delay(ms);
        state = !state;
    }
}
