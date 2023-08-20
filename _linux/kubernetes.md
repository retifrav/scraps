## Kubernetes

<!-- MarkdownTOC -->

- [Installing](#installing)
    - [kubectl](#kubectl)
    - [minikube](#minikube)
- [Cluster](#cluster)
- [Dashboard](#dashboard)
    - [Access via kubectl proxy](#access-via-kubectl-proxy)
    - [Access via SSH tunnel](#access-via-ssh-tunnel)

<!-- /MarkdownTOC -->

### Installing

```
$ lsb_release -a
Description:    Ubuntu 22.04.3 LTS
Codename:    jammy
```

#### kubectl

<https://kubernetes.io/docs/tasks/tools/install-kubectl-linux/>

``` sh
$ curl -LO "https://dl.k8s.io/release/$(curl -L -s https://dl.k8s.io/release/stable.txt)/bin/linux/amd64/kubectl"

$ curl -LO "https://dl.k8s.io/$(curl -L -s https://dl.k8s.io/release/stable.txt)/bin/linux/amd64/kubectl.sha256"
$ echo "$(cat kubectl.sha256) kubectl" | sha256sum --check
kubectl: OK

$ sudo install ./kubectl /usr/local/bin/kubectl
$ rm ./kubectl*
$ kubectl version --client
Client Version: v1.28.0
Kustomize Version: v5.0.4-0.20230601165947-6ce0bf390ce3
```

You can check if it is configured correctly, but it will likely report problems:

``` sh
$ kubectl cluster-info
E0819 14:52:32.958888    2799 memcache.go:265] couldn't get current server API group list: Get "http://localhost:8080/api?timeout=32s": dial tcp 127.0.0.1:8080: connect: connection refused

To further debug and diagnose cluster problems, use 'kubectl cluster-info dump'.
The connection to the server localhost:8080 was refused - did you specify the right host or port?
```

Because first you need to run a cluster, such as with `minikube`.

#### minikube

First install [minikube](https://minikube.sigs.k8s.io/docs/start/) itself:

``` sh
$ curl -LO https://storage.googleapis.com/minikube/releases/latest/minikube-linux-amd64

$ sudo install ./minikube-linux-amd64 /usr/local/bin/minikube
$ rm ./minikube*
$ minikube version
minikube version: v1.31.2
commit: fd7ecd9c4599bef9f04c0986c4a0187f98a4396e
```

And then install a [driver](https://minikube.sigs.k8s.io/docs/drivers/), for instance [Docker](https://minikube.sigs.k8s.io/docs/drivers/docker/). Instructions for [Ubuntu](https://docs.docker.com/engine/install/ubuntu/):

``` sh
$ for pkg in docker.io docker-doc docker-compose podman-docker containerd runc; do sudo apt remove $pkg; done
$ sudo apt install ca-certificates curl gnupg

$ sudo install -d /etc/apt/keyrings
$ curl -fsSL https://download.docker.com/linux/ubuntu/gpg | sudo gpg --dearmor -o /etc/apt/keyrings/docker.gpg
$ echo \
  "deb [arch="$(dpkg --print-architecture)" signed-by=/etc/apt/keyrings/docker.gpg] https://download.docker.com/linux/ubuntu \
  "$(. /etc/os-release && echo "$VERSION_CODENAME")" stable" | \
  sudo tee /etc/apt/sources.list.d/docker.list > /dev/null

$ sudo apt update
$ sudo apt install docker-ce docker-ce-cli containerd.io docker-buildx-plugin docker-compose-plugin

$ sudo docker run hello-world
Unable to find image 'hello-world:latest' locally
latest: Pulling from library/hello-world
719385e32844: Pull complete
Digest: sha256:dcba6daec718f547568c562956fa47e1b03673dd010fe6ee58ca806767031d1c
Status: Downloaded newer image for hello-world:latest

Hello from Docker!
This message shows that your installation appears to be working correctly.
```

Here the demo image is run with `sudo`, we need to allow running Docker commands without `sudo`. [Grant access](https://docs.docker.com/engine/install/linux-postinstall/) to Unix socket:

``` sh
$ sudo groupadd docker
$ sudo usermod -aG docker $USER
```

Logout and login back to refresh the access group. Check that you can run Docker commands without `sudo`:

``` sh
$ docker run hello-world
```

### Cluster

<https://kubernetes.io/docs/tutorials/hello-minikube/>

``` sh
$ minikube start --driver=docker
😄  minikube v1.31.2 on Ubuntu 22.04
✨  Using the docker driver based on user configuration
📌  Using Docker driver with root privileges
👍  Starting control plane node minikube in cluster minikube
🚜  Pulling base image ...
💾  Downloading Kubernetes v1.27.4 preload ...
    > preloaded-images-k8s-v18-v1...:  393.21 MiB / 393.21 MiB  100.00% 36.32 M
    > gcr.io/k8s-minikube/kicbase...:  447.37 MiB / 447.62 MiB  99.94% 42.20 Mi
🔥  Creating docker container (CPUs=2, Memory=2200MB) ...
🐳  Preparing Kubernetes v1.27.4 on Docker 24.0.4 ...
    ▪ Generating certificates and keys ...
    ▪ Booting up control plane ...
    ▪ Configuring RBAC rules ...
🔗  Configuring bridge CNI (Container Networking Interface) ...
    ▪ Using image gcr.io/k8s-minikube/storage-provisioner:v5
🔎  Verifying Kubernetes components...
🌟  Enabled addons: default-storageclass, storage-provisioner
🏄  Done! kubectl is now configured to use "minikube" cluster and "default" namespace by default
```

### Dashboard

``` sh
$ minikube dashboard --url
🔌  Enabling dashboard ...
    ▪ Using image docker.io/kubernetesui/metrics-scraper:v1.0.8
    ▪ Using image docker.io/kubernetesui/dashboard:v2.7.0
💡  Some dashboard features require the metrics-server addon. To enable all features please run:

    minikube addons enable metrics-server


🤔  Verifying dashboard health ...
🚀  Launching proxy ...
🤔  Verifying proxy health ...
http://127.0.0.1:33605/api/v1/namespaces/kubernetes-dashboard/services/http:kubernetes-dashboard:/proxy/
```

You can specify a port with `--port 8080`, but it might not matter much (*if you intend to access it via `kubectl proxy`*), so random port will be fine.

#### Access via kubectl proxy

Then if you want to access it from a different host, go back to the terminal where you were running `minikube start` and launch a proxy:

``` sh
$ kubectl proxy --port 8080 --address="0.0.0.0"
Starting to serve on [::]:8080
```

Now you can try to open it in your web-browser as <http://SERVER-IP-ADDRESS:8080/api/v1/namespaces/kubernetes-dashboard/services/http:kubernetes-dashboard:/proxy/>, but you should get `403 Forbidden`, so add `--disable-filter=true` and try again. But actually that is a bad idea, so restrict it to your server (*not your host/client*) IP address with `--accept-hosts` instead, so for example:

``` sh
$ kubectl proxy --port 8080 --address="0.0.0.0" --accept-hosts="^123\.321\.123\.321$"
```

So here `123.321.123.321` is that `SERVER-IP-ADDRESS` from the URL.

Don't forget to open/enable that port in your cloud provider / web-hoster network settings for that VM (*and probably also allow it in system's firewall*).

#### Access via SSH tunnel

Alternatively, you can forward the local port [via SSH tunnel](https://github.com/retifrav/scraps/blob/master/_linux/ssh.md#open-a-tunnel-to-some-port). Then you won't need to run `kubectl proxy`, and actually that is better, as you won't be exposing your dashboard to the entire internet that way:

``` sh
$ ssh -N -L 8080:localhost:33605 root@123.321.123.321
```

here:

- `8080` - local port on your computer which you'd like to use;
- `localhost` - your localhost, innit;
- `33605` - the port that dashboard started with when you ran `minikube dashboard --url`;
- SSH connection to the server, better to have it in your `~/.ssh/config`, but if not, then:
    + `root` - your username on the server;
    + `123.321.123.321` - public IP address of the server.

And then you'll be able to open <http://localhost:8080/api/v1/namespaces/kubernetes-dashboard/services/http:kubernetes-dashboard:/proxy/> in your web-browser.
