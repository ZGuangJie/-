### Transformer

#### 1、Transformer提出的动机

​		Google在2017年发表的一篇论文，名字叫做“Attention is all you need”，提出了Transformer这个模型用来解决 当前**主流的序列模型**都是**基于复杂的循环神经网络**或者是**卷积神经网络**构造而来的Encoder-Decoder模型，并且就算先前最好的序列模型也都是**基于注意力机制下的Encoder-Decoder架构**。但由于传统的Encoder-Decoder架构在建模过程中，**下一个时刻的计算**过程会**依赖于上一个时刻的输出**，这种固有的属性就限制了传统的Encoder-Decoder模型就**不能以并行**的方式进行计算。

##### 1.1 问题解决思路

​		Transfomer架构是只通过**注意力机制**来计算**模型输入与输出**的隐含表示，而这种注意力的名字就是大名鼎鼎的自注意力机制（self-attention）。

![Transformer架构图](https://cdn.jsdelivr.net/gh/ZGuangJie/GoPicture/golang/202406241202472.png)

​		总的来说，所谓的**自注意力机制就是通过某种运算来直接计算得到句子在编码过程中每个位置上的注意力权重**；然后再**以权重和的形式来计算得到整个句子的隐含向量表示**。最终，Transformer架构就是基于这种自注意力机制而构建的Encoder-Decoder模型。





将向量通过模型传递，使每一个向量都能获得比单个单词更丰富、更具体的含义。



#### 2、什么是self-attention

​		所谓的自注意力机制其实就是论文中所指的“Scaled Dot-Product Attention”。原文里说：注意力机制可以**描述为将query和一系列的key-value对** 映射到**某个输出的过程**，而这个输出的向量就是根据**query和key计算得到的权重作用于value**上的权重和。==$\text{Attention}(Q,K,V)=\text{softmax}(\frac{QK^T}{\sqrt{d_k}})V$==

(输出就是权重作用于 value 上的权重和，权重通过 query 和 key 计算)

```
An attention function can be described as mapping a query and a set of key-value 
to an output, where the query, keys, values, and output are all vectors. The output is computed as a weighted sum of the values, where the weight assigned to each value is computed by a compatibility function of the query with the corresponding key.
```

