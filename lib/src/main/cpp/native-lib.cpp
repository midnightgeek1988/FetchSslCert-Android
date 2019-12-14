/*
 * Alireza Karimi
 * This is a JNI class use to work with boringSSL
 */
#include <jni.h>
#include <string.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "boringssl/include/openssl/ssl.h"
#include "boringssl/include/openssl/err.h"
#include <android/log.h>

#define APPNAME "Alireza"


#ifdef __cplusplus
extern "C" {
#endif

int create_socket(char url_str[], BIO *out);
char *X509_to_PEM(X509 *cert);
JNIEXPORT jstring JNICALL
Java_ir_alireza_lib_opensslhandle_JniHandler_fetchPairCertificateChain(JNIEnv *env, jobject thiss,jstring address) {
    __android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "START_1");

    const char *temp = env->GetStringUTFChars(address, 0);

    char dest_url[strlen(temp)];
    strcpy(dest_url,temp);
    BIO *outbio = NULL;
    X509 *cert = NULL;
    STACK_OF(X509) *certstack;
    X509_NAME *certname = NULL;
    const SSL_METHOD *method;
    SSL_CTX *ctx;
    SSL *ssl;
    int server = 0;
    int ret, i;
    __android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "START_2");

    /* ---------------------------------------------------------- *
     * These function calls initialize openssl for correct work.  *
     * ---------------------------------------------------------- */
    OpenSSL_add_all_algorithms();
    ERR_load_BIO_strings();
    ERR_load_crypto_strings();
    SSL_load_error_strings();
    __android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "START_3");

    /* ---------------------------------------------------------- *
     * Create the Input/Output BIO's.                             *
     * ---------------------------------------------------------- */
    outbio = BIO_new(BIO_s_file());
    outbio = BIO_new_fp(stdout, BIO_NOCLOSE);
    __android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "START_4");

    /* ---------------------------------------------------------- *
     * initialize SSL library and register algorithms             *
     * ---------------------------------------------------------- */
    if (SSL_library_init() < 0)
        BIO_printf(outbio, "Could not initialize the OpenSSL library !\n");

    /* ---------------------------------------------------------- *
     * Set SSLv2 client hello, also announce SSLv3 and TLSv1      *
     * ---------------------------------------------------------- */
    method = SSLv23_client_method();

    /* ---------------------------------------------------------- *
     * Try to create a new SSL context                            *
     * ---------------------------------------------------------- */
    if ((ctx = SSL_CTX_new(method)) == NULL)
        BIO_printf(outbio, "Unable to create a new SSL context structure.\n");

    /* ---------------------------------------------------------- *
     * Disabling SSLv2 will leave v3 and TSLv1 for negotiation    *
     * ---------------------------------------------------------- */
    SSL_CTX_set_options(ctx, SSL_OP_NO_SSLv2);

    /* ---------------------------------------------------------- *
     * Create new SSL connection state object                     *
     * ---------------------------------------------------------- */
    ssl = SSL_new(ctx);

    /* ---------------------------------------------------------- *
     * Make the underlying TCP socket connection                  *
     * ---------------------------------------------------------- */
    server = create_socket(dest_url, outbio);
    if (server==-1){
        return nullptr;
    }
    if (server != 0)
        BIO_printf(outbio, "Successfully made the TCP connection to: %s.\n", dest_url);

    /* ---------------------------------------------------------- *
     * Attach the SSL session to the socket descriptor            *
     * ---------------------------------------------------------- */
    SSL_set_fd(ssl, server);

    /* ---------------------------------------------------------- *
     * Try to SSL-connect here, returns 1 for success             *
     * ---------------------------------------------------------- */
    if (SSL_connect(ssl) != 1) {
        BIO_printf(outbio, "Error: Could not build a SSL session to: %s.\n", dest_url);
        return env->NewStringUTF("");
    }else {
        BIO_printf(outbio, "Successfully enabled SSL/TLS session to: %s.\n", dest_url);
    }
    /* ---------------------------------------------------------- *
     * Get the remote certificate into the X509 structure         *
     * ---------------------------------------------------------- */
    cert = SSL_get_peer_certificate(ssl);
    certstack = SSL_get_peer_cert_chain(ssl);

    if (cert == NULL) {
        BIO_printf(outbio, "Error: Could not get a certificate from: %s.\n", dest_url);
        return env->NewStringUTF("");
    }else {
        BIO_printf(outbio, "Retrieved the server's certificate from: %s.\n", dest_url);
    }

    /* ---------------------------------------------------------- *
     * extract various certificate information                    *
     * -----------------------------------------------------------*/
    certname = X509_NAME_new();
    certname = X509_get_subject_name(cert);

    const char * buf = X509_NAME_oneline(certname, 0, 0);
    char * cert_value_full="";

    for (x509_st *x : certstack){
        BIO_printf(outbio, "Retrieved the server's certificate from: %s.\n", dest_url);
        const char * cert_value = X509_to_PEM(x);
        char * str3 = (char *) malloc(1 + strlen(cert_value_full)+ strlen(cert_value) );
        strcpy(str3, cert_value_full);
        strcat(str3, cert_value);
        cert_value_full=str3;
    }

    __android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "DATA %s \n",buf);
    __android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "DATA %s \n",cert_value_full);

    /* ---------------------------------------------------------- *
     * display the cert subject here                              *
     * -----------------------------------------------------------*/
    BIO_printf(outbio, "Displaying the certificate subject data:\n");
    X509_NAME_print_ex(outbio, certname, 0, 0);
    BIO_printf(outbio, "\n");

    /* ---------------------------------------------------------- *
     * Free the structures we don't need anymore                  *
     * -----------------------------------------------------------*/
    SSL_free(ssl);
    close(server);
    SSL_CTX_free(ctx);
    BIO_printf(outbio, "Finished SSL/TLS connection with server: %s.\n", dest_url);
    __android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "END");
    jstring jstrBuf = env->NewStringUTF(cert_value_full);

    return jstrBuf;

}

char *X509_to_PEM(X509 *cert) {

    BIO *bio = NULL;
    char *pem = NULL;

    if (NULL == cert) {
        return NULL;
    }

    bio = BIO_new(BIO_s_mem());
    if (NULL == bio) {
        return NULL;
    }

    if (0 == PEM_write_bio_X509(bio, cert)) {
        BIO_free(bio);
        return NULL;
    }

    pem = (char *) malloc(bio->num_write + 1);
    if (NULL == pem) {
        BIO_free(bio);
        return NULL;
    }

    memset(pem, 0, bio->num_write + 1);
    BIO_read(bio, pem, bio->num_write);
    BIO_free(bio);
    return pem;
}

int create_socket(char url_str[], BIO *out) {
    int sockfd;
    char hostname[256] = "";
    char portnum[6] = "443";
    char proto[6] = "";
    char *tmp_ptr = NULL;
    int port;
    struct hostent *host;
    struct sockaddr_in dest_addr;

    /* ---------------------------------------------------------- *
     * Remove the final / from url_str, if there is one           *
     * ---------------------------------------------------------- */
    if (url_str[strlen(url_str)] == '/')
        url_str[strlen(url_str)] = '\0';

    /* ---------------------------------------------------------- *
     * the first : ends the protocol string, i.e. http            *
     * ---------------------------------------------------------- */
    strncpy(proto, url_str, (strchr(url_str, ':') - url_str));

    /* ---------------------------------------------------------- *
     * the hostname starts after the "://" part                   *
     * ---------------------------------------------------------- */
    strncpy(hostname, strstr(url_str, "://") + 3, sizeof(hostname));

    /* ---------------------------------------------------------- *
     * if the hostname contains a colon :, we got a port number   *
     * ---------------------------------------------------------- */
    if (strchr(hostname, ':')) {
        tmp_ptr = strchr(hostname, ':');
        /* the last : starts the port number, if avail, i.e. 8443 */
        strncpy(portnum, tmp_ptr + 1, sizeof(portnum));
        *tmp_ptr = '\0';
    }

    port = atoi(portnum);

    if ((host = gethostbyname(hostname)) == NULL) {
        BIO_printf(out, "Error: Cannot resolve hostname %s.\n", hostname);
        //abort();
        return -1;
    }

    /* ---------------------------------------------------------- *
     * create the basic TCP socket                                *
     * ---------------------------------------------------------- */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(port);
    dest_addr.sin_addr.s_addr = *(long *) (host->h_addr);

    /* ---------------------------------------------------------- *
     * Zeroing the rest of the struct                             *
     * ---------------------------------------------------------- */
    memset(&(dest_addr.sin_zero), '\0', 8);

    tmp_ptr = inet_ntoa(dest_addr.sin_addr);

    /* ---------------------------------------------------------- *
     * Try to make the host connect here                          *
     * ---------------------------------------------------------- */
    if (connect(sockfd, (struct sockaddr *) &dest_addr,
                sizeof(struct sockaddr)) == -1) {
        BIO_printf(out, "Error: Cannot connect to host %s [%s] on port %d.\n",
                   hostname, tmp_ptr, port);
    }

    return sockfd;
}
#ifdef __cplusplus
}
#endif

