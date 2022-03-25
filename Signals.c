#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/mman.h>

void sigHandlerStartingChain(int sigNum);

void sigHandler2(int sigNum);

void sigHandler3(int sigNum);

void sigHandler4(int sigNum);

void sigHandler5(int sigNum);

void sigHandler6(int sigNum);

void sigHandler7(int sigNum);


void sigHandlerStartingChain(int sigNum) {
    write(STDOUT_FILENO, "\n#1 SIGNAL IN CHAIN - SIGINT: Interrupt from keyboard\n", 54);
    signal(SIGUSR1, sigHandler2);
    fflush(stdout);
    raise(SIGUSR1);
}

void sigHandler2(int sigNum) {
    write(STDOUT_FILENO, "#2 SIGNAL IN CHAIN - SIGUSR1: User defined signal 1\n", 52);
    fflush(stdout);
    signal(SIGCHLD, sigHandler3);
}

void sigHandler3(int sigNum) {
    write(STDOUT_FILENO, "#3 SIGNAL IN CHAIN - SIGCHLD: Child stopped or terminated\n", 58);
    fflush(stdout);
    signal(SIGFPE, sigHandler4);
    int num = 10;
    int zero = 0;
    int errorDivByZero = num / zero;
}

void sigHandler4(int sigNum) {
    write(STDOUT_FILENO, "#4 SIGNAL IN CHAIN - SIGFPE: Floating-point exception\n", 56);
    fflush(stdout);
    signal(SIGABRT, sigHandler5);
    abort();
}

void sigHandler5(int sigNum) {
    write(STDOUT_FILENO, "#5 SIGNAL IN CHAIN - SIGABRT: Abort signal from abort\n", 54);
    fflush(stdout);
    signal(SIGSEGV, sigHandler6);
    int *ptr;
    *ptr = 0;
}

void sigHandler6(int sigNum) {
    write(STDOUT_FILENO, "#6 SIGNAL IN CHAIN - SIGSEGV: Invalid memory reference\n", 55);
    fflush(stdout);
    signal(SIGALRM, sigHandler7);
    alarm(1);
    sleep(1);
}

void sigHandler7(int sigNum) {
    write(STDOUT_FILENO, "#7 SIGNAL IN CHAIN - SIGALRM: Timer signal from alarm(1)\n", 57);
    fflush(stdout);
    exit(1);
}

int main() {
    /**
     * NOTE: press CTRL C to start the trigger of the signals chain.
     */
    while (1) {
        // the fork idea is taken from the lecture
        pid_t pid;
        int status;
        signal(SIGINT, sigHandlerStartingChain);
        pid = fork();
        if (!pid) {
            exit(1);
        }
        wait(&status);
    }
}
