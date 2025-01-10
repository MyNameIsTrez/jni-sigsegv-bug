I created this repository to help me formulate [this Stack Overflow question of mine](https://stackoverflow.com/questions/79327777/is-this-a-bug-in-the-jvm-or-in-nasm), called "Is this a bug in the JVM, or in NASM?".

To see this error:

```
Initializing...
dlopen(): ./mage.so: undefined symbol: game_fn_define_human
Iteration: 1
Recursing...
In segv_handler()
Jumped
FATAL ERROR in native method: Static field ID passed to JNI
        at java.io.FileOutputStream.writeBytes(java.base@23.0.1/Native Method)
        ...
        at com.sun.tools.javac.launcher.SourceLauncher.main(jdk.compiler@23.0.1/SourceLauncher.java:78)
^CAborted (core dumped)
```

Compile `foo.c` to `foo.so` (you will need to replace the jdk include paths here with your own, which `ls /usr/lib/jvm` can help with):

```bash
gcc foo.c -o libfoo.so -shared -fPIC -g -Wall -Wextra -Wpedantic -Werror -Wfatal-errors -Wno-infinite-recursion -I/usr/lib/jvm/jdk-23.0.1-oracle-x64/include -I/usr/lib/jvm/jdk-23.0.1-oracle-x64/include/linux
```

Then assemble `mage.s` to `mage.o`:

```bash
nasm mage.s -felf64
```

And link `mage.o` to `mage.so`:

```bash
ld mage.o -o mage.so -shared
```

Finally we run `Main.java` in an infinite loop, which should eventually print `FATAL ERROR in native method: Static field ID passed to JNI`:

```bash
while true; do java -Xcheck:jni -Djava.library.path=. Main.java; done
```

Hitting `Ctrl`+`Z` a few times will suspend the loop, where you can then use `kill %%` to kill it.

For some reason generating `mage.o` from compiling `mage.c`, instead of from assembling `mage.s`, never prints the error:

```bash
gcc mage.c -c
```
