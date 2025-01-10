#include <dlfcn.h>
#include <jni.h>
#include <setjmp.h>
#include <signal.h>
#include <stdlib.h>

jmp_buf jmp_buffer;

static char *base;
static char *top;

static void segv_handler(int sig) {
    (void)sig;
    siglongjmp(jmp_buffer, 1);
}

JNIEXPORT void JNICALL Java_Main_foo(JNIEnv *env, jobject obj) {
    (void)env;
    (void)obj;
    char b;
    base = &b;

    struct sigaction sigsegv_sa = {
        .sa_handler = segv_handler
    };
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

    if (sigsetjmp(jmp_buffer, 1)) {
        fprintf(stderr, "Jumped %p %p %ld\n", base, top, (base - top) / 1024);
        return;
    }

    char c;
    top = &c;
    while (1) {
        top--;
        *top = 1;
    }
}
