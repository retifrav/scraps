## IKEv2 VPN server with StrongSwan

Original: <https://www.digitalocean.com/community/tutorials/how-to-set-up-an-ikev2-vpn-server-with-strongswan-on-ubuntu-20-04>

<!-- MarkdownTOC -->

- [Introduction](#introduction)
- [Installing StrongSwan](#installing-strongswan)
- [Firewall and kernel IP forwarding](#firewall-and-kernel-ip-forwarding)
- [Certificates](#certificates)

<!-- /MarkdownTOC -->

### Introduction

A virtual private network, or VPN, allows you to securely encrypt traffic as it travels through untrusted networks, such as those at the coffee shop, a conference, or an airport.

[Internet Key Exchange v2](https://en.wikipedia.org/wiki/Internet_Key_Exchange), or IKEv2, is a protocol that allows for direct IPSec tunneling between the server and client. Here we'll be using [StrongSwan](https://www.strongswan.org/) on a Ubuntu server.

### Installing StrongSwan

First do the [usual ritual](https://github.com/retifrav/scraps/blob/master/_linux/new-linux-server.md) of setting up a new GNU/Linux server.

Then install StrongSwan - an open-source IPSec daemon, which we'll configure as our VPN server. We will also install the public key infrastructure (PKI) component so that we can create a Certificate Authority (CA) to provide credentials for our infrastructure.

Install the software:

``` sh
$ sudo apt install strongswan \
    strongswan-pki \
    libcharon-extra-plugins \
    libcharon-extauth-plugins \
    libstrongswan-extra-plugins
```

The additional `libcharon-extauth-plugins` package is used to ensure that various clients can authenticate to your server using a shared username and passphrase. The `libstrongswan-extra-plugins` package is included so that Strongswan supports elliptic curve cipher suites that use the Curve25519 cryptography suite.

### Firewall and kernel IP forwarding

With the StrongSwan configuration complete, we need to configure the firewall to allow VPN traffic through and forward it.

If you followed the prerequisite initial server setup tutorial, you should have a UFW firewall enabled. If you don't yet have UFW configured, you should start by adding a rule to allow SSH connections through the firewall so your current session doesn't close when you enable UFW:

``` sh
$ sudo ufw allow OpenSSH
```

**Это, блядь, очень важный момент! Не проеби сервер! Обязательно добавь здесь SSH!**

Then enable the firewall by typing:

``` sh
$ sudo ufw enable
```

Then, add a rule to allow UDP traffic to the standard IPSec ports, `500` and `4500`:

``` sh
$ sudo ufw allow 500,4500/udp
```

Next, we will open up one of UFW's configuration files to add a few low-level policies for routing and forwarding IPSec packets. Before we we can do this, though, we need to find which network interface on our server is used for internet access. Find this interface by querying for the device associated with the default route:

``` sh
$ ip route show default
```

Your public interface should follow the word `dev`. For example, this result shows the interface named `eth0`, which is highlighted in the following example:

``` sh
default via your_server_ip dev eth0 proto static
```

When you have your public network interface, open the `/etc/ufw/before.rules` file in your text editor. The rules in this file are added to the firewall before the rest of the usual input and output rules. They are used to configure network address translation (NAT) so that the server can correctly route connections to and from clients and the Internet.

``` sh
$ sudo nano /etc/ufw/before.rules
```

Near the top of the file (*before the `*filter` line*), add the following configuration block. Change each instance of `eth0` in the above configuration to match the interface name you found with ip route. The `*nat` lines create rules so that the firewall can correctly route and manipulate traffic between the VPN clients and the internet. The `*mangle` line adjusts the maximum packet segment size to prevent potential issues with certain VPN clients:

``` conf
*nat
-A POSTROUTING -s 10.10.10.0/24 -o eth0 -m policy --pol ipsec --dir out -j ACCEPT
-A POSTROUTING -s 10.10.10.0/24 -o eth0 -j MASQUERADE
COMMIT

*mangle
-A FORWARD --match policy --pol ipsec --dir in -s 10.10.10.0/24 -o eth0 -p tcp -m tcp --tcp-flags SYN,RST SYN -m tcpmss --mss 1361:1536 -j TCPMSS --set-mss 1360
COMMIT

*filter
:ufw-before-input - [0:0]
:ufw-before-output - [0:0]
:ufw-before-forward - [0:0]
:ufw-not-local - [0:0]
...
```

Next, after the `*filter` and `chain` definition lines, add one more block of configuration:

``` conf
...
*filter
:ufw-before-input - [0:0]
:ufw-before-output - [0:0]
:ufw-before-forward - [0:0]
:ufw-not-local - [0:0]

-A ufw-before-forward --match policy --pol ipsec --dir in --proto esp -s 10.10.10.0/24 -j ACCEPT
-A ufw-before-forward --match policy --pol ipsec --dir out --proto esp -d 10.10.10.0/24 -j ACCEPT
```

These lines tell the firewall to forward ESP (*Encapsulating Security Payload*) traffic so the VPN clients will be able to connect. ESP provides additional security for our VPN packets as they're traversing untrusted networks. Save the file.

Before restarting the firewall, we'll change some network kernel parameters to allow routing from one interface to another. The file that controls these settings is called `/etc/ufw/sysctl.conf`. We'll need to configure a few things in the file including.

First IPv4 packet forwarding needs to be turned on so that traffic can move between the VPN and public facing network interfaces on the server. Next we'll disable Path MTU discovery to prevent packet fragmentation problems. Finally we will not accept ICMP redirects nor send ICMP redirects to prevent man-in-the-middle attacks.

Open UFW's kernel parameters configuration file using nano or your preferred text editor:

``` sh
$ sudo nano /etc/ufw/sysctl.conf
```

Now add the following `net/ipv4/ip_forward=1` setting at the end of the file to enable forwarding packets between interfaces:

``` conf
...
net/ipv4/ip_forward=1
```

Next block sending and receiving ICMP redirect packets by adding the following lines to the end of the file:

``` conf
...
net/ipv4/conf/all/accept_redirects=0
net/ipv4/conf/all/send_redirects=0
```

Finally, turn off Path MTU discovery by adding this line to the end of the file:

``` conf
...
net/ipv4/ip_no_pmtu_disc=1
```

Save the file when you are finished. Now we can enable all of our changes by disabling and re-enabling the firewall, since UFW applies these settings any time that it restarts:

``` sh
$ sudo ufw disable
$ sudo ufw enable
```

### Certificates

There are 2 options:

- [username/password authentication](./username-password.md) - that works on Mac OS and iOS, but fails on Windows (*reasons unknown*);
- [certificate-based authentication](./certificate.md) - this does work on Windows too.
