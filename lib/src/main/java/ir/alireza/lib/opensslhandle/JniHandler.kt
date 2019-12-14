package ir.alireza.lib.opensslhandle
/**
 * @author AlirezaKarimi
 * Alireza.karimi.67@gmail.com
 */

internal class JniHandler {
    external fun fetchPairCertificateChain(address: String): String

    private object Holder { val INSTANCE = JniHandler() }

    companion object {
        init {
            System.loadLibrary("open-ssl-handle")
        }
        val instance: JniHandler by lazy { Holder.INSTANCE }
    }

}