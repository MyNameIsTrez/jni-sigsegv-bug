class Main {
    private native void foo();

    public static void main(String[] args) {
        new Main().run();
    }

    public void run() {
        System.loadLibrary("foo");

        long iteration = 0;
        for (int i = 0; i < 2; i++) {
            System.out.println("Iteration: " + ++iteration);
            foo();
        }
    }
}
