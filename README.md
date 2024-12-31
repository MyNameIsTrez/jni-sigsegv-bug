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

Compile `foo.c` to `foo.so`:

```bash
gcc foo.c -o libfoo.so -shared -fPIC -g -Wall -Wextra -Wpedantic -Werror -Wfatal-errors -Wno-infinite-recursion -I/usr/lib/jvm/jdk-23.0.1-oracle-x64/include -I/usr/lib/jvm/jdk-23.0.1-oracle-x64/include/linux
```

Followed by trying this a bunch of times:

```bash
java -Xcheck:jni -Djava.library.path=. Main.java
```

You can use this in a second terminal to kill the program when it hangs:

```bash
pkill -9 -f 'java -Xcheck:jni -Djava.library.path=. Main.java'
```
