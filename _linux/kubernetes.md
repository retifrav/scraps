## Kubernetes

<!-- MarkdownTOC -->

- [Installation](#installation)
    - [kubectl](#kubectl)
    - [minikube](#minikube)
- [Cluster](#cluster)
- [Dashboard](#dashboard)
    - [Access via kubectl proxy](#access-via-kubectl-proxy)
    - [Access via SSH tunnel](#access-via-ssh-tunnel)
- [Deployment of a test pod](#deployment-of-a-test-pod)
    - [Accessing the service from internet](#accessing-the-service-from-internet)
        - [Via reverse-proxy](#via-reverse-proxy)
        - [Via SSH tunnel](#via-ssh-tunnel)
    - [Deleting test pod](#deleting-test-pod)
    - [Addons](#addons)
- [Deploying one more pod](#deploying-one-more-pod)

<!-- /MarkdownTOC -->

### Installation

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

Install [minikube](https://minikube.sigs.k8s.io/docs/start/) itself:

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

Now you can try to open it in your web-browser as <http://SERVER-PUBLIC-IP-ADDRESS:8080/api/v1/namespaces/kubernetes-dashboard/services/http:kubernetes-dashboard:/proxy/>, but you should get `403 Forbidden`, so add `--disable-filter=true` and try again. But actually that is a bad idea, so restrict it to your server (*not your machine/client*) IP address with `--accept-hosts` instead, so for example:

``` sh
$ kubectl proxy --port 8080 --address="0.0.0.0" --accept-hosts="^123\.321\.123\.321$"
```

So here `123.321.123.321` is that `SERVER-PUBLIC-IP-ADDRESS` from the URL.

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

### Deployment of a test pod

First an example just to check that it works:

``` sh
$ kubectl create deployment hello-node --image=registry.k8s.io/e2e-test-images/agnhost:2.39 -- /agnhost netexec --http-port=8080
deployment.apps/hello-node created

$ kubectl get deployments
NAME         READY   UP-TO-DATE   AVAILABLE   AGE
hello-node   1/1     1            1           17s

$ kubectl get pods
NAME                          READY   STATUS    RESTARTS   AGE
hello-node-7579565d66-m76r5   1/1     Running   0          38s

$ kubectl get events
LAST SEEN   TYPE     REASON              OBJECT                             MESSAGE
104s        Normal   Scheduled           pod/hello-node-7579565d66-m76r5    Successfully assigned default/hello-node-7579565d66-m76r5 to minikube
104s        Normal   Pulling             pod/hello-node-7579565d66-m76r5    Pulling image "registry.k8s.io/e2e-test-images/agnhost:2.39"
99s         Normal   Pulled              pod/hello-node-7579565d66-m76r5    Successfully pulled image "registry.k8s.io/e2e-test-images/agnhost:2.39" in 4.691587518s (4.691596618s including waiting)
99s         Normal   Created             pod/hello-node-7579565d66-m76r5    Created container agnhost
99s         Normal   Started             pod/hello-node-7579565d66-m76r5    Started container agnhost
104s        Normal   SuccessfulCreate    replicaset/hello-node-7579565d66   Created pod: hello-node-7579565d66-m76r5
104s        Normal   ScalingReplicaSet   deployment/hello-node              Scaled up replica set hello-node-7579565d66 to 1

$ kubectl config view
apiVersion: v1
clusters:
- cluster:
    certificate-authority: /home/USERNAME/.minikube/ca.crt
    extensions:
    - extension:
        last-update: Sat, 19 Aug 2023 15:53:47 UTC
        provider: minikube.sigs.k8s.io
        version: v1.31.2
      name: cluster_info
    server: https://192.168.49.2:8443
  name: minikube
contexts:
- context:
    cluster: minikube
    extensions:
    - extension:
        last-update: Sat, 19 Aug 2023 15:53:47 UTC
        provider: minikube.sigs.k8s.io
        version: v1.31.2
      name: context_info
    namespace: default
    user: minikube
  name: minikube
current-context: minikube
kind: Config
preferences: {}
users:
- name: minikube
  user:
    client-certificate: /home/USERNAME/.minikube/profiles/minikube/client.crt
    client-key: /home/USERNAME/.minikube/profiles/minikube/client.key
```

By default the pod is only accessible by its internal IP address within the cluster, and to make this `hello-node` container accessible from outside the Kubernetes virtual network, the pod needs to be exposed as Kubernetes Service:

``` sh
$ kubectl expose deployment hello-node --type=LoadBalancer --port=8080
service/hello-node exposed
```

Apparently, the `8080` port here must be the same as the port used in `kubectl create deployment hello-node` command.

List of services now:

``` sh
$ kubectl get services
NAME         TYPE           CLUSTER-IP    EXTERNAL-IP   PORT(S)          AGE
hello-node   LoadBalancer   10.97.34.30   <pending>     8080:32535/TCP   2m34s
kubernetes   ClusterIP      10.96.0.1     <none>        443/TCP          17h

$ minikube service list
|----------------------|---------------------------|--------------|---------------------------|
|      NAMESPACE       |           NAME            | TARGET PORT  |            URL            |
|----------------------|---------------------------|--------------|---------------------------|
| default              | hello-node                |         8080 | http://192.168.49.2:32535 |
| default              | kubernetes                | No node port |                           |
| kube-system          | kube-dns                  | No node port |                           |
| kubernetes-dashboard | dashboard-metrics-scraper | No node port |                           |
| kubernetes-dashboard | kubernetes-dashboard      | No node port |                           |
|----------------------|---------------------------|--------------|---------------------------|
```

Start the service:

``` sh
$ minikube service hello-node --url=false
|-----------|------------|-------------|---------------------------|
| NAMESPACE |    NAME    | TARGET PORT |            URL            |
|-----------|------------|-------------|---------------------------|
| default   | hello-node |        8080 | http://192.168.49.2:32535 |
|-----------|------------|-------------|---------------------------|
🎉  Opening service default/hello-node in default browser...
👉  http://192.168.49.2:32535
```

You can now try to open it via local CLI web-browser on the server:

``` sh
$ w3m http://192.168.49.2:32535
NOW: 2023-08-20 10:04:11.550114075 +0000 UTC m=+2014.908737657
```

or via cURL (*also locally on the server*):

``` sh
$ curl -I http://192.168.49.2:32535
HTTP/1.1 200 OK
Date: Sun, 20 Aug 2023 10:05:09 GMT
Content-Length: 62
Content-Type: text/plain; charset=utf-8

$ curl http://192.168.49.2:32535
NOW: 2023-08-20 10:05:16.629933573 +0000 UTC m=+2079.988557255
```

#### Accessing the service from internet

As I understood it, the `minikube` cluster isn't meant for exposing services to the internet, but if you still would like to do that, there are some options.

##### Via reverse-proxy

Using NGINX, for example, add a new location in its config:

``` nginx
server {
    listen       80;
    server_name  localhost;

    location / {
        root  /var/www/some;
        index index.html;
    }

    location /k8s/hello/ {
        proxy_pass http://192.168.49.2:32535;
    }

    # ...
}
```

And then you'll be able to open <http://SERVER-PUBLIC-IP-ADDRESS/k8s/hello/> in your web-browser.

##### Via SSH tunnel

Yet again, if you don't want to expose your service to the entire internet, so you only want to be able to open that service from your machine, then you can still create an [SSH tunnel](#access-via-ssh-tunnel), but this time the setup will be a bit more complex, because now you'd need to open **2** tunnels.

First on the server from the container to server environment:

``` sh
$ ssh -o UserKnownHostsFile=/dev/null -o StrictHostKeyChecking=no -N docker@127.0.0.1 -p 32772 -i /home/USERNAME/.minikube/machines/minikube/id_rsa -L 9876:localhost:32535
```

Where the fuck `32772` came from? Well, I got it from running:

``` sh
$ sudo ss -lntup
Netid       State        Recv-Q        Send-Q               Local Address:Port                Peer Address:Port       Process
udp         UNCONN       0             0                    127.0.0.53%lo:53                       0.0.0.0:*           users:(("systemd-resolve",pid=496,fd=13))
udp         UNCONN       0             0                    10.3.0.4%eth0:68                       0.0.0.0:*           users:(("systemd-network",pid=494,fd=18))
udp         UNCONN       0             0                        127.0.0.1:323                      0.0.0.0:*           users:(("chronyd",pid=669,fd=6))
udp         UNCONN       0             0                            [::1]:323                         [::]:*           users:(("chronyd",pid=669,fd=7))
tcp         LISTEN       0             4096                     127.0.0.1:32768                    0.0.0.0:*           users:(("docker-proxy",pid=1815,fd=4))
tcp         LISTEN       0             4096                     127.0.0.1:32769                    0.0.0.0:*           users:(("docker-proxy",pid=1828,fd=4))
tcp         LISTEN       0             4096                     127.0.0.1:32770                    0.0.0.0:*           users:(("docker-proxy",pid=1841,fd=4))
tcp         LISTEN       0             4096                     127.0.0.1:32771                    0.0.0.0:*           users:(("docker-proxy",pid=1854,fd=4))
tcp         LISTEN       0             4096                     127.0.0.1:32772                    0.0.0.0:*           users:(("docker-proxy",pid=1868,fd=4))
tcp         LISTEN       0             4096                 127.0.0.53%lo:53                       0.0.0.0:*           users:(("systemd-resolve",pid=496,fd=14))
tcp         LISTEN       0             128                        0.0.0.0:22                       0.0.0.0:*           users:(("sshd",pid=683,fd=3))
tcp         LISTEN       0             128                           [::]:22                          [::]:*           users:(("sshd",pid=683,fd=4))
```

and trying all the ports that are used by Docker, out of which `32772` turned out to be SSH.

It can also be found in `~/.minikube/logs/lastStart.txt` file:

```
libmachine: Using SSH client type: native
libmachine: &{{{<nil> 0 [] [] []} docker [0x80f160] 0x812200 <nil>  [] 0s} 127.0.0.1 32772 <nil> <nil>}
```

It might be that the port gets a random value every time, because `~/.minikube/machines/minikube/config.json` has `SSHPort` set to `0` by default, so one might prefer to set it to a concrete expected value.

Anyway, now when you tunneled container's port, you can tunnel it further from server environment to your machine:

``` sh
$ ssh -N -L 8080:localhost:9876 azure-tmp
```

and then you will be able to open <http://localhost:8080> in web-browser on your machine.

#### Deleting test pod

``` sh
$ kubectl delete service hello-node
$ kubectl delete deployment hello-node
```

#### Addons

Enable some addon:

``` sh
$ minikube addons enable metrics-server
💡  metrics-server is an addon maintained by Kubernetes. For any concerns contact minikube on GitHub.
You can view the list of minikube maintainers at: https://github.com/kubernetes/minikube/blob/master/OWNERS
    ▪ Using image registry.k8s.io/metrics-server/metrics-server:v0.6.4
🌟  The 'metrics-server' addon is enabled
```

Check the list of what addons you have:

``` sh
$ kubectl get pod,svc -n kube-system
NAME                                   READY   STATUS    RESTARTS      AGE
pod/coredns-5d78c9869d-2x97c           1/1     Running   1 (23h ago)   24h
pod/etcd-minikube                      1/1     Running   1 (23h ago)   24h
pod/kube-apiserver-minikube            1/1     Running   1 (23h ago)   24h
pod/kube-controller-manager-minikube   1/1     Running   1 (23h ago)   24h
pod/kube-proxy-h7kq2                   1/1     Running   1 (23h ago)   24h
pod/kube-scheduler-minikube            1/1     Running   1 (23h ago)   24h
pod/metrics-server-7746886d4f-8tx9d    0/1     Running   0             30s
pod/storage-provisioner                1/1     Running   2 (23h ago)   24h

NAME                     TYPE        CLUSTER-IP      EXTERNAL-IP   PORT(S)                  AGE
service/kube-dns         ClusterIP   10.96.0.10      <none>        53/UDP,53/TCP,9153/TCP   24h
service/metrics-server   ClusterIP   10.104.136.43   <none>        443/TCP                  30s
```

The [dashboard](#dashboard) should now have some more information, and you should be able to run things like:

``` sh
$ kubectl top pods
error: Metrics API not available

$ kubectl rollout restart deployment hello-node
deployment.apps/hello-node restarted

$ kubectl top pods
error: metrics not available yet

$ kubectl top pods
NAME                          CPU(cores)   MEMORY(bytes)
hello-node-59cc88794c-mzccv   2m           20Mi
```

### Deploying one more pod

<https://kubernetes.io/docs/tutorials/kubernetes-basics/deploy-app/deploy-intro/>

What nodes do we have:

``` sh
$ kubectl get nodes
NAME       STATUS   ROLES           AGE   VERSION
minikube   Ready    control-plane   24h   v1.27.4
```

New deployment, just like it was with a [test pod](#deployment-of-a-test-pod):

``` sh
$ kubectl create deployment kubernetes-bootcamp --image=gcr.io/google-samples/kubernetes-bootcamp:v1
deployment.apps/kubernetes-bootcamp created
```

This time without `/agnhost netexec` and `--http-port` for some reasons.

What do we have now:

``` sh
$ kubectl get deployments
NAME                  READY   UP-TO-DATE   AVAILABLE   AGE
hello-node            1/1     1            1           7h1m
kubernetes-bootcamp   0/1     1            0           3s

$ kubectl get pods -o go-template --template '{{range .items}}{{.metadata.name}}{{"\n"}}{{end}}'
hello-node-59cc88794c-mzccv
kubernetes-bootcamp-855d5cc575-zg97m
```

Using the pods names you can expose the API via proxy, as [before](#dashboard):

``` sh
$ kubectl proxy --port 8080 --address="0.0.0.0" --accept-hosts="^123\.321\.123\.321$"
```

and quire some information about them from your machine:

``` sh
$ curl http://SERVER-PUBLIC-IP-ADDERSS:8080/api/v1/namespaces/default/pods/hello-node-59cc88794c-mzccv/
$ curl http://SERVER-PUBLIC-IP-ADDERSS:8080/api/v1/namespaces/default/pods/kubernetes-bootcamp-855d5cc575-zg97m/
```
