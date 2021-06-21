package dai.android.media.live

import android.os.Bundle
import android.view.View
import android.widget.Button
import androidx.appcompat.app.AppCompatActivity
import dai.android.media.client.rtmp.RtmpClient

class MainActivity : AppCompatActivity() {

    private var mediaConfig: MediaConfig? = null

    private var rtmpClient = RtmpClient()
    private val rtmpClientUrl =
        "rtmp://live-push.bilivideo.com/live-bvc/?streamname=live_522907740_44689574&key=4005e7f6505f20cf24ea21cefe7b5218&schedule=rtmp&pflag=1"


    private lateinit var btnStart: Button
    private lateinit var btnFinish: Button

    private val buttonClick = View.OnClickListener { v ->
        when (v.id) {
            R.id.btn_start -> {
                rtmpClient.open(rtmpClientUrl, true)
            }

            R.id.btn_finish -> {
                rtmpClient.close()
            }
        }
    }


    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        btnStart = findViewById(R.id.btn_start)
        btnStart.setOnClickListener(buttonClick)

        btnFinish = findViewById(R.id.btn_finish)
        btnStart.setOnClickListener(buttonClick)
    }


    companion object {
        // Used to load the 'native-lib' library on application startup.
        init {
            System.loadLibrary("KuLive")
        }
    }
}