#ifndef SIGS_h
#define SIGS_h

#include <stdlib.h>
#include <signal.h>

void armerSignal(int, void (*)(int));
void masquerSignal(int);
void demasquerSignal(int);

#endif
