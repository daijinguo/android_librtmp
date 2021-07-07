package dai.android.leak;

import android.app.Application;

public final class LibraryHook {
    private final static String TAG = "LibraryHook";
    private static final String SO_NAME = "native_hooker";

    static {
        System.loadLibrary(SO_NAME);
    }

    public static void init(Application app) {
        String[] libs = new String[]{
                "native_hooker",
                "rtmp_client_jni",
                "KuLive",
        };
        hookELF(libs);
    }

    private static native boolean hookELF(String[] files);

    //!! private static native void init();
}

