package com.notjulie.tempest

import android.graphics.Canvas
import android.graphics.Paint
import android.graphics.drawable.ColorDrawable
import android.os.Build
import android.os.Bundle
import android.os.Handler
import android.os.Looper
import android.view.SurfaceHolder
import android.view.SurfaceView
import androidx.annotation.RequiresApi
import androidx.appcompat.app.AppCompatActivity
import com.notjulie.tempest.databinding.ActivityMainBinding
import java.util.Timer
import java.util.TimerTask

class MainActivity : AppCompatActivity(), SurfaceHolder.Callback {

    private lateinit var binding: ActivityMainBinding
    private lateinit var view : SurfaceView
    private lateinit var handler : Handler
    private var currentHolder: SurfaceHolder? = null
    private var location: Float = 0.0F
    private val background : Paint = Paint()

    @RequiresApi(Build.VERSION_CODES.P)
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding.root)

        // Example of a call to a native method
        //binding.sampleText.text = stringFromJNI()

        // get our view
        view = findViewById(R.id.surfaceView);
        view.background = ColorDrawable()
        val surface : SurfaceHolder = view.holder
        surface.addCallback(this)

        // create our handler
        handler = Handler.createAsync(Looper.getMainLooper())

        // schedule something to do
        Timer().scheduleAtFixedRate(object : TimerTask() {
            override fun run() {
                location = location + 10
                val holder: SurfaceHolder? = currentHolder
                if (holder == null)
                    return
                val canvas : Canvas = holder.lockHardwareCanvas()

                val backgroumd = Paint()
                backgroumd.setAntiAlias(true)
                backgroumd.setDither(true)
                backgroumd.setColor(0x1FFFF00)
                backgroumd.setStyle(Paint.Style.FILL)
                canvas.drawRect(0.0F, 0.0F, canvas.width.toFloat(), canvas.height.toFloat(), background)

                val paint = Paint()
                paint.setAntiAlias(true)
                paint.setDither(true)
                paint.setColor(-0x100FF00)
                paint.setStyle(Paint.Style.STROKE)
                paint.setStrokeJoin(Paint.Join.ROUND)
                paint.setStrokeCap(Paint.Cap.ROUND)
                paint.setStrokeWidth(10.0F)

                canvas.drawOval(location,location,100.0F,100.0F, paint)
                holder.unlockCanvasAndPost(canvas)
            }
        },0L,100L)
    }

    /**
     * A native method that is implemented by the 'tempest' native library,
     * which is packaged with this application.
     */
    private external fun stringFromJNI(): String

    companion object {
        // Used to load the 'tempest' library on application startup.
        init {
            System.loadLibrary("tempest")
        }
    }

    @RequiresApi(Build.VERSION_CODES.O)
    override fun surfaceCreated(holder: SurfaceHolder) {
        currentHolder = holder
        /*val canvas : Canvas = holder.lockHardwareCanvas()
        val paint = Paint()
        canvas.drawOval(0.0F,0.0F,100.0F,100.0F, paint)
        holder.unlockCanvasAndPost(canvas)*/
    }

    @RequiresApi(Build.VERSION_CODES.O)
    override fun surfaceChanged(holder: SurfaceHolder, format: Int, width: Int, height: Int) {
        val canvas : Canvas = holder.lockHardwareCanvas()
        val paint = Paint()
        paint.setAntiAlias(true)
        paint.setDither(true)
        paint.setColor(-0x100FF00)
        paint.setStyle(Paint.Style.STROKE)
        paint.setStrokeJoin(Paint.Join.ROUND)
        paint.setStrokeCap(Paint.Cap.ROUND)
        paint.setStrokeWidth(10.0F)

        canvas.drawOval(0.0F,0.0F,100.0F,100.0F, paint)
        holder.unlockCanvasAndPost(canvas)
    }

    override fun surfaceDestroyed(holder: SurfaceHolder) {
        TODO("Not yet implemented")
    }
}