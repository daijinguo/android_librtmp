package dai.android.media.live;

import android.app.Application;
import android.content.Context;

import com.qiu.liang.leak.NativeLeakProf;

public class App extends Application {

    @Override
    protected void attachBaseContext(Context base) {
        super.attachBaseContext(base);

        NativeLeakProf.init(this);
    }
}
