package dai.android.media.client.rtmp;

import android.util.Log;

import java.io.IOException;

public class RtmpClient {

    private static final String TAG = "RtmpClient";

    static {
        System.loadLibrary("rtmp_client_jni");
    }


    public static final int RTMP_SUCCESS = 0; // rtmp success
    public final static int RTMP_READ_DONE = -1; // RTMP readRaw has received an EOF or READ_COMPLETE from the server


    // exception const value

    public static final int RTMP_TRUE = 1;
    public static final int RTMP_FALSE = 0;

    // rtmp memory allocate failed
    public static final int RTMP_ERROR_OBJECT_NOT_ALLOC = -100;

    // rtmp url init failed
    public static final int RTMP_ERROR_URL_SETUP = -200;
    // rtmp url connect failed
    public static final int RTMP_ERROR_URL_CONNECT = -201;
    // rtmp url connect stream failed
    public static final int RTMP_ERROR_URL_CONNECT_STREAM = -202;
    // rtmp not connected
    public static final int RTMP_NOT_CONNECTED = -203;


    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    public static class RtmpIOException extends IOException {
        private final int errorCode;

        public RtmpIOException(int code) {
            super("rtmp error code: " + code);
            errorCode = code;
        }

        public int getErrorCode() {
            return errorCode;
        }
    }


    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    private long nativeHandler = 0L;

    private int _timeoutInSecond = 10; // timeout 10 seconds

    public RtmpClient() {
    }

    public void setTimeout(int timeoutInSecond) {
        if (timeoutInSecond > 0) {
            _timeoutInSecond = timeoutInSecond;
        }
    }

    public void publish(String url, PublishConfig config) throws RtmpIOException {
        if (nativeHandler > 0) {
            close();
        }

        nativeHandler = nativeInit();
        Log.i(TAG, "publish: native rtmp address: 0x" + Long.toHexString(nativeHandler));

        if (nativeHandler <= 0) {
            throw new RtmpIOException(RTMP_ERROR_OBJECT_NOT_ALLOC);
        }

        int result = nativeOpen(nativeHandler, url, true, _timeoutInSecond, config);
        if (result != RTMP_TRUE) {
            throw new RtmpIOException(result);
        }

        Log.i(TAG, "publish rtmp success");
    }

    public void open(String url) throws RtmpIOException {
        if (nativeHandler > 0) {
            close();
        }

        nativeHandler = nativeInit();
        Log.i(TAG, "open: native rtmp address: 0x" + Long.toHexString(nativeHandler));

        if (nativeHandler <= 0) {
            throw new RtmpIOException(RTMP_ERROR_OBJECT_NOT_ALLOC);
        }

        int result = nativeOpen(nativeHandler, url, false, _timeoutInSecond, null);
        if (result != RTMP_TRUE) {
            throw new RtmpIOException(result);
        }

        Log.i(TAG, "open rtmp success");
    }

    public boolean isConnected() {
        return nativeIsConnected(nativeHandler);
    }

    public int readRaw(byte[] data, int offset, int size) throws RtmpIOException, IllegalStateException {
        int result = nativeRead(nativeHandler, data, offset, size);
        if (result < RTMP_SUCCESS && result != RTMP_READ_DONE) {
            throw new RtmpIOException(result);
        }
        return result;
    }

    public int writeRaw(byte[] data, int offset, int size) throws RtmpIOException, IllegalStateException {
        int result = nativeWrite(nativeHandler, data, offset, size);
        if (result < RTMP_SUCCESS) {
            throw new RtmpIOException(result);
        }
        return result;
    }

    public int writeRaw(byte[] data) throws RtmpIOException, IllegalStateException {
        return writeRaw(data, 0, data.length);
    }

    public int writeAudio(byte[] data, int offset, int length, long timestamp) throws RtmpIOException, IllegalStateException {
        return nativeWriteAudio(nativeHandler, data, offset, length, timestamp);
    }

    public boolean pause(boolean pause) throws RtmpIOException, IllegalStateException {
        int result = nativePause(nativeHandler, pause);
        if (result != RTMP_SUCCESS) {
            throw new RtmpIOException(result);
        }
        return true;
    }

    public void close() {
        nativeClose(nativeHandler);
        nativeHandler = 0L;
    }


    private native long nativeInit();

    private native int nativeOpen(long handler, String url, boolean publish, int timeout, PublishConfig config);

    private native int nativeRead(long handler, byte[] data, int offset, int size) throws IllegalStateException;

    private native int nativeWrite(long handler, byte[] data, int offset, int size) throws IllegalStateException;

    private native int nativeWriteAudio(long handler, byte[] data, int offset, int length, long timestamp) throws IllegalStateException;

    private native int nativePause(long handler, boolean pause) throws IllegalStateException;

    private native boolean nativeIsConnected(long handler);

    private native void nativeClose(long handler);

}
