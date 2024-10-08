### Nginx简介

#### 1、Nginx是什么？

​		Nginx是一个**高性能**的HTTP和**反向代理**的web服务器，处理高并发能力十分强大，能够经受上万级别的高负载考验。支持热部署，可以7*24不间断运行，几个月都不需要重新启动。

![](https://cdn.jsdelivr.net/gh/ZGuangJie/GoPicture/golang/202406211031332.png)

#### 2、Nginx的反向代理和正向代理

**正向代理**：在本地搭建一个服务器来帮助我们去访问其他服务器。（**浏览器中配置代理服务器**）

**反向代理**：在服务器集群上，有多个服务器，在服务器端配置一个代理服务器，**由代理服务器去选择目标服务器获取数据后在返回给客户端**，此时反向代理服务器和目标服务器对外就是一个服务器，暴露的是**代理服务器地址**，**隐藏真实服务器的地址**。（其实就是在服务器端配置**代理服务器**）

#### 3、Nginx的负载均衡

​		有时候一个服务器在处理**高并发请求**时压力太大无法承受，所以需要搭建一个服务器集群来分担高并发带来的压力，通过拓展网络设备和服务器的带宽、增加吞吐量、加强网络的处理能力、提高网络的灵活性和可用性。负载均衡（Load Balance）意思就是将客户端请求分摊到多个操作单元上（服务器上）。

##### 3.1 三种 Niginx 负载均衡方式

1. **轮询法（默认）**

    按照**客户端请求**顺序将请求逐一分配到**集群服务器**上，如果有服务器down掉，可以自动剔除。

2. **加权轮询**

    **指定**每台服务器被访问的**概率**，适用于多台服务器之间**性能不均衡**的情况。

3. **ip_hash**

    **ip_hash**解决的问题是，轮询负载均衡时**同一用户多次提交的请求可能不会同一服务器响应**，那别的服务器就拿不到**先前的登录信息的问题**。其实就是使用客户端的**ip地址根据hash算法，自动定位到一台固定的服务器**。每个请求按访问ip的hash结果分配，这样每个访客固定访问一个后端服务器，可以解决session的问题。

#### 4、Nginx的动静分离

​		Nginx的静态处理能力很强，但动态处理能力不足。所以一般使用动静分离技术，也就是使用正则匹配来区分动态和静态资源的请求。静态资源交给Nginx，动态资源交给Apache或其他模块处理。

​	在Nginx的配置中，是通过location配置段配合正则匹配实现静态与动态页面的不同处理方式。

![](https://cdn.jsdelivr.net/gh/ZGuangJie/GoPicture/golang/202406211109536.png)

[参考]([Nginx详解（一文带你搞懂Nginx）-CSDN博客](https://blog.csdn.net/hyfsbxg/article/details/122322125))