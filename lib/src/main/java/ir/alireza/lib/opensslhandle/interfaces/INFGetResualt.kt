package ir.alireza.lib.opensslhandle.interfaces
/**
 * @author AlirezaKarimi
 * Alireza.karimi.67@gmail.com
 */

interface INFGetResualt {
    fun success(data: String)
    fun failed(throws: Exception)
}