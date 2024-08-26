## 1、rpc解决了什么问题，与消息队列应用场景比较

**rpc就是解决本地在调远程方法时，消除交互时的网络不稳定的问题。** 解决的是同步处理，

请求回应

socket网络问题

rpc消除端到端的交互问题

## 2、消息队列应用场景

​	解决任务中不紧迫、非必要的流程扔到消息队列中，异步解决问题。

消息持久化

确保消息一定被消费

## 3、grpc组成

http 2.0 + protobuffer。采用http2.0的原因是有丰富的安全连接机制。

```http	
http 1.0 提供长连接，请求回应模式
http 1.1 pipeline 可以发送多个请求得到多个回应
http 2.0 stream(随时发送请求，随时回应请求。乱序发送，乱序回应)
```



gRPC是一个高性能、开源和通用的RPC框架，面向移动和TTP/2设计。目前提供C、Jva和Go语言版本，分别是：grpc,grpc-java,grpc-go.其中C版本支持C,C+,Node.js,.Python,Ruby,Objective-C,PHP和
C#支持gRPC基于HTTP/2标准设计，带来诸如双向流、流控、头部压缩、单TCP连接上的多复用请求等特性。这些特性使得其在移动设备上表现更好，更省电和节省空间占用。

在gRPC里客户端应用可以像调用本地对象一样直接调用另一台不同的机器上服务端应用的方法，使得您能够更容易地创建分布式应用和服务。与许多RPC系统类似，gRPC也是基于以下理念：定义一个服务，指定



## 4、grpc中service的API

1. unary api 一元普通模式
2. client stream api 客户端流模式
3. server stream api 服务端流模式（一个请求，多个回应）
4. bidirectional stream api 双端流

<img src="E:\技术使用总结笔记\grpc\img\服务模式.png" style="zoom:50%;" />





## 使用教程

1. ​	在.proto文件中定义请求参数、返回参数和servi中rpc的方法。
2. 生成.pb.go模板文件，实现的所有功能都在_grpc.pb.go文件中，直接搜相关功能的写法。
3. 在服务端实现具体的方法，绑定到grpc



## grpc关键特性

1. 超时
2. 重试
3. 拦截器
4. 命名解析
5. 负载均衡
6. 安全连接