## Transformer

### 1. 什么是Transformer

​	Transformer 是一种基于**注意力机制**的深度学习模型，最初由Google在2017年发表的一篇论文**《Attention is All You Need》**中提出。



### 2. 为什么需要Transformer

​	Transformer 出现之前，NLP 领域主要依赖 RNN（循环神经网络）系列模型（如 LSTM、GRU），它们按顺序处理文本，存在两个关键缺陷。

1. 处理长文本的梯度消失问题；
2. 无法并行计算；顺序处理每一个词，无法利用 GPU 的并行计算能力，长文本处理超慢；

`Transformer的解决方案`：==完全放弃传统的逐词处理方式（RNN），使用自注意力机制让模型动态计算整语句中词语间的关联强度，从而实现更快的训练（将字词embedding成n*d的矩阵，GPU并行计算）和更强的的理解力（解决梯度消失）==。

​	注：n*d中，n是句子中单词的个数，d是==表示向量==的维度。

### 3.Transformer整体架构

​	下图是Transformer用于中英文翻译的整体结构，可以看到**Transformer由Encoder和Decoder两部分组成**，每个Encoder去给每个词语做embedding，得到的结果就是整句话的编码信息矩阵。

![](https://cdn.jsdelivr.net/gh/ZGuangJie/GoPicture@main/AI_Engineering20260617092428481.png)

Transformer 具体的工作流程大体如下：

- **第一步获取单词embedding后的向量X**：X由单词的 Embedding（Embedding就是从原始数据提取出来的Feature） 和**单词位置**的 **Embedding** 相加得到。

    ![](https://cdn.jsdelivr.net/gh/ZGuangJie/GoPicture@main/AI_Engineering20260617103914146.png)

- **第二步获取整句话的编码矩阵：**将得到的单词表示向量矩阵**x** 传入 Encoder 中，经过 6 个 Encoder block 后可以得到句子所有单词的编码信息矩阵 **C**，如下图。单词向量矩阵用 表示， n 是句子中单词个数，d 是表示向量的维度 (论文中 d=512)。每一个 Encoder block 输出的矩阵维度与输入完全一致。

![img](https://cdn.jsdelivr.net/gh/ZGuangJie/GoPicture@main/AI_Engineering20260617105106348.png)

- **第三步依次Decoder**：将 Encoder 输出的编码信息矩阵 **C**传递到 Decoder 中，Decoder 依次会根据当前翻译过的单词 1~ i 翻译下一个单词 i+1，如下图所示。在使用的过程中，翻译到单词 i+1 的时候需要通过 **Mask (掩盖)** 操作遮盖住 i+1 之后的单词。

![img](https://cdn.jsdelivr.net/gh/ZGuangJie/GoPicture@main/AI_Engineering/20260617110248185.png)

​	上图 Decoder 接收了 Encoder 的编码矩阵 **C**，然后首先输入一个翻译开始符 "<Begin>"，预测第一个单词 "I"；然后输入翻译开始符 "<Begin>" 和单词 "I"，预测单词 "have"，以此类推。这是 Transformer 使用时候的大致流程，接下来是里面各个部分的细节。==Decoder时从<Begin>字符开始，依次根绝前面的字符预测后面的字符，因为编码矩阵包含句子所有的信息，所以在预测时需要Mask后面的所有词语。==



### 4.实现细节

​	下图是论文中 Transformer 的内部结构图，包含下面这些部分：

- Multi-Head Attention层：由多个self-Attention组成
- Add & Norm 层：
    - **Add**是一种残差连接，通常用于解决多层网络训练的问题，可以让网络只关注当前差异的部分
    - **Norm**指 Layer Normalization，通常用于 RNN 结构，Layer Normalization 会将每一层神经元的输入都转成均值方差都一样的，这样可以加快收敛
- Feed Forward层：Feed Forward 层比较简单，是一个两层的全连接层，第一层的激活函数为 Relu，第二层不使用激活函数

​	左侧为 Encoder block，右侧为 Decoder block。可以看到左侧 Encoder block 包含一个 Multi-Head Attention，而 右侧 Decoder block 包含两个 Multi-Head Attention (其中有一个用到 Masked)。Multi-Head Attention 上方还包括一个 Add & Norm 层，Add 表示残差连接 ([Residual Connection](https://zhida.zhihu.com/search?content_id=163422979&content_type=Article&match_order=1&q=Residual+Connection&zhida_source=entity)) 用于防止网络退化，Norm 表示 [Layer Normalization](https://zhida.zhihu.com/search?content_id=163422979&content_type=Article&match_order=1&q=Layer+Normalization&zhida_source=entity)，用于对每一层的激活值进行归一化。



![img](https://cdn.jsdelivr.net/gh/ZGuangJie/GoPicture@main/AI_Engineering/20260617112540849.png)

#### 4.1单词Embedding

​	Transformer 中单词的输入表示 **x**由**单词 Embedding** 和**位置 Embedding** （Positional Encoding）相加得到。其中单词Embedding的方式多种多样。

![img](https://cdn.jsdelivr.net/gh/ZGuangJie/GoPicture@main/AI_Engineering/20260617111004189.png)



#### 4.2位置Embedding

​	Transformer 中除了单词的 Embedding，还需要使用位置 Embedding 表示单词出现在句子中的位置。**因为 Transformer是使用全局信息，不能利用单词的顺序信息，而这部分信息对于 NLP 来说非常重要。**所以 Transformer 中使用位置 Embedding 保存单词在序列中的相对或绝对位置。

​	位置 Embedding 用 **PE**表示，**PE** 的维度与单词 Embedding 是一样的。PE 可以通过训练得到，也可以使用某种公式计算得到。在 Transformer 中采用了后者，计算公式如下：

![img](https://cdn.jsdelivr.net/gh/ZGuangJie/GoPicture@main/AI_Engineering/20260617111654751.png)

​	其中，pos 表示单词在句子中的位置，d 表示 PE的维度 (与词 Embedding 一样)，2i 表示偶数的维度，2i+1 表示奇数维度 (即 2i≤d, 2i+1≤d)。使用这种公式计算 PE 有以下的好处：

- 使 PE 能够适应比训练集里面所有句子更长的句子，假设训练集里面最长的句子是有 20 个单词，突然来了一个长度为 21 的句子，则使用公式计算的方法可以计算出第 21 位的 Embedding。
- 可以让模型容易地计算出相对位置，对于固定长度的间距 k，**PE(pos+k)** 可以用 **PE(pos)** 计算得到。因为 Sin(A+B) = Sin(A)Cos(B) + Cos(A)Sin(B), Cos(A+B) = Cos(A)Cos(B) - Sin(A)Sin(B)。

将单词的词 Embedding 和位置 Embedding 相加，就可以得到单词的表示向量 **x**，**x** 就是 Transformer 的输入。



#### 4.3Self-Attention（自注意力机制）

>输入：
>
>—> 编码矩阵C 或者 上一个Encoder block的输出

​	因为 **Self-Attention**是 Transformer 的重点，所以我们重点关注 Multi-Head Attention 以及 Self-Attention，首先详细了解一下 Self-Attention 的内部逻辑。

​	在计算的时候需要用到矩阵**Q(查询),K(键值),V(值)**，而**Q,K,V**正是通过 Self-Attention 的输入进行线性变换得到的。在实际中，头一个 Self-Attention **接收的是输入是单词的表示向量x组成的编码矩阵**X、 后面接**收的是上一个 Encoder block 的输出。**

![img](https://cdn.jsdelivr.net/gh/ZGuangJie/GoPicture@main/AI_Engineering/20260617140032040.png)

#### 4.4 Q、K、V的计算

​	Self-Attention 的输入用矩阵X进行表示，则可以使用线性变阵矩阵**WQ,WK,WV**计算得到**Q,K,V**。计算如下图所示，**注意 X, Q, K, V 的每一行都表示一个单词。**

![img](https://cdn.jsdelivr.net/gh/ZGuangJie/GoPicture@main/AI_Engineering/20260617145221287.png)

#### 4.5 Self-Attention的输出

>输入：Q、K、V
>
>输出：

​	使用下面的公式计算：

![img](https://cdn.jsdelivr.net/gh/ZGuangJie/GoPicture@main/AI_Engineering/20260617145940441.png)

​	公式中计算矩阵**Q**和**K**每一行向量的内积，为了防止内积过大，因此除以d~k~的平方根。**Q**乘以**K**的转置后，得到的矩阵行列数都为 n，n 为句子单词数，这个矩阵可以表示单词之间的 attention 强度。下图为**Q**乘以K^T^ ，1234 表示的是句子中的单词。

![img](https://cdn.jsdelivr.net/gh/ZGuangJie/GoPicture@main/AI_Engineering/20260617150549202.png)

​	得到Q*K^T^之后，使用 Softmax 计算每一个单词对于其他单词的 attention 系数，公式中的 Softmax 是对矩阵的每一行进行 Softmax，即每一行的和都变为 1.

![img](https://cdn.jsdelivr.net/gh/ZGuangJie/GoPicture@main/AI_Engineering/20260617150652616.png)

​	得到 Softmax 矩阵之后可以和**V**相乘，得到最终的输出**Z**。

![img](https://cdn.jsdelivr.net/gh/ZGuangJie/GoPicture@main/AI_Engineering/20260617152233852.png)

​	上面的过程其实就是单词i之间的关系权重矩阵：列表示不同单词，使用Softmax做归一化

​	最终单词 1 的输出**Z~1~**等于所有单词 i 的值 **V~i~** 根据 attention 系数的比例加在一起得到，如下图所示：

![img](https://cdn.jsdelivr.net/gh/ZGuangJie/GoPicture@main/AI_Engineering/20260617155013521.png)

#### 4.6 Multi-Head Attention

​	在上一节中，我们已经知道怎么通过 Self-Attention 计算得到输出矩阵 Z，而 Multi-Head Attention 是由多个 Self-Attention 组合形成的，下图是论文中 Multi-Head Attention 的结构图

![img](https://cdn.jsdelivr.net/gh/ZGuangJie/GoPicture@main/AI_Engineering/20260617160240579.png)

​	从上图可以看到 Multi-Head Attention 包含多个 Self-Attention 层，首先将输入**X**分别传递到 h 个不同的 Self-Attention 中，计算得到 h 个输出矩阵**Z**。下图是 h=8 时候的情况，此时会得到 8 个输出矩阵**Z**。

![img](https://cdn.jsdelivr.net/gh/ZGuangJie/GoPicture@main/AI_Engineering/20260617172126989.png)

​	得到 8 个输出矩阵Z~1~ 到 Z~8~之后，Multi-Head Attention 将它们拼接在一起 **(Concat)**，然后传入一个**Linear**层，得到 Multi-Head Attention 最终的输出**Z**。

![img](https://cdn.jsdelivr.net/gh/ZGuangJie/GoPicture@main/AI_Engineering/20260617172233856.png)

​	可以看到 Multi-Head Attention 输出的矩阵**Z**与其输入的矩阵**X**的维度是一样的。



#### 4.7 Encoder结构

