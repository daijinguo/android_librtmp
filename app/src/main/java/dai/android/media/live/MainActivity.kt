package dai.android.media.live

import android.os.Bundle
import androidx.appcompat.app.AppCompatActivity
import dai.android.media.client.rtmp.RtmpClient

class MainActivity : AppCompatActivity() {

    private var mediaConfig: MediaConfig? = null

    private var rtmpClient = RtmpClient()
    private val rtmpClientUrl =
        "rtmp://live-push.bilivideo.com/live-bvc/?streamname=live_522907740_44689574&key=4005e7f6505f20cf24ea21cefe7b5218&schedule=rtmp&pflag=1"

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        // Example of a call to a native method
        /// findViewById<TextView>(R.id.sample_text).text = stringFromJNI()

//        mediaConfig = MediaConfig.Builder("rtmp://xxx")
//            .setAudio(
//                MediaConfig.AudioBuilder().build()
//            )
//            .setVideo(
//                MediaConfig.VideoBuilder().build()
//            ).build()
//
//        mediaConfig!!.audio.channelConfig

        rtmpClient.open(rtmpClientUrl, true)

    }


    companion object {
        // Used to load the 'native-lib' library on application startup.
        init {
            System.loadLibrary("KuLive")
        }
    }
}