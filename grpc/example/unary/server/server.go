package main

import (
	"context"
	"example/protos"
	"fmt"
	"net"

	"google.golang.org/grpc"
)

// 在echo_grpc.pb.go找到一个接口，在C++中叫做纯虚函数，用时直接重写

// type EchoServiceServer interface {
// 	// 指定参数类型和返回值类型
// 	GetUnaryEcho(context.Context, *EchoRequest) (*EchoResponse, error)
// mustEmbedUnimplementedEchoServiceServer() //必须嵌入UnimplementedEchoServiceServer
// }

type echoService struct {
	protos.UnimplementedEchoServiceServer
}

func (es *echoService) GetUnaryEcho(ctx context.Context, req *protos.EchoRequest) (*protos.EchoResponse, error) {
	// 获取.proto文件里定义的参数，直接在前面加Get
	request := "received:" + req.GetRep()
	fmt.Println(request)
	return &protos.EchoResponse{Res: request}, nil
}

func main() {
	// 服务端生成grpc客户端
	rpcs := grpc.NewServer()
	// 注册到服务器中,不知道的参数去echo_grpc.pb.go文件里去找
	protos.RegisterEchoServiceServer(rpcs, &echoService{})

	// 实现网络功能
	lis, err := net.Listen("tcp", ":8080")
	if err != nil {
		// 抛出异常
		panic(err)
	}
	defer lis.Close()
	rpcs.Serve(lis)
}
