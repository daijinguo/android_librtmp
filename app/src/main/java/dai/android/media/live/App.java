package dai.android.media.live;

import android.app.Application;
import android.content.Context;

import com.qiu.liang.leak.NativeLeakProf;

import dai.android.leak.LibraryHook;

public class App extends Application {

    @Override
    protected void attachBaseContext(Context base) {
        super.attachBaseContext(base);

        // NativeLeakProf.init(this);
    }

    @Override
    public void onCreate() {
        super.onCreate();

        LibraryHook.start(this);
    }
}
