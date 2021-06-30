package dai.android.media.live

import android.content.Intent
import android.os.Bundle
import android.view.View
import android.widget.Button
import androidx.appcompat.app.AppCompatActivity

class MainActivity : AppCompatActivity() {

    private lateinit var btnGoToRtmpLive: Button

    private val viewClickListener = View.OnClickListener { v ->
        when (v.id) {
            R.id.btnGoToRtmpLive -> {
                val intent = Intent(this, RtmpActivity::class.java)
                startActivity(intent)
            }
        }
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        setContentView(R.layout.activity_main)

        btnGoToRtmpLive = findViewById(R.id.btnGoToRtmpLive)
        btnGoToRtmpLive.setOnClickListener(viewClickListener)
    }
}
