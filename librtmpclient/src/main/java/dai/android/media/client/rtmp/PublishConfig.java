package dai.android.media.client.rtmp;

public final class PublishConfig {

    public final int videoWidth;
    public final int videoHeight;

    public final int soundFormat;
    public final int soundRate;
    public final int soundSize;
    public final int soundType;


    private PublishConfig(
            int vWidth, int vHeight,
            int audioFormat, int audioRate, int audioSize, int audioType
    ) {
        videoWidth = vWidth;
        videoHeight = vHeight;
        soundFormat = audioFormat;
        soundRate = audioRate;
        soundSize = audioSize;
        soundType = audioType;
    }


    public static class Build {

        private int vH = 0;
        private int vW = 0;

        // 10  - AAC
        private int sFormat = 10;
        // 3   - 44kHz
        private int sRate = 3;
        // 1   - 16-bit sample
        private int sSize = 1;
        // 1   - Stereo sound
        private int sType = 1;

        public PublishConfig builder() {
            return new PublishConfig(vW, vH, sFormat, sRate, sSize, sType);
        }

        public Build video(int width, int height) {
            vW = width;
            vH = height;
            return this;
        }

        public Build audio(int format, int rate, int size, int type) {
            sFormat = format;
            sRate = rate;
            sSize = size;
            sType = type;
            return this;
        }
    }

}
