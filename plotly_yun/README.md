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


### Troubleshooting
- Connecting the yun to WiFi has no error handling. It can fail silently while attempting to connect to a network, so double check your encryption and your network name. Also, network names are case-senstive and sometimes the Arduino will default to connecting to the lower-case version of your network.
- If you've connected multiple arduino yun's, you won't be able to connect with `arduino.local`. Instead, connect with the devices IP address
- Use the latest Arduino IDE (1.5.6 -r2)
- Reseting the network ("hold down the wifi button for > 5 seconds") often requires several tries!
- To verify that the python client on the Linino is ready to recieve commands, run `$ telnet localhost 6571`
