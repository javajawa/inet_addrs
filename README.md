h1. `inet_addrs` Address Print Tool

`inet_addrs` is a simple command line tool to print out the current
IPv4 and IPv6 addresses allocated to the machine.
The output is in the form of:

```
$ inet_addrs
lo       127.0.0.1
eth0     10.0.0.2
eth0     fe80:0000:0000:0000:f03c:91ff:fe55:b018

$ inet_addrs eth0
10.0.02
fe80:0000:0000:0000:f03c:91ff:fe55:b018
```

