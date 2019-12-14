# boringsslhandler
A library for easy to use boringssl in android

The library now only has the ability to download and save the pair certificate chain but I will add more method in future.
You dont need to immpliment your own JNI , just use opensslhandler class :

sample in kotline:

1-getServerCert:
```kotline
OpenSSLHandler.INSTANCE.getPairCertificateAsync("YOUR ADDRESS",object : INFGetResualt {
    override fun success(data: String) {

    }
    
    override fun failed(throws: Exception) {
        if (throws is FetchException){
            Log.e("CERT","Error get server cert")
        }else{
            Log.e("CERT","Error get server cert Unknown ERROR")
            }
        }
})
```

2-saveServerCert:
```kotline
OpenSSLHandler.INSTANCE.getPairCertificateAsync(YOUR_ADDRESS,object : INFGetResualt {
    override fun success(data: String) {

    }

    override fun failed(throws: Exception) {
        if (throws is FetchException){
            Log.e("CERT","Error get server cert")
        }else if (throws is SaveException){
            Log.e("CERT","Error save server cert")
        }else{
            Log.e("CERT","Error get server cert Unknown ERROR")
        }
    }
},"SAVE_LOCATION_AND_NAME")
```
