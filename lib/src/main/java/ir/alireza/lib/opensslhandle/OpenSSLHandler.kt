package ir.alireza.lib.opensslhandle
import ir.alireza.lib.opensslhandle.customException.FetchException
import ir.alireza.lib.opensslhandle.customException.SaveException
import ir.alireza.lib.opensslhandle.interfaces.INFGetResualt
import kotlin.concurrent.thread

/**
 * <p>Openssl controller</p>
 * This class used to access to openssl method
 *
 * @author AlirezaKarimi
 * Alireza.karimi.67@gmail.com
 * @see ir.alireza.lib.opensslhandle.interfaces
 * @see ir.alireza.lib.opensslhandle.customException
 */
class OpenSSLHandler private constructor(){
    private object Holder { val INSTANCE = OpenSSLHandler() }

    companion object {
        val INSTANCE: OpenSSLHandler by lazy { Holder.INSTANCE }
    }

    /**
     * <p>get pair certificate</p>
     * this class used to get pair certificate in Async mode
     */
    fun getPairCertificateAsync(url: String, delegate: INFGetResualt, file: String = ""){
        thread {
            val cert: String=JniHandler.instance.fetchPairCertificateChain(url)
            if (!cert.isNullOrEmpty()){
                if (file.isNotEmpty()){
                    if (FileHelper.instance.saveToFile(file,cert))
                        delegate.success(cert)
                    else
                        delegate.failed(throw SaveException("Save Certificate Exception"))
                }else{
                    delegate.success(cert)
                }
            }else{
                delegate.failed(throw FetchException("Fetch Certificate Exception"))
            }
        }
    }

    /**
     * <p>get pair certificate</p>
     * this class used to get pair certificate in sync mode
     * ATTENTION: This is a blocking method,dont use from main thread
     */
    @Throws(SaveException::class,FetchException::class)
    fun getPairCertificateSync(url: String,file: String = ""): Boolean{
        val cert: String=JniHandler.instance.fetchPairCertificateChain(url)
        if (!cert.isNullOrEmpty()){
            if (file.isNotEmpty()){
                if (FileHelper.instance.saveToFile(file,cert))
                    return true
                else
                    throw SaveException("Save Certificate Exception")
            }else{
                return false
            }
        }else{
            throw FetchException("Fetch Certificate Exception")
        }
    }

}