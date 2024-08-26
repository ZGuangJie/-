### RNN及其变体在时序数据上的预测

​		本次学习目的在于完成小论文《会遇态势下的船舶轨迹预测》，也是问了完成硕士大论文。

#### 1、循环神经网络 RNN

​		

#### 2、LSTM 长短期记忆网络

​		设计 **LSTM** 的目的主要是为了避免传统 **RNN** 网络的 `长时期依赖 （long-term dependency）`的问题。它们的本质就是通过引入 `记忆门` 能够记住很长时期内的信息。

![LSTM数据流向图](https://cdn.jsdelivr.net/gh/ZGuangJie/GoPicture/golang/202406081235744.png)

##### 2.1 遗忘门

​	首先是 LSTM 要决定让那些信息继续通过这个 cell，这是通过一个叫做`forget gate layer`的`sigmoid 神经层`来实现的。它的输入是$ h_{t-1}$ 和 $x_t $，输出是一个数值都在 0，1 之间的向量（向量长度和 cell 的状态 $ C_{t-1} $ 一样），表示让 $C_{t-1} $ 的各部分信息通过的比重。 0 表示“不让任何信息通过”， 1 表示“让所有信息通过”。

​		<img src="https://cdn.jsdelivr.net/gh/ZGuangJie/GoPicture/golang/202406081237128.png" alt="遗忘门" style="zoom:50%;" />

##### 2.2 输入门

​		下一步是决定让多少新的信息加入到 cell 状态 中来。实现这个需要包括两个 步骤：首先，一个叫做`input gate layer `的 `sigmoid `层决定哪些信息需要更新；一个 tanh 层生成一个向量，也就是备选的用来更新的内容$~ \tilde{C_t}$。在下一步，我们把这两部分联合起来，对 cell 的状态进行一个更新。

<img src="https://cdn.jsdelivr.net/gh/ZGuangJie/GoPicture/golang/202406081240433.png" alt="输入门" style="zoom:50%;" />

<img src="https://cdn.jsdelivr.net/gh/ZGuangJie/GoPicture/golang/202406081242205.png" alt="综合输入" style="zoom:50%;" />

##### 2.3 输出门

​		最后，我们需要来决定输出什么值了。这个输出主要是依赖于 cell 的状态$ C_t$，但是又不仅仅依赖于 $C_t$ ， 而 是 需 要 经 过 一 个 过 滤 的 处 理 。 首 先 ， 我 们 还 是 使 用 一 个 sigmoid 层 来 （ 计 算 出 ） 决 定 ，而是需要经过一个过滤的处理。首先，我们还是使用一个 sigmoid 层来（计算出）决定，而是需要经过一个过滤的处理。首先，我们还是使用一个*s**i**g**m**o**i**d*层来（计算出）决定 C_t 中 的 哪 部 分 信 息 会 被 输 出 。 接 着 ， 我 们 把 中的哪部分信息会被输出。接着，我们把中的哪部分信息会被输出。接着，我们把$ C_t $通过一个 tanh 层（把数值都归到 -1 和 1 之间），然后把 tanh 层的输出和 sigmoid 层计算出来的权重相乘，这样就得到了最后输出的结果。

<img src="https://cdn.jsdelivr.net/gh/ZGuangJie/GoPicture/golang/202406081244443.png" style="zoom:50%;" />



总结：`LSTM就是通过一些 门 来选择性的遗忘一部分信息，有选择性的记忆一些信息，缓解了经典RNN带来的梯度消失问题。`

```python
# pytorch版
class LSTM(nn.Module):
    def __init__(self, batch_first=True):
        super(LSTM, self).__init__()
        self.input_size = args.input_size
        self.hidden_size = args.hidden_size
        self.output_size = args.output_size
        self.dropout = args.dropout
        self.num_layers = args.num_layers
        # 开始线性层提取数据input：特征纬度->隐藏层 (self.input_size,  self.hidden_size)
        self.lstm =  nn.LSTM(self.input_size, 
                             self.hidden_size, 
                             num_layers=self.num_layers, 
                             batch_first=batch_first,
                             )
        self.fc = nn.Linear(self.hidden_size, self.output_size*10)  
    def forward(self, x):
        # 初始化状态
        h0 = torch.zeros(self.num_layers, x.size(0), self.hidden_size).to(x.device)
        c0 = torch.zeros(self.num_layers, x.size(0), self.hidden_size).to(x.device)
        # 其实这里不太明白，输出的output是怎么算出来的，上面六个公式里最后的ht吗？
        output, (h_0, _) = self.lstm(x, (h0, c0))
        output = self.fc(output[:, -1, :])
        return output.view(-1, 10, 2)
    
"""
	在李沐的书里似乎找到一些答案,好像是最后的ht乘以一个自学习矩阵
	for input in inputs:
        for X in input:
            I = torch.sigmoid(torch.mm(X, W_xi) + torch.mm(H, W_hi) + b_i)
            F = torch.sigmoid(torch.mm(X, W_xf) + torch.mm(H, W_hf) + b_f)
            O = torch.sigmoid(torch.mm(X, W_xo) + torch.mm(H, W_ho) + b_o)
            C_tilda = torch.tanh(torch.mm(X, W_xc) + torch.mm(H, W_hc) + b_c)
            C = F * C + I * C_tilda
            H = O * C.tanh()
            
            // 上面的六个公式，并不是最终的结果，而是增加了一个全连接层
            Y = torch.mm(H, W_hq) + b_q
            Y = Y.view(-1, args.output_size)
        outputs.append(Y)
"""

```





#### 3、Seq2Seq模型

​		所谓Seq2Seq(Sequence to Sequence)，即**序列到序列**模型，就是一种能够**根据给定的序列，通过特定的生成方法生成另一个序列**的方法，同时这两个序列可以**不等长**。这种结构又叫**Encoder-Decoder模型**，即编码-解码模型，其是RNN的一个变种，为了解决RNN要求序列等长的问题。

![Seq2Seq](https://cdn.jsdelivr.net/gh/ZGuangJie/GoPicture/golang/202406111743923.png)

#### 3.1 Seq2Seq结构详解

​		Seq2Seq最基础的结构由三部分组成，**`编码器`，`语义向量C`和`解码器`**，C是连接二者的。**编码器**通过学习，**将输入序列编码成一个固定大小的`状态向量C`**，作为**解码器**的输入，**解码器RNN**通过对C的学习进行输出。常用RNN的LSTM和GRU结构。

![](https://cdn.jsdelivr.net/gh/ZGuangJie/GoPicture/golang/202406121028064.png)

