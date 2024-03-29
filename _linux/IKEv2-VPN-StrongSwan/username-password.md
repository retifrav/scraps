## IKEv2 VPN server with StrongSwan, username/password authentication

Original: <https://www.digitalocean.com/community/tutorials/how-to-set-up-an-ikev2-vpn-server-with-strongswan-on-ubuntu-20-04>

This guide takes first steps from the [base guide](./index.md) and makes a setup
for a username/password authentication (*which doesn't work on Windows, for some reasons*).

<!-- MarkdownTOC -->

- [Certificates](#certificates)
    - [Certificate authority](#certificate-authority)
    - [Certificate for the VPN server](#certificate-for-the-vpn-server)
- [Configuring StrongSwan](#configuring-strongswan)
- [VPN authentication](#vpn-authentication)
- [Setting-up clients](#setting-up-clients)
    - [Certificate](#certificate)
    - [Mac OS](#mac-os)
    - [iOS](#ios)

<!-- /MarkdownTOC -->

### Certificates

#### Certificate authority

An IKEv2 server requires a certificate to identify itself to clients. To help create the required certificate, the `strongswan-pki` package comes with a utility called `pki` to generate a Certificate Authority and server certificates.

To begin, let's create a few directories to store all the assets we'll be working on. The directory structure matches some of the directories in `/etc/ipsec.d`, where we will eventually move all of the items we create:

``` sh
$ mkdir -p ~/pki/{cacerts,certs,private}
```

Then lock down the permissions so that our private files can't be seen by other users:

``` sh
$ chmod 700 ~/pki
```

Now that we have a directory structure to store everything, we can generate a root key. This will be a 4096-bit RSA key that will be used to sign our root certificate authority.

Execute these commands to generate the key (*[more details](https://docs.strongswan.org/docs/5.9/pki/pkiQuickstart.html#_generating_a_ca_certificate)*):

``` sh
$ pki --gen --type rsa --size 4096 --outform pem > ~/pki/private/ca-key.pem
```

Following that we can move on to creating our root certificate authority, using the key that we just generated to sign the root certificate:

``` sh
$ pki --self --ca --lifetime 3650 --type rsa \
    --in ~/pki/private/ca-key.pem \
    --dn "C=RU, O=Declaration of VAR, CN=Declaration of VAR Root CA" \
    --outform pem > ~/pki/cacerts/ca-cert.pem
```

The `--lifetime 3650` flag is used to ensure that the certificate authority's root certificate will be valid for 10 years. The root certificate for an authority does not change typically, since it would have to be redistributed to every server and client that rely on it, so 10 years is a safe default expiry value.

You can change the distinguished name (DN) value to something else if you would like. The common name (CN field) here is just the indicator, so it doesn't have to match anything in your infrastructure.

Now that we've got our root certificate authority up and running, we can create a certificate that the VPN server will use.

#### Certificate for the VPN server

We'll now create a certificate and key for the VPN server. This certificate will allow the client to verify the server's authenticity using the CA certificate we just generated.

First, create a private key for the VPN server with the following command:

``` sh
$ pki --gen --type rsa --size 4096 --outform pem > ~/pki/private/server-key.pem
```

Now, create and sign the VPN server certificate with the certificate authority's key you created in the previous step. Execute the following command, but change the Common Name (CN) and the Subject Alternate Name (SAN) field to your VPN server's DNS name or IP address:

``` sh
$ pki --pub --in ~/pki/private/server-key.pem --type rsa \
    | pki --issue --lifetime 1825 \
    --cacert ~/pki/cacerts/ca-cert.pem \
    --cakey ~/pki/private/ca-key.pem \
    --dn "C=RU, O=Declaration of VAR, CN=some.vpn.your-domain.com" \
    --san some.vpn.your-domain.com \
    --flag serverAuth --flag ikeIntermediate \
    --outform pem > ~/pki/certs/server-cert.pem
```

> If you are using an IP address instead of a DNS name, you will need to specify multiple `--san` entries. The line in the previous command block where you specify the distinguished name (`--dn ...``) will need to be modified with the extra entry like the following excerpted line:
> 
> ``` sh
> --dn "CN=IP address" --san @IP_address --san IP_address \
> ```
> 
> The reason for this extra `--san @IP_address` entry is that some clients will check whether the TLS certificate has both an DNS entry and an IP Address entry for a server when they verify its identity.

The `--flag serverAuth` option is used to indicate that the certificate will be used explicitly for server authentication, before the encrypted tunnel is established. The `--flag ikeIntermediate` option is used to support older macOS clients.

Now that we've generated all of the TLS/SSL files StrongSwan needs, we can move the files into place in the `/etc/ipsec.d` directory by typing:

``` sh
$ sudo cp -r ~/pki/* /etc/ipsec.d/
```

In this step, we've created a certificate pair that will be used to secure communications between the client and the server. We've also signed the certificates with the CA key, so the client will be able to verify the authenticity of the VPN server using the CA certificate. With all of these certificates ready, we'll move on to configuring the software.

### Configuring StrongSwan

StrongSwan has a default configuration file with some examples, but we will have to do most of the configuration ourselves. Let's back up the file for reference before starting from scratch:

``` sh
$ sudo mv /etc/ipsec.conf{,.original}
```

Create and open a new blank configuration file using your preferred text editor. Here, we'll use nano:

``` sh
$ sudo nano /etc/ipsec.conf
```

> As you work through this section to configure the server portion of your VPN, you will encounter settings that refer to `left` and `right` sides of a connection. When working with IPSec VPNs, the `left` side by convention refers to the local system that you are configuring, in this case the server. The `right` side directives in these settings will refer to remote clients, like phones and other computers.
> 
> When you move on to configuring clients later in this tutorial, the client configuration files will refer to themselves using various `left` directives, and the server will be referred to using `right` side terminology.

First, we'll tell StrongSwan to log daemon statuses for debugging and allow duplicate connections. Add these lines to the file:

``` conf
config setup
    charondebug="ike 1, knl 1, cfg 0"
    uniqueids=no
```

Then, we'll create a configuration section for our VPN. We'll also tell StrongSwan to create IKEv2 VPN Tunnels and to automatically load this configuration section when it starts up. Append the following lines to the file:

``` conf
...
conn ikev2-vpn
    auto=add
    compress=no
    type=tunnel
    keyexchange=ikev2
    fragmentation=yes
    forceencaps=yes
```

We'll also configure dead-peer detection to clear any "dangling" connections in case the client unexpectedly disconnects. Add these lines:

``` conf
...
conn ikev2-vpn
    ...
    dpdaction=clear
    dpddelay=300s
    rekey=no
```

Next, we'll configure the server's `left` side IPSec parameters. Each of the following parameters ensures that the server is configured to accept connections from clients and to identify itself correctly. You'll add each of these settings to the `/etc/ipsec.conf` file once you are familiar with what they are and why they are used:

- `left=%any`: the `%any` value ensures that the server will use the network interface where it receives incoming connections for subsequent communication with clients. For example, if you are connecting a client over a private network, the server will use the private IP address where it receives traffic for the rest of the connection
- `leftid=@server_domain_or_IP`: this option controls the name that the server presents to clients. When combined with the next option `leftcert`, the `leftid` option ensures that the server's configured name and the Distinguished Name (DN) that is contained in the public certificate match
- `leftcert=server-cert.pem`: this option is the path to the public certificate for the server that you configured in Step 3. Without it, the server will not be able to authenticate itself with clients, or finish negotiating the IKEv2 set up
- `leftsendcert=always`: the always value ensures that any client that connects to the server will always receive a copy of the server's public certificate as part of the initial connection set up.
- `leftsubnet=0.0.0.0/0`: the last `left` side option that you will add tells clients about the subnets that are reachable behind the server. In this case, `0.0.0.0/0` is used to represent the entire set of IPv4 addresses, meaning that the server will tell clients to send all their traffic over the VPN by default

Now that you are familiar with each of the relevant "left" side options, add them all to the file like this:

``` conf
...
conn ikev2-vpn
    ...
    left=%any
    leftid=@server_domain_or_IP
    leftcert=server-cert.pem
    leftsendcert=always
    leftsubnet=0.0.0.0/0
```

> When configuring the server ID (`leftid`), only include the @ character if your VPN server will be identified by a domain name:
> /etc/ipsec.conf
> 
> ``` conf
> ... leftid=@vpn.example.com
> ...
> ```
> 
> If the server will be identified by its IP address, just put the IP address in:
> 
> ``` conf
> ...
> leftid=your_server_ip
> ...
> ```

Next, we can configure the client's `right` side IPSec parameters. Each of the following parameters tells the server how to accept connections from clients, how clients should authenticate to the server, and the private IP address ranges and DNS servers that clients will use. Add each of these settings to the `/etc/ipsec.conf` file once you are familiar with what they are and why they are used:

- `right=%any`: the `%any` option for the `right` side of the connection instructs the server to accept incoming connections from any remote client
- `rightid=%any`: this option ensures that the server will not reject connections from clients that provide an identity before the encrypted tunnel is established
- `rightauth=eap-mschapv2`: this option configures the authentication method that clients will use to authenticate to the server. eap-mschapv2 is used here for broad compatibility to support clients like Windows, macOS, and Android devices
- `rightsourceip=10.10.10.0/24`: this option instructs the server to assign private IP addresses to clients from the specified `10.10.10.0/24` pool of IPs
- `rightdns=8.8.8.8,8.8.4.4`: these IP addresses are Google's public DNS resolvers. They can be changed to use other public resolvers, the VPN server's resolvers, or any other resolver that clients can reach
- `rightsendcert=never`: this option instructs the server that clients do not need to send a certificate to authenticate themselves.

Now that you are familiar with the required "right" side options for the VPN, add the following lines to `/etc/ipsec.conf`:

``` conf
...
conn ikev2-vpn
    ...
    right=%any
    rightid=%any
    rightauth=eap-mschapv2
    rightsourceip=10.10.10.0/24
    rightdns=8.8.8.8,8.8.4.4
    rightsendcert=never
```

Now we'll tell StrongSwan to ask the client for user credentials when they connect:

``` conf
...
conn ikev2-vpn
    ...
    eap_identity=%identity
```

Finally, add the following lines to support Linux, Windows, macOS, iOS, and Android clients. These lines specify the various key exchange, hashing, authentication, and encryption algorithms (commonly referred to as Cipher Suites) that StrongSwan will allow different clients to use:

``` conf
...
conn ikev2-vpn
    ...
    ike=chacha20poly1305-sha512-curve25519-prfsha512,aes256gcm16-sha384-prfsha384-ecp384,aes256-sha1-modp1024,aes128-sha1-modp1024,3des-sha1-modp1024!
    esp=chacha20poly1305-sha512,aes256gcm16-ecp384,aes256-sha256,aes256-sha1,3des-sha1!
```

Each supported cipher suite is delineated from the others by a comma. For example `chacha20poly1305-sha512-curve25519-prfsha512` is one suite, and `aes256gcm16-sha384-prfsha384-ecp384` is another. The cipher suites that are listed here are selected to ensure the widest range of compatibility across Windows, macOS, iOS, Android, and Linux clients.

The complete configuration file should look like this:

``` conf
config setup
    charondebug="ike 1, knl 1, cfg 0"
    uniqueids=no

conn ikev2-vpn
    auto=add
    compress=no
    type=tunnel
    keyexchange=ikev2
    fragmentation=yes
    forceencaps=yes
    dpdaction=clear
    dpddelay=300s
    rekey=no
    left=%any
    leftid=@server_domain_or_IP
    leftcert=server-cert.pem
    leftsendcert=always
    leftsubnet=0.0.0.0/0
    right=%any
    rightid=%any
    rightauth=eap-mschapv2
    rightsourceip=10.10.10.0/24
    rightdns=8.8.8.8,8.8.4.4
    rightsendcert=never
    eap_identity=%identity
    ike=chacha20poly1305-sha512-curve25519-prfsha512,aes256gcm16-sha384-prfsha384-ecp384,aes256-sha1-modp1024,aes128-sha1-modp1024,3des-sha1-modp1024!
    esp=chacha20poly1305-sha512,aes256gcm16-ecp384,aes256-sha256,aes256-sha1,3des-sha1!
```

### VPN authentication

Our VPN server is now configured to accept client connections, but we don't have any credentials configured yet. We'll need to configure a couple things in a special configuration file called `ipsec.secrets`:

- we need to tell StrongSwan where to find the private key for our server certificate, so the server will be able to authenticate to clients.
- we also need to set up a list of users that will be allowed to connect to the VPN.

Let's open the secrets file for editing:

``` sh
$ sudo nano /etc/ipsec.secrets
```

First, we'll tell StrongSwan where to find our private key and how to parse it:

``` conf
: RSA "server-key.pem"
```

Make sure that the line begins with the `:` character and that there is a space after it so that the entire line reads `: RSA "server-key.pem"`.

Then, we'll define the user credentials. You can make up any username or password combination that you like:

``` conf
YOUR-USERNAME : EAP "YOUR-PASSWORD"
```

Save and close the file. Now that we've finished working with the VPN parameters, we'll restart the VPN service so that our configuration is applied:

``` sh
$ sudo systemctl restart strongswan-starter.service
```

Now that the VPN server has been fully configured with both server options and user credentials, it's time to move on to configuring the most important part: the firewall.

### Setting-up clients

#### Certificate

Now that you have everything set up, it's time to try it out. First, you'll need to copy the CA certificate you created and install it on your client device(s) that will connect to the VPN. The easiest way to do this is to log into your server and output the contents of the certificate file:

``` sh
$ cat /etc/ipsec.d/cacerts/ca-cert.pem
```

You'll see output similar to this:

```
-----BEGIN CERTIFICATE-----
MIIFNDCCAxygAwIBAgIIHCsidG5mXzgwDQYJKoZIhvcNAQEMBQAwODELMAkGA1UE

...

H2YUdz8XNHrJHvMQKWFpi0rlEcMs+MSXPFWE3Q7UbaZJ/h8wpSldSUbQRUlphExJ
dJ4PX+MUJO/vjG1/ie6Kh25xbBAc3qNq8siiJZDwrg6vjEK7eiZ1rA==
-----END CERTIFICATE-----
```

Copy this output to your computer, including the `-----BEGIN CERTIFICATE-----` and `-----END CERTIFICATE-----` lines, and save it to a file with a recognizable name, such as ca-cert.pem. Ensure the file you create has the `.pem` extension.

#### Mac OS

Follow these steps to import the certificate:

1. Double-click the certificate file. Keychain Access will pop up with a dialog that says "Keychain Access is trying to modify the system keychain. Enter your password to allow this."
2. Enter your password, then click on Modify Keychain
3. Double-click the newly imported VPN certificate. This brings up a small properties window where you can specify the trust levels. Set IP Security (IPSec) to Always Trust and you'll be prompted for your password again. This setting saves automatically after entering the password.

Now that the certificate is imported and trusted, configure the VPN connection with these steps:

1. Go to System Preferences and choose Network
2. Click on the small "plus" button on the lower-left of the list of networks
3. In the popup that appears, set Interface to VPN, set the VPN Type to IKEv2, and give the connection a name
4. In the Server and Remote ID field, enter the server's domain name or IP address. Leave the Local ID blank
5. Click on Authentication Settings, select Username, and enter your username and password you configured for your VPN user. Then click OK

#### iOS

To configure the VPN connection on an iOS device, follow these steps:

1. Send the root certificate to the device, either via Files, or e-mail, or some messenger
2. Open the certificate file, then tap Install and enter your passcode. Or go to Settings, Profiles or something and install it there
3. Go to Settings, General, VPN and tap Add VPN Configuration. This will bring up the VPN connection configuration screen
4. Tap on Type and select IKEv2
5. In the Description field, enter a short name for the VPN connection. This could be anything you like
6. In the Server and Remote ID field, enter the server's domain name or IP address. The Local ID field can be left blank
7. Enter your username and password in the Authentication section, then tap Done
