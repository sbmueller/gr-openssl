gr-openssl
=========================

gr-openssl is a gnuradio oot-package providing encryption routines using the OpenSSL crypto library


Implemented Functionality
----------------------------------------------------------------
###Symmetric Encryption
Symmetric encryption uses the same keys for encryption and decryption. These keys have to be known at the
transmitter and receiver side for correct operation. OpenSSL provides various encryption ciphers which can 
be used here, like the well known AES (Advanced Encryption Standard) cipher.

Available ciphers with different key configuratons: **AES, Blowfish, Camellia, CAST, DES, RC4, RC2, SEED**  
(depending on your openssl version/configuration) 

Available modes of operation: **ECB, CBC, OFB, CFB and CTR.**  
For more background info, see wikipedia.
    
The implementation operates with byte PDU (packet data unit) messages or tagged stream blocks from GNU Radio.
For correct decryption its necessary to have the correct initialization vector(IV) at receiver side. The 
encryption blocks generates these (random) IVs and outputs them coupled with the first encrypted data. The 
message blocks therefore puts the IV in the metadata field of the first generated PDU, the tagged stream block as
a tag on the first output sample.  
The encryption can be reseted by inserting an "final" pmt in the metadata field of
 the PDU, respectively as a tag on a sample within a tagged stream packet. Then a new IV is generated.
    
###Hash Functions / Message Digests
The hash function block operates on PDU messages and calculates the desired hash.  
Available hash functions: **MD4, MD5, ripemd160, sha, sha1, sha224, sha256, sha384, sha512, whirlpool**

###Key Generation
The necessary keys for symmetric encryption can be randomly generated or derived from a password. This implementation
uses PBKDF2 (Password-Based Key Derivation Function 2) provided by OpenSSL.


Planned functionality
-------------------------------------------------------------
###authenticated encryption
###asymmetric encryption 
###message signing and verifying






