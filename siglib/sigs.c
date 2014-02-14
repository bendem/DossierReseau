#include "sigs.h"

void armerSignal(int sig, void (*handler)(int)) {
    struct sigaction sigAction;
    sigAction.sa_handler = handler;
    sigemptyset(&sigAction.sa_mask);
    sigAction.sa_flags = 0;

    if(sigaction(sig, &sigAction, NULL)) {
        perror("Erreur d'armement!");
        exit(1);
    }
}

void masquerSignal(int sig) {
    sigset_t x;
    sigemptyset(&x);
    sigaddset(&x, sig);

    if(sigprocmask(SIG_BLOCK, &x, NULL)) {
        perror("Erreur de masquage!");
        exit(1);
    }
}

void demasquerSignal(int sig) {
    sigset_t x;
    sigemptyset(&x);
    sigaddset(&x, sig);

    if(sigprocmask(SIG_UNBLOCK, &x, NULL)) {
        perror("Erreur de masquage!");
        exit(1);
    }
}
