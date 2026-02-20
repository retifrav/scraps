## Conan

Instructions here were written at the times of Conan 1.x and are likely obsolete by now. You should probably refer to a more detailed and somewhat more updated article [here](https://decovar.dev/blog/2022/02/06/cpp-dependencies-with-conan/).

<!-- MarkdownTOC -->

- [Package from existing binaries](#package-from-existing-binaries)
- [Upload the package to Conan server](#upload-the-package-to-conan-server)
    - [Local server](#local-server)
    - [Remote server behind NGINX](#remote-server-behind-nginx)
- [Install the package](#install-the-package)

<!-- /MarkdownTOC -->

### Package from existing binaries

In a CMake-based project, build and install, so resulting artifacts are in `install` folder:

``` sh
$ tree -L 1 ./install/
install/
├── bin
├── cmake
├── include
└── lib
```

Create a `conanfile.py` in the project root:

``` py
from conans import ConanFile, tools


class SomePackageConan(ConanFile):
    name = "SomePackage"
    settings = "os", "compiler", "build_type", "arch"
    description = "Cross-platform visualization engine"
    homepage = "https://your.host/SomePackage/"
    url = "https://gitlab.your.host/your-host/SomePackage/"
    license = "https://your.host/SomePackage/Documentation/SomePackage-SDK-EULA.pdf"
    author = "your host"
    topics = ("visualization")

    def set_version(self):
        git = tools.Git(folder=self.recipe_folder)
        tag = git.get_tag().replace("build-", "").split("_")
        major_minor = 0.0 if tag[0] == "master" else tag[0]
        patch = tag[1]
        self.version = f"{major_minor}.{patch}"

    def package(self):
        self.copy("*")

    def package_info(self):
        self.cpp_info.libs = tools.collect_libs(self)

```

Every version is built on buildbots and tagged like `build-master_58446`.

Create a Conan package from project root:

``` sh
$ conan export-pkg ./conanfile.py -pf=install
```

The resulting package will be saved to `~/.conan/data/SomePackage/`. Apparently, there is no way to change this path, if for example you'd like to output the package to a specific path.

### Upload the package to Conan server

There are several option for private server, the simplest is [conan_server](https://docs.conan.io/en/latest/uploading_packages/running_your_server.html).

Run it once to generate default config.

#### Local server

Edit `~/.conan_server/server.conf`:

``` ini
[server]
ssl_enabled: False
port: 9300
host_name: localhost
...

[write_permissions]
*/*@*/*: *

[read_permissions]
*/*@*/*: *

[users]
demo: demo
```

Write permissions are not set by default, and if you won't set them, you'll get this error trying to upload a package:

``` sh
ERROR: SomePackage/0.0.1: Upload recipe to 'my_local_server' failed: Permission denied for user: 'demo'. [Remote: my_local_server]

ERROR: Errors uploading some packages
```

Output from `conan_server`:

```
$ conan_server
***********************
Using config: C:\Users\someuser\.conan_server\server.conf
Storage: C:\Users\someuser\.conan_server\data
Public URL: http://localhost:9300/v1
PORT: 9300
***********************
Bottle v0.12.19 server starting up (using WSGIRefServer())...
Listening on http://0.0.0.0:9300/
Hit Ctrl-C to quit.

127.0.0.1 - - [24/Sep/2021 11:22:42] "GET /v1/ping HTTP/1.1" 200 0
127.0.0.1 - - [24/Sep/2021 11:22:44] "GET /v1/conans/SomePackage/0.0.1/_/_/digest HTTP/1.1" 404 0
127.0.0.1 - - [24/Sep/2021 11:22:46] "GET /v1/users/check_credentials HTTP/1.1" 401 19
127.0.0.1 - - [24/Sep/2021 11:22:52] "GET /v1/users/authenticate HTTP/1.1" 200 124
127.0.0.1 - - [24/Sep/2021 11:22:54] "GET /v1/users/check_credentials HTTP/1.1" 200 4
127.0.0.1 - - [24/Sep/2021 11:22:56] "GET /v1/conans/SomePackage/0.0.1/_/_ HTTP/1.1" 404 0
127.0.0.1 - - [24/Sep/2021 11:22:58] "POST /v1/conans/SomePackage/0.0.1/_/_/upload_urls HTTP/1.1" 403 17
```

Anyway, start the server, add a remote and upload the package:

``` sh
$ conan_server

$ conan remote add my_local_server http://localhost:9300

$ conan upload SomePackage --all -r=my_local_server
Are you sure you want to upload 'SomePackage/0.0.1' to 'my_local_server'? (yes/no): yes
Uploading to remote 'my_local_server':
Uploading SomePackage/0.0.1 to remote 'my_local_server'
Please log in to "my_local_server" to perform this action. Execute "conan user" command.
Remote 'my_local_server' username: demo
Please enter a password for "demo" account:
Uploaded conanfile.py -> SomePackage/0.0.1 [0.81k]
Uploaded conanmanifest.txt -> SomePackage/0.0.1 [0.06k]
Uploaded conan recipe 'SomePackage/0.0.1' to 'my_local_server': http://localhost:9300
Uploading package 1/1: SOME-ID to 'my_local_server'
Compressing conan_package.tgz completed [507 files]
Uploaded conan_package.tgz -> SomePackage/0.0.1:3fb4 [233251.20k]
Uploaded conaninfo.txt -> SomePackage/0.0.1:3fb4 [0.40k]
Uploaded conanmanifest.txt -> SomePackage/0.0.1:3fb4 [38.32k]
```

It will get "uploaded" to `~/.conan_server/data/`.

#### Remote server behind NGINX

Edit `~/.conan_server/server.conf`:

``` ini
[server]
ssl_enabled: False
port: 9300
host_name: your.host
...

[write_permissions]
*/*@*/*: packages

[read_permissions]
*/*@*/*: *

[users]
# has nothing to do with system user
# also apparently it doesn't like certain symbols in passwords
packages: HERE-GOES-SOME-PASSWORD
```

If you won't set `host_name` to your actual domain, then you'll get these errors trying to upload to this remote:

``` sh
ERROR: HTTPSConnectionPool(host='127.0.0.1', port=9300): Max retries exceeded with url: /v1/files/somePackage/0.0.1/_/_/0/export/conanfile.py?signature=SOME-SIGNATURE-HERE (Caused by NewConnectionError('<urllib3.connection.HTTPSConnection object at 0x000002D43FEBF6D0>: Failed to establish a new connection: [WinError 10061] No connection could be made because the target machine actively refused it'))
```

Add this to NGINX config:

``` nginx
location /files/packages/conan/ {
    # Conan server has its own authentication
    #auth_basic           "restricted area";
    #auth_basic_user_file /etc/nginx/packages.htpasswd;
    proxy_pass http://127.0.0.1:9300/;
    # these might not be needed
    proxy_http_version 1.1;
    proxy_set_header Upgrade $http_upgrade;
    proxy_set_header Connection keep-alive;
    proxy_set_header Host $host;
    proxy_cache_bypass $http_upgrade;
    proxy_set_header X-Forwarded-For $proxy_add_x_forwarded_for;
    proxy_set_header X-Forwarded-Proto $scheme;
}
```

Create a system `packages` user with home in `/home/packages/` and disabled login.

Create a systemd service:

``` ini
[Unit]
Description=Conan server

[Service]
WorkingDirectory=/var/www/conan-server/
ExecStart=/usr/local/bin/conan_server
Restart=always
RestartSec=10
SyslogIdentifier=conan_server
User=packages

[Install]
WantedBy=multi-user.target
```

Add this remote on client and upload the package:

``` sh
$ conan remote add Some https://your.host/files/packages/conan/

$ conan upload SomePackage --all --remote=Some --check --confirm
Uploading to remote 'Some':
Uploading SomePackage/0.0.1 to remote 'Some'
Recipe is up to date, upload skipped
Uploading package 1/1: SOME-ID to 'Some'
Uploaded conan_package.tgz -> SomePackage/0.0.1:3fb4 [233251.20k]
Uploaded conaninfo.txt -> SomePackage/0.0.1:3fb4 [0.40k]
Uploaded conanmanifest.txt -> SomePackage/0.0.1:3fb4 [38.32k]
```

It will get uploaded to `/home/packages/.conan_server/data/` on the server.

You can also check remotes list, your user credentials and packages available on the server:

``` sh
$ conan user
Current user of remote 'conancenter' set to: 'None' (anonymous)
Current user of remote 'my_local_server' set to: 'demo' [Authenticated]
Current user of remote 'Some' set to: 'packages' [Authenticated]

$ conan remote list
conancenter: https://center.conan.io [Verify SSL: True]
my_local_server: http://localhost:9300 [Verify SSL: True]
Some: https://your.host/files/packages/conan/ [Verify SSL: True]

$ conan search --remote=my_local_server
Existing package recipes:

SomePackage/0.0.1
```

### Install the package

On some other machine, where you need to install your package from the remote:

``` sh
$ conan remote add Some https://your.host/files/packages/conan/
$ cd /path/to/somewhere
$ echo -e "[requires]\nmSomePackage/0.0.1" > conanfile.txt
$ conan install . --remote=Some
```
