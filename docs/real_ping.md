# Real `ping` output

## With ip address

`$ ping 8.8.8.8`

```
PING 8.8.8.8 (8.8.8.8): 56 data bytes
64 bytes from 8.8.8.8: icmp_seq=0 ttl=63 time=6.290 ms
64 bytes from 8.8.8.8: icmp_seq=1 ttl=63 time=8.469 ms
64 bytes from 8.8.8.8: icmp_seq=2 ttl=63 time=5.609 ms
64 bytes from 8.8.8.8: icmp_seq=3 ttl=63 time=6.969 ms
64 bytes from 8.8.8.8: icmp_seq=4 ttl=63 time=8.523 ms
^C--- 8.8.8.8 ping statistics ---
5 packets transmitted, 5 packets received, 0% packet loss
round-trip min/avg/max/stddev = 5.609/7.172/8.523/1.164 ms
```

## With hostname

`$ ping google.fr`

```
PING google.fr (172.217.20.163): 56 data bytes
64 bytes from 172.217.20.163: icmp_seq=0 ttl=63 time=5.425 ms
64 bytes from 172.217.20.163: icmp_seq=1 ttl=63 time=6.102 ms
64 bytes from 172.217.20.163: icmp_seq=2 ttl=63 time=8.457 ms
64 bytes from 172.217.20.163: icmp_seq=3 ttl=63 time=8.579 ms
^C--- google.fr ping statistics ---
4 packets transmitted, 4 packets received, 0% packet loss
round-trip min/avg/max/stddev = 5.425/7.141/8.579/1.399 ms
```

## With verbose and ttl flag

`$ ping 8.8.8.8 -v --ttl 1`

```
PING 8.8.8.8 (8.8.8.8): 56 data bytes, id 0x0578 = 1400
36 bytes from 10.0.2.2: Time to live exceeded
IP Hdr Dump:
 4500 0054 442d 4000 0101 195e 0a00 020f 0808 0808
Vr HL TOS  Len   ID Flg  off TTL Pro  cks      Src      Dst     Data
 4  5  00 0054 442d   2 0000  01  01 195e 10.0.2.15  8.8.8.8
ICMP: type 8, code 0, size 64, id 0x0578, seq 0x0000
36 bytes from 10.0.2.2: Time to live exceeded
IP Hdr Dump:
 4500 0054 446b 4000 0101 1920 0a00 020f 0808 0808
Vr HL TOS  Len   ID Flg  off TTL Pro  cks      Src      Dst     Data
 4  5  00 0054 446b   2 0000  01  01 1920 10.0.2.15  8.8.8.8
ICMP: type 8, code 0, size 64, id 0x0578, seq 0x0001
^C--- 8.8.8.8 ping statistics ---
2 packets transmitted, 0 packets received, 100% packet loss
```
