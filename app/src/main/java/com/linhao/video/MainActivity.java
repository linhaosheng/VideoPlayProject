package com.linhao.video;

import android.os.Environment;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.Surface;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.widget.TextView;

import com.linhao.video.listener.VideoPlayListsner;
import com.linhao.video.util.ExecutorUtils;

import java.io.File;

public class MainActivity extends AppCompatActivity implements SurfaceHolder.Callback, VideoPlayListsner {

    private static final String TAG = "MainActivity===";
    private SurfaceView surfaceView;
    private SurfaceHolder surfaceHolder;

    static {
        System.loadLibrary("native-lib");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        initView();

    }

    private void initView() {
        surfaceView = (SurfaceView) findViewById(R.id.surfaceView);
        surfaceHolder = surfaceView.getHolder();
        surfaceHolder.addCallback(this);

    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();


    @Override
    public void surfaceCreated(SurfaceHolder holder) {

        Runnable runnable = new Runnable() {
            @Override
            public void run() {
                String path = "/storage/extsd/video/A/20180101000003A.mp4";
                Log.d(TAG, "path==" + path);
                File file = new File(path);
                if (file.exists()){
                    Log.d(TAG,"file exit");
                }
                try {
                    setSurface(surfaceHolder.getSurface(), "/storage/extsd/video/A/20180101221156A11111.mp4");
                }catch (Exception e){
                    e.printStackTrace();
                }
            }
        };
        ExecutorUtils.execute(runnable);

    }

    @Override
    public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {

    }

    @Override
    public void surfaceDestroyed(SurfaceHolder holder) {

    }


    public native void setSurface(Surface surface, String path);


    @Override
    public void error(String error) {
       Log.d(TAG,"22212121221");
    }

    @Override
    public void success() {

    }
}
