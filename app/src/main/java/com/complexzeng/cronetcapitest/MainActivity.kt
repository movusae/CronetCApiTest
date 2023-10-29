package com.complexzeng.cronetcapitest

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.util.Log
import android.widget.TextView
import com.complexzeng.cronetcapitest.databinding.ActivityMainBinding
import org.chromium.net.CronetEngine

class MainActivity : AppCompatActivity() {

  private lateinit var binding: ActivityMainBinding

  override fun onCreate(savedInstanceState: Bundle?) {
    super.onCreate(savedInstanceState)

    val cronet = CronetEngine.Builder(this).build()
    Log.i("cplx", "cronet version=" + cronet.versionString)

    binding = ActivityMainBinding.inflate(layoutInflater)
    setContentView(binding.root)

    // Example of a call to a native method
    binding.sampleText.text = stringFromJNI()
  }

  /**
   * A native method that is implemented by the 'cronetcapitest' native library,
   * which is packaged with this application.
   */
  external fun stringFromJNI(): String

  companion object {
    // Used to load the 'cronetcapitest' library on application startup.
    init {
      System.loadLibrary("cronetcapitest")
    }
  }
}