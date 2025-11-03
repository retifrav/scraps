## NGINX

<!-- MarkdownTOC -->

- [Installation](#installation)
    - [Installation on Windows](#installation-on-windows)
- [Authentication](#authentication)
    - [Client certificate](#client-certificate)
        - [Generating certificates](#generating-certificates)
        - [Using client certificate](#using-client-certificate)
            - [Client certificate on Mac OS](#client-certificate-on-mac-os)
            - [Client certificate on Windows](#client-certificate-on-windows)

<!-- /MarkdownTOC -->

### Installation

#### Installation on Windows

<http://nginx.org/en/docs/windows.html>

Download the [archive](https://nginx.org/en/download.html), unpack it and open that directory (`d:\programs`) in `cmd`.

``` cmd
> cd d:\programs\nginx
> start nginx
```

Check if it's running:

``` cmd
> tasklist /fi "imagename eq nginx.exe"
```

Config is in `d:\programs\conf\nginx.conf`, default webroot is in `d:\programs\html`.

To reload the config:

``` cmd
> nginx -s reload
```

To quit NGINX gracefully (takes some time):

``` cmd
> nginx -s quit
```

To shutdown NGINX right away:

``` cmd
> nginx -s stop
```

### Authentication

#### Client certificate

- shorter variant: <https://ssltrust.com.au/help/setup-guides/client-certificate-authentication>
- detailed guide with configs: <https://docs.nginx.com/nginx-instance-manager/system-configuration/secure-traffic/>

##### Generating certificates

Make a folder:

``` sh
$ sudo mkdir -p /var/www/certificates/client/your-company.com && cd $_
```

Certificate files for server (*you might need to use `sudo`, if your current user does not access rights to this folder*):

``` sh
$ openssl genrsa -out ./ca-private.key 4096

$ openssl req -new -x509 -days 365 -key ./ca-private.key -out ./ca-private.crt
Country Name (2 letter code) [AU]:NO
State or Province Name (full name) [Some-State]:Oslo
Locality Name (eg, city) []:Oslo
Organization Name (eg, company) [Internet Widgits Pty Ltd]:Your Company
Organizational Unit Name (eg, section) []:
Common Name (e.g. server FQDN or YOUR name) []:your-company.com
Email Address []:post@your-company.com
```

For client (*this needs to be done for every user, don't re-use the same certificate*):

``` sh
$ openssl genrsa -out user1.key 2048

$ openssl req -new -key ./user1.key -out ./user1.csr
Country Name (2 letter code) [AU]:NO
State or Province Name (full name) [Some-State]:Oslo
Locality Name (eg, city) []:Oslo
Organization Name (eg, company) [Internet Widgits Pty Ltd]:Your Company
Organizational Unit Name (eg, section) []:
Common Name (e.g. server FQDN or YOUR name) []:your-company.com
Email Address []:user1@your-company.com

Please enter the following 'extra' attributes
to be sent with your certificate request
A challenge password []:SOME-PASSWORD
An optional company name []:Your Company

$ echo 'You can increment the set_serial if you need to reissue the certificate'
$ openssl x509 -req -days 365 -in ./user1.csr -CA ./ca-private.crt -CAkey ./ca-private.key -set_serial 01 -out ./user1.crt
Certificate request self-signature ok
subject=C = NO, ST = Oslo, L = Oslo, O = Your Company, CN = your-company.com, emailAddress = user1@your-company.com

$ echo 'This is a certificate bundle for importing into keychains/vaults'
$ openssl pkcs12 -export -out ./user1.pfx -inkey ./user1.key -in ./user1.crt -certfile ./ca-private.crt
```

The `.pfx` extension could probably just as well be `.p12`.

Change the ownership:

``` sh
$ sudo chown -R www-data:www-data /var/www/certificates/client/your-company.com
```

##### Using client certificate

In the NGINX site config:

``` nginx
server {
    listen 443 ssl;
    listen [::]:443 ssl;

    server_name your-company.com;

    # these have nothing to do with client authentication, it's just SSL/TLS (from Let's Encrypt)
    ssl_certificate /var/www/certificates/fullchain.pem;
    ssl_certificate_key /var/www/certificates/privkey.pem;

    # this is the client certificate
    ssl_client_certificate /var/www/certificates/client/your-company.com/ca-private.crt;
    ssl_verify_client on;

    location / {
        root /var/www/html;
        index index.html;
        
        #try_files $uri $uri/ =404;
    }

    error_page 404 /404.html;
    error_page 403 /403.html;
    error_page 401 /401.html;
}

server {
    listen 80;
    listen [::]:80;
    server_name your-company.com;
    return 301 https://$server_name$request_uri;
}
```

Be aware that `ssl_client_certificate` and `ssl_verify_client` cannot be placed inside `location` blocks, so this certificate authentication will be required for all the locations of this server/site config.

###### Client certificate on Mac OS

Download `user1.crt` and `user1.key` from server to the client host and then with cURL:

``` sh
$ curl --cert ./user1.crt --key ./user1.key https://your-company.com/experiments/some.txt
```

For using it in browsers/applications, download `user1.pfx` and add it to Keychain Access. It will add two entries under Certificates, one of which will be with red-circled cross, but there should (*probably*) be no need to trust that one. Then you can try openning the <https://your-company.com/experiments/some.txt> from a web-browser, and it might not show the dialog for choosing a certificate at first (*and also a password for accessing the keychain*), so try restarting the browser.

###### Client certificate on Windows

Trying to use `user1.crt` and `user1.key` with cURL will be failing:

``` sh
$ curl --cert /e/temp/user1.crt --key /e/temp/user1.key https://your-company.com/experiments/some.txt
curl: (58) schannel: Failed to import cert file E:/temp/user1.crt, last error is 0x80092002
```

Trying to use `user1.pfx` instead will first fail with missing password:

``` sh
$ curl --cert e:/temp/user1.pfx https://your-company.com/experiments/some.txt
curl: (58) schannel: Failed to import cert file E:/temp/user1.pfx, password is bad
```

but it will also fail if the password is in fact provided:

``` sh
curl --cert e:/temp/user1.pfx:SOME-PASSWORD https://your-company.com/experiments/some.txt
curl: (35) schannel: AcquireCredentialsHandle failed: SEC_E_NO_CREDENTIALS (0x8009030E) - No credentials are available in the security package
```

Fuck knows what to do with this.

Trying to import `user1.pfx` via `Manage computer sertificates` does not succeed either, even when being done as Administrator.
