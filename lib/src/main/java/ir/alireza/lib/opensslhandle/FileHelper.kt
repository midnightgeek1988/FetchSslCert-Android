package ir.alireza.lib.opensslhandle

import java.io.File

/**
 * @author AlirezaKarimi
 * Alireza.karimi.67@gmail.com
 */
internal class FileHelper {
    private object Holder { val INSTANCE = FileHelper() }

    companion object {
        val instance: FileHelper by lazy { Holder.INSTANCE }
    }

    fun saveToFile(address: String, data: String): Boolean{
        File(address).printWriter().use { out->out.write(data) }
        return checkFileExist(address)
    }

    fun checkFileExist(address: String): Boolean{
        return File(address).exists()
    }
}