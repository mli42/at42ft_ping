# ft_ping

## Prerequisites

- [Vagrant](https://www.vagrantup.com/)
- [Virtual Box](https://www.virtualbox.org/)

## Usage

### Virtual Machine

```sh
# Start the Virtual Machine
make vm.up
# Connect through SSH
make vm.ssh
# Open wireshark with X11 forwarding
wireshark
```

You can log in as `vagrant:vagrant`

### ft_ping

```sh
make
sudo ./ft_ping [OPTIONS...] HOST
```

```
Options:
  -v, --verbose    print verbose messages
  -?, --help       display this help and exit
      --ttl=NUM    set time-to-live for the cache (<= 255)
```

## Sources

- [How to: Calculate ICMP Packet checksum](https://www.ietf.org/rfc/rfc1071.txt) (page 6)
