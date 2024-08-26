package main

import (
	"bufio"
	"context"
	"example/protos"
	"fmt"
	"os"

	"google.golang.org/grpc"
)

func main() {
	// http2 安全
	client, err := grpc.Dial("127.0.0.1:8080", grpc.WithInsecure())
	if err != nil {
		panic(err)
	}

	defer client.Close()
	c := protos.NewEchoServiceClient(client)
	// 读取控制台输入
	reader := bufio.NewReader(os.Stdin)

	for {
		line, _, err := reader.ReadLine()
		if err != nil {
			panic(err)
		}
		req := protos.EchoRequest{Rep: string(line)}
		// 调用远程方法
		res, err := c.GetUnaryEcho(context.Background(), &req)
		if err != nil {
			panic(err)
		}
		fmt.Println(res.GetRes())
	}
}
