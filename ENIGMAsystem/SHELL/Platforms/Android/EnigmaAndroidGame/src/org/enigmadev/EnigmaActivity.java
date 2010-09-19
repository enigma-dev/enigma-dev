package org.enigmadev;
import java.io.IOException;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import android.app.Activity;
import android.content.Context;
import android.content.res.AssetFileDescriptor;
import android.content.res.AssetManager;
import android.media.AudioManager;
import android.media.SoundPool;
import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.view.MotionEvent;
import android.view.WindowManager;

public class EnigmaActivity extends Activity {
	
	static EnigmaActivity app;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        app=this;
        //getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, WindowManager.LayoutParams.FLAG_FULLSCREEN);
        mGLView = new EnigmaGLSurfaceView(this);
        setContentView(mGLView);
    }

    @Override
    protected void onPause() {
        super.onPause();
        mGLView.onPause();
    }

    @Override
    protected void onResume() {
        super.onResume();
        mGLView.onResume();
    }
    
    
    public SoundPool _sounds = new SoundPool(8,AudioManager.STREAM_MUSIC,0);
    public static int sound_load(String fname) {
        AssetManager am = app.getAssets();
        System.out.println("Sound load? :O");
        try {
            AssetFileDescriptor fd = am.openFd(fname);
            int sid = app._sounds.load(fd.getFileDescriptor(),fd.getStartOffset(),fd.getLength(),1);
            
            return sid;
        } catch(IOException e) {
        	System.out.println("Sound Error in sound_load (java) file doesn't exist");
        }
        
        return 0;
    }
    public static int sound_play(int sid) {
        app._sounds.play(sid,(float)1.0,(float)1.0,0,0,(float)1.0);
        return 1;
    }
    
    

    private GLSurfaceView mGLView;

    static {
        System.loadLibrary("ndkEnigmaGame");
    }
}

class EnigmaGLSurfaceView extends GLSurfaceView {
    public EnigmaGLSurfaceView(Context context) {
        super(context);
        mRenderer = new EnigmaRenderer();
        setRenderer(mRenderer);
    }

    public boolean onTouchEvent(final MotionEvent e) {
       
    	
        	/*if (e.getActionIndex() == MotionEvent.ACTION_POINTER_DOWN)
        	{
        		System.out.println("Mouse clicked java");
        		nativeMouse_Press((int)e.getX(),(int) e.getY());
        	} else if (e.getAction() == MotionEvent.ACTION_POINTER_UP)
        	{
        		
        		nativeMouse_Release((int)e.getX(),(int) e.getY());
        	}
        	nativeMouse_Press((int)e.getX(),(int) e.getY());*/
    	
    	
    	for (int i = 0; i<e.getPointerCount(); i++) {
            boolean masked = false;
            switch(e.getActionMasked()) {
                case MotionEvent.ACTION_POINTER_DOWN:
                case MotionEvent.ACTION_POINTER_UP:
                    masked = true;
                break;
            }
            if (masked && i != e.getActionIndex()) { continue; }
            float x = e.getX(i); 
            float y = e.getY(i);
            
            int pid = e.getPointerId(i);
            int type = 0;
            switch(e.getActionMasked()) {
                case MotionEvent.ACTION_DOWN: {nativeMouse_Press((int)x,(int) y);}; break;
                case MotionEvent.ACTION_MOVE: {}; break;
                case MotionEvent.ACTION_OUTSIDE: {}; break;
                case MotionEvent.ACTION_POINTER_DOWN: {}; break;
                case MotionEvent.ACTION_POINTER_UP: {}; break;
                case MotionEvent.ACTION_UP: {nativeMouse_Release((int)x,(int) y);}; break;
                case MotionEvent.ACTION_CANCEL: {}; break;
            }
            if (masked) { break; }
        }
        return true;
    }

    EnigmaRenderer mRenderer;

    //private static native void nativePause();
    private static native void nativeMouse_Press(int x, int y);
    private static native void nativeMouse_Release(int w, int y);
}

class EnigmaRenderer implements GLSurfaceView.Renderer {
    public void onSurfaceCreated(GL10 gl, EGLConfig config) {
        nativeInit();
    }

    public void onSurfaceChanged(GL10 gl, int w, int h) {
        //gl.glViewport(0, 0, w, h);
        nativeResize(w, h);
    }

    public void onDrawFrame(GL10 gl) {
        nativeRender();
    }

    private static native void nativeInit();
    private static native void nativeResize(int w, int h);
    private static native void nativeRender();
    private static native void nativeDone();
}

class EnigmaSoundEngine {
	
	static EnigmaSoundEngine app;
	
    
    
    public AssetManager getAssets() {
		return null;
		}

}
