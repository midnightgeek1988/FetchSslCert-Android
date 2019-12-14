package ir.alireza.lib.opensslhandle

import android.support.test.runner.AndroidJUnit4
import android.util.Log
import ir.alireza.lib.opensslhandle.customException.FetchException
import ir.alireza.lib.opensslhandle.interfaces.INFGetResualt
import org.junit.Test
import org.junit.runner.RunWith
import org.junit.Assert.*

/**
 * Instrumented test, which will execute on an Android device.
 *
 * See [testing documentation](http://d.android.com/tools/testing).
 */
@RunWith(AndroidJUnit4::class)
class GetCertTest {
    internal var timeout: Long = 60000
    internal var interval: Long = 1000
    internal var halt = Halt(timeout, interval)

    @Test
    fun GetCertTest() {
        var res: String = "-1"
        OpenSSLHandler.INSTANCE.getPairCertificateAsync("https://www.google.com",object : INFGetResualt {
            override fun failed(throws: Exception) {
                if (throws is FetchException){
                    Log.e("CERT","Error get server cert")
                    res="error"
                }else{
                    Log.e("CERT","Error get server cert Unknown ERROR")
                    res="error"
                }
            }

            override fun success(data: String) {
                if (data.isNotEmpty()){
                    Log.i("CERT","get server cert : ")
                    Log.i("CERT",data)
                    res="cert successfully fetch from server"
                }else{
                    Log.e("CERT","get server cert is Empty !!!!!! ")
                    res="error"
                }
            }


        })
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
