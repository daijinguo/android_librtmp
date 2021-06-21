package dai.android.media.client.rtmp;

import android.util.Log;

import java.io.IOException;

public class RtmpClient {

    private static final String TAG = "RtmpClient";

    static {
        System.loadLibrary("rtmp_client_jni");
    }


    public static final int RTMP_SUCCESS = 0; // rtmp success
    public final static int RTMP_READ_DONE = -1; // RTMP read has received an EOF or READ_COMPLETE from the server


    // exception const value

    public static final int RTMP_TRUE = 1;
    public static final int RTMP_FALSE = 0;

    // rtmp memory allocate failed
    public static final int RTMP_ERROR_MEM_ALLOC = -100;

    // rtmp url init failed
    public static final int RTMP_ERROR_URL_INIT = -200;
    // rtmp url connect failed
    public static final int RTMP_ERROR_URL_CONNECT = -201;
    // rtmp url connect stream failed
    public static final int RTMP_ERROR_URL_CONNECT_STREAM = -202;


    /**
     * RTMP client could not allocate memory for rtmp context structure
     */
    public static final int RTMP_ERROR_OPEN_ALLOC = -2;

    /**
     * RTMP client could not open the stream on server
     */
    public static final int RTMP_ERROR_OPEN_CONNECT_STREAM = -3;

    /**
     * Received an unknown option from the RTMP server
     */
    public final static int RTMP_ERROR_UNKNOWN_RTMP_OPTION = -4;

    /**
     * RTMP server sent a packet with unknown AMF type
     */
    public final static int RTMP_ERROR_UNKNOWN_RTMP_AMF_TYPE = -5;

    /**
     * DNS server is not reachable
     */
    public final static int RTMP_ERROR_DNS_NOT_REACHABLE = -6;

    /**
     * Could not establish a socket connection to the server
     */
    public final static int RTMP_ERROR_SOCKET_CONNECT_FAIL = -7;

    /**
     * SOCKS negotiation failed
     */
    public final static int RTMP_ERROR_SOCKS_NEGOTIATION_FAIL = -8;

    /**
     * Could not create a socket to connect to RTMP server
     */
    public final static int RTMP_ERROR_SOCKET_CREATE_FAIL = -9;

    /**
     * SSL connection requested but not supported by the client
     */
    public final static int RTMP_ERROR_NO_SSL_TLS_SUPP = -10;

    /**
     * Could not connect to the server for handshake
     */
    public final static int RTMP_ERROR_HANDSHAKE_CONNECT_FAIL = -11;

    /**
     * Handshake with the server failed
     */
    public final static int RTMP_ERROR_HANDSHAKE_FAIL = -12;

    /**
     * RTMP server connection failed
     */
    public final static int RTMP_ERROR_CONNECT_FAIL = -13;

    /**
     * Connection to the server lost
     */
    public final static int RTMP_ERROR_CONNECTION_LOST = -14;

    /**
     * Received an unexpected timestamp from the server
     */
    public final static int RTMP_ERROR_KEYFRAME_TS_MISMATCH = -15;

    /**
     * The RTMP stream received is corrupted
     */
    public final static int RTMP_ERROR_READ_CORRUPT_STREAM = -16;

    /**
     * Memory allocation failed
     */
    public final static int RTMP_ERROR_MEM_ALLOC_FAIL = -17;

    /**
     * Stream indicated a bad datasize, could be corrupted
     */
    public final static int RTMP_ERROR_STREAM_BAD_DATASIZE = -18;

    /**
     * RTMP packet received is too small
     */
    public final static int RTMP_ERROR_PACKET_TOO_SMALL = -19;

    /**
     * Could not send packet to RTMP server
     */
    public final static int RTMP_ERROR_SEND_PACKET_FAIL = -20;

    /**
     * AMF Encode failed while preparing a packet
     */
    public final static int RTMP_ERROR_AMF_ENCODE_FAIL = -21;

    /**
     * Missing a :// in the URL
     */
    public final static int RTMP_ERROR_URL_MISSING_PROTOCOL = -22;

    /**
     * Hostname is missing in the URL
     */
    public final static int RTMP_ERROR_URL_MISSING_HOSTNAME = -23;

    /**
     * The port number indicated in the URL is wrong
     */
    public final static int RTMP_ERROR_URL_INCORRECT_PORT = -24;

    /**
     * Error code used by JNI to return after throwing an exception
     */
    public final static int RTMP_ERROR_IGNORED = -25;

    /**
     * RTMP client has encountered an unexpected error
     */
    public final static int RTMP_ERROR_GENERIC_ERROR = -26;

    /**
     * A sanity check failed in the RTMP client
     */
    public final static int RTMP_ERROR_SANITY_FAIL = -27;


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

    public void open(String url, boolean publishMode) throws RtmpIOException {
        if (nativeHandler > 0) {
            close();
        }

        nativeHandler = nativeInit();
        Log.i(TAG, "native rtmp address: 0x" + Long.toHexString(nativeHandler));

        if (nativeHandler <= 0) {
            throw new RtmpIOException(RTMP_ERROR_MEM_ALLOC);
        }

        int result = nativeOpen(nativeHandler, url, publishMode, _timeoutInSecond);
        if (result != RTMP_TRUE) {
            throw new RtmpIOException(result);
        }

        Log.i(TAG, "open rtmp success");
    }

    public boolean isConnected() {
        return nativeIsConnected(nativeHandler);
    }

    public int read(byte[] data, int offset, int size) throws RtmpIOException, IllegalStateException {
        int result = nativeRead(nativeHandler, data, offset, size);
        if (result < RTMP_SUCCESS && result != RTMP_READ_DONE) {
            throw new RtmpIOException(result);
        }
        return result;
    }

    public int write(byte[] data, int offset, int size) throws RtmpIOException, IllegalStateException {
        int result = nativeWrite(nativeHandler, data, offset, size);
        if (result < RTMP_SUCCESS) {
            throw new RtmpIOException(result);
        }
        return result;
    }

    public int write(byte[] data) throws RtmpIOException, IllegalStateException {
        return write(data, 0, data.length);
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

    private native int nativeOpen(long handler, String url, boolean publish, int timeout);

    private native int nativeRead(long handler, byte[] data, int offset, int size) throws IllegalStateException;

    private native int nativeWrite(long handler, byte[] data, int offset, int size) throws IllegalStateException;

    private native int nativePause(long handler, boolean pause) throws IllegalStateException;

    private native boolean nativeIsConnected(long handler);

    private native void nativeClose(long handler);

}
