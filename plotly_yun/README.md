### In progress!

scp this folder over
```
$ scp -r * root@10.0.1.190:/root/
```

ssh in
```bash
$ ssh root@10.0.1.190
```
SSL
```bash
$ opkg update
$ opkg install python-openssl
```
with outputs:
```bash
root@Arduino:~# opkg update
Downloading http://download.linino.org/dogstick/all-in-one/latest/packages//Packages.gz.
Updated list of available packages in /var/opkg-lists/barrier_breaker.
Downloading http://download.linino.org/dogstick/all-in-one/latest/packages//Packages.sig.
Signature check passed.
root@Arduino:~# opkg install python-openssl
Installing python-openssl (2.7.3-2) to root...
Downloading http://download.linino.org/dogstick/all-in-one/latest/packages//python-openssl_2.7.3-2_ar71xx.ipk.
Configuring python-openssl.

```
