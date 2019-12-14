package ir.alireza.lib.opensslhandle

import android.os.Environment
import android.support.test.runner.AndroidJUnit4
import android.util.Log
import ir.alireza.lib.opensslhandle.customException.FetchException
import ir.alireza.lib.opensslhandle.customException.SaveException
import ir.alireza.lib.opensslhandle.interfaces.INFGetResualt
import org.junit.Test
import org.junit.runner.RunWith

import org.junit.Assert.*
import java.io.File
import android.support.test.InstrumentationRegistry



/**
 * Instrumented test, which will execute on an Android device.
 *
 * See [testing documentation](http://d.android.com/tools/testing).
 */
@RunWith(AndroidJUnit4::class)
class SaveCertTest {
    internal var timeout: Long = 60000
    internal var interval: Long = 1000
    internal var halt = Halt(timeout, interval)

    @Test
    fun SaveCertTest() {
        var res: String = "-1"
        val context = InstrumentationRegistry.getTargetContext()
        val sd_main = File(context.getExternalFilesDir(null).absolutePath+"/")
        if (!sd_main.exists()) {
            sd_main.mkdir()
        }

        OpenSSLHandler.INSTANCE.getPairCertificateAsync("https://www.google.com",object : INFGetResualt {
            override fun success(data: String) {
                if (data.isNotEmpty()){
                    Log.i("CERT","save server cert : ")
                    Log.i("CERT",data)
                    res="cert successfully fetch from server"

                }else{
                    Log.e("CERT","save server cert is Empty !!!!!! ")
                    res="error"

                }

            }

            override fun failed(throws: Exception) {
                res="error"
                if (throws is FetchException){
                    Log.e("CERT","Error get server cert")
                }else if (throws is SaveException){
                    Log.e("CERT","Error save server cert")
                }else{
                    Log.e("CERT","Error get server cert Unknown ERROR")
                }

            }

        },sd_main.absolutePath+"/tempc.txt")
        halt.execHalt(object: ConditionCheck {
            override fun condition(): Boolean {
                return res.contentEquals("-1")
            }

            override fun finalyAssert() {
                assertEquals(res,"cert successfully fetch from server")
            }

        })
    }
}
