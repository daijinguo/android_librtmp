package dai.android.media.live

import android.os.Bundle
import android.util.Log
import android.view.SurfaceHolder
import android.view.SurfaceView
import android.view.View
import android.widget.Button
import androidx.appcompat.app.AppCompatActivity
import com.qiu.liang.leak.NativeLeakProf
import dai.android.leak.LibraryHook
import dai.android.media.client.rtmp.RtmpClient

class RtmpActivity : AppCompatActivity() {

    private var mediaConfig: MediaConfig? = null

    private var rtmpClient = RtmpClient()

    // private val rtmpClientUrl =
    //     "rtmp://live-push.bilivideo.com/live-bvc/?streamname=live_522907740_44689574&key=4005e7f6505f20cf24ea21cefe7b5218&schedule=rtmp&pflag=1"
    private val rtmpClientUrl = "rtmp://58.200.131.2:1935/livetv/hunantv"

    private lateinit var btnStart: Button
    private lateinit var btnFinish: Button
    private lateinit var displayView: SurfaceView

    private val buttonClick = View.OnClickListener { v ->
        when (v.id) {
            R.id.btn_start -> {
                // rtmpClient.open(rtmpClientUrl, true)
                /// NativeLeakProf.dumpLeakStack()
                /// Log.i(TAG, NativeLeakProf.dumpLeakInfo())

                LibraryHook.scan();

            }

            R.id.btn_finish -> {
                rtmpClient.close()
            }
        }
    }

    private val holderCallback = object : SurfaceHolder.Callback {
        override fun surfaceCreated(holder: SurfaceHolder) {
            Log.i(TAG, "surfaceCreated")

            rtmpClient.open(rtmpClientUrl)
        }

        override fun surfaceChanged(holder: SurfaceHolder, format: Int, width: Int, height: Int) {
            Log.i(TAG, "surfaceChanged: fmt=$format width=$width height=$height")

            // val config = PublishConfig.Build().video(width, height).builder()
            // rtmpClient.publish(rtmpClientUrl, config)
            rtmpClient.open(rtmpClientUrl)
        }

        override fun surfaceDestroyed(holder: SurfaceHolder) {
        }
    }


    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_rtmp_live)

        btnStart = findViewById(R.id.btn_start)
        btnStart.setOnClickListener(buttonClick)

        btnFinish = findViewById(R.id.btn_finish)
        btnStart.setOnClickListener(buttonClick)

        displayView = findViewById(R.id.displayView)
        displayView.holder.addCallback(holderCallback)
    }


    companion object {
        init {
            System.loadLibrary("KuLive")
        }

        private const val TAG = "MainActivity"
    }
}