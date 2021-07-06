package dai.android.leak;

import android.content.Context;
import android.os.FileObserver;
import android.os.Handler;
import android.os.HandlerThread;
import android.os.Looper;
import android.os.Message;
import android.os.Process;
import android.util.Log;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;

import java.io.BufferedInputStream;
import java.io.ByteArrayOutputStream;
import java.io.Closeable;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.util.HashSet;
import java.util.Set;
import java.util.concurrent.atomic.AtomicBoolean;

public final class LibraryHook {
    private static final String TAG = "LibraryHook";
    private static final String LIBNAME = "NativeLeakReporter";
    private static final String PROC_MAP_FILE = new String("/proc/" + Process.myPid() + "/maps");

    static {
        System.loadLibrary(LIBNAME);
    }

    private static void closeQuietly(Closeable... closeables) {
        if (null == closeables)
            return;

        for (Closeable c : closeables) {
            if (null != c) {
                try {
                    c.close();
                } catch (IOException ignore) {
                }
            }
        }
    }

    private static String reader(File target) throws IOException {
        if (null == target || !target.exists()) {
            return "";
        }

        BufferedInputStream inputStream = null;
        ByteArrayOutputStream bos = null;

        try {
            inputStream = new BufferedInputStream(new FileInputStream(target));
            bos = new ByteArrayOutputStream();
            byte[] buffer = new byte[4096];
            int length;

            while ((length = inputStream.read(buffer)) != -1) {
                bos.write(buffer, 0, length);
            }
            return new String(bos.toByteArray());
        } catch (IOException ioe) {
            Log.e(TAG, Log.getStackTraceString(ioe));
            throw ioe;
        } finally {
            closeQuietly(inputStream, bos);
        }
    }

    //============================================================================================
    private static final Set<String> elfFiles = new HashSet<>();

    private static void scanLibrary(Context context) {
        if (null == context)
            return;

        String pkgName = context.getPackageName();
        try {
            String strLines = reader(new File(PROC_MAP_FILE));
            if (null == strLines) {
                Log.e(TAG, "read no proc data in proc");
                return;
            }

            int sizeOld = elfFiles.size();
            String[] split = strLines.split("\n");
            for (String line : split) {
                /// Log.i(TAG, ">>> " + line);

                if (!line.endsWith(".so") || line.contains(LIBNAME))
                    continue;

                if (line.contains(pkgName)) {
                    String elfFile = line.substring(line.lastIndexOf("/") + 1, line.lastIndexOf(".so"));
                    if (elfFiles.contains(elfFile))
                        continue;

                    elfFiles.add(elfFile);
                    Log.i(TAG, "find new shared library: " + line);
                }
            }


        } catch (IOException e) {
            Log.e(TAG, Log.getStackTraceString(e));
        }
    }


    private static final int CMD_SCAN_LIBS = 100;

    private static final class ProcMapHandler extends Handler {
        public ProcMapHandler(Looper looper) {
            super(looper);
        }

        @Override
        public void handleMessage(@NonNull Message msg) {
            if (msg.what == CMD_SCAN_LIBS) {
                scanLibrary(app);
            }
        }
    }

    private static final HandlerThread scanThread = new HandlerThread("scanThread");
    private static final AtomicBoolean started = new AtomicBoolean(false);
    private static Handler H = null;
    private static Context app = null;

    private static class ProcMapFileObserver extends FileObserver {
        public ProcMapFileObserver(String path) {
            super(path, FileObserver.ALL_EVENTS);
        }

        @Override
        public void onEvent(int event, @Nullable String path) {
            Log.w(TAG, "event: " + event);

            if (event == FileObserver.MODIFY) {
                Log.i(TAG, "event = " + event + " modify");
                H.obtainMessage(CMD_SCAN_LIBS).sendToTarget();
            }
        }
    }

    private static final ProcMapFileObserver fileObserver = new ProcMapFileObserver(PROC_MAP_FILE);

    public static void scan() {
        H.obtainMessage(CMD_SCAN_LIBS).sendToTarget();
    }


    public static void start(Context context) {
        if (null == context) {
            return;
        }
        app = context.getApplicationContext();

        if (started.compareAndSet(false, true)) {
            Log.i(TAG, "start to scan library");
            scanThread.start();
            H = new ProcMapHandler(scanThread.getLooper());
            H.sendEmptyMessage(CMD_SCAN_LIBS);

            // n not use FileObserver for this code
            fileObserver.startWatching();
        }
    }

    public static void end() {

        if (started.get()) {
            Log.i(TAG, "end the scan library");
            fileObserver.stopWatching();
            scanThread.quitSafely();
            started.set(false);
        }
    }
}
