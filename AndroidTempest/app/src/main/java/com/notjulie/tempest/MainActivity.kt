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
    @RequiresApi(Build.VERSION_CODES.TIRAMISU)
    private val tempest : Tempest = Tempest()

    private var background: Paint = Paint()

    init {
        // initialize our background
        background.isAntiAlias = true
        background.isDither = true
        background.color = 0xFF000000.toInt()
        background.style = Paint.Style.FILL
    }


    @RequiresApi(Build.VERSION_CODES.P)
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding.root)

        // get our view
        view = findViewById(R.id.surfaceView)
        view.background = ColorDrawable()
        val surface : SurfaceHolder = view.holder
        surface.addCallback(this)

        // create our handler
        handler = Handler.createAsync(Looper.getMainLooper())

        // schedule something to do
        Timer().scheduleAtFixedRate(object : TimerTask() {
            @RequiresApi(Build.VERSION_CODES.TIRAMISU)
            override fun run() {
                val holder: SurfaceHolder? = currentHolder
                val canvas : Canvas? = holder?.lockHardwareCanvas()

                if (canvas != null) {
                    // draw the background
                    val width = canvas.width.toFloat()
                    val height = canvas.height.toFloat()
                    canvas.drawRect(0.0F, 0.0F, canvas.width.toFloat(), canvas.height.toFloat(), background)

                    // draw the vectors
                    val vectors = tempest.getCurrentVectors()
                    while (vectors.moveNext()) {
                        val startX = (vectors.getStartX().toFloat() + 32768) * width / 65536
                        val startY = (32767 - vectors.getStartY().toFloat()) * height / 65536
                        val endX = (vectors.getEndX().toFloat() + 32768) * width / 65536
                        val endY = (32767 - vectors.getEndY().toFloat()) * height / 65536
                        canvas.drawLine(
                            startX, startY, endX, endY,
                            getPaintForColor(vectors.getColor())
                        )
                    }

                    holder.unlockCanvasAndPost(canvas)
                }

            }
        },0L,100L)
    }

    private fun getPaintForColor(color: Int) : Paint {
        val paint = Paint()
        paint.isAntiAlias = true
        paint.isDither = true
        paint.color = color
        paint.style = Paint.Style.STROKE
        paint.strokeJoin = Paint.Join.ROUND
        paint.strokeCap = Paint.Cap.ROUND
        paint.strokeWidth = 10.0F
        return paint
    }

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
        paint.isAntiAlias = true
        paint.isDither = true
        paint.color = -0x100FF00
        paint.style = Paint.Style.STROKE
        paint.strokeJoin = Paint.Join.ROUND
        paint.strokeCap = Paint.Cap.ROUND
        paint.strokeWidth = 10.0F

        canvas.drawOval(0.0F,0.0F,100.0F,100.0F, paint)
        holder.unlockCanvasAndPost(canvas)
    }

    override fun surfaceDestroyed(holder: SurfaceHolder) {
        TODO("Not yet implemented")
    }


}