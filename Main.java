class Main {
    private native void init();
    private native void foo();

    public static void main(String[] args) {
        new Main().run();
    }

    public void run() {
        System.loadLibrary("foo");

        init();

        long iteration = 0;
        while (true) {
            System.out.println("Iteration: " + ++iteration);
            foo();
        }
    }
}
