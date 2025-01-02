#include <dlfcn.h>
#include <jni.h>
#include <pthread.h>
#include <setjmp.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

jmp_buf jmp_buffer;

// Accessing this from a handler is possibly undefined behavior,
// but removing this does not impact the overall results anyways
volatile pthread_t expected_thread;

static void segv_handler(int sig) {
    (void)sig;

    char msg[] = "In segv_handler()\n";
    write(STDERR_FILENO, msg, sizeof(msg)-1);

    if (!pthread_equal(pthread_self(), expected_thread)) {
        char msg2[] = "Unexpected thread entered handler; exiting\n";
        write(STDERR_FILENO, msg2, sizeof(msg2)-1);
        _exit(EXIT_FAILURE);
    }

    siglongjmp(jmp_buffer, 1);
}

JNIEXPORT void JNICALL Java_Main_init(JNIEnv *env, jobject obj) {
    (void)env;
    (void)obj;

    fprintf(stderr, "Initializing...\n");

    struct sigaction sigsegv_sa = {
        .sa_handler = segv_handler,
        .sa_flags = SA_ONSTACK, // SA_ONSTACK gives SIGSEGV its own stack
    };

    // Handle stack overflow
    // See https://stackoverflow.com/a/7342398/13279557
    static char stack[SIGSTKSZ];
    stack_t ss = {
        .ss_size = SIGSTKSZ,
        .ss_sp = stack,
    };

    if (sigaltstack(&ss, NULL) == -1) {
        perror("sigaltstack");
        exit(EXIT_FAILURE);
    }

    if (sigfillset(&sigsegv_sa.sa_mask) == -1) {
        perror("sigfillset");
        exit(EXIT_FAILURE);
    }

    if (sigaction(SIGSEGV, &sigsegv_sa, NULL) == -1) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

	void *dll = dlopen("./mage.so", RTLD_NOW);
    if (!dll) {
        fprintf(stderr, "dlopen(): %s\n", dlerror());
    }
}

void recurse() {
    recurse();
}

JNIEXPORT void JNICALL Java_Main_foo(JNIEnv *env, jobject obj) {
    (void)env;
    (void)obj;

    expected_thread = pthread_self();

    if (sigsetjmp(jmp_buffer, 1)) {
        fprintf(stderr, "Jumped\n");
        return;
    }

    fprintf(stderr, "Recursing...\n");

    recurse();
}
