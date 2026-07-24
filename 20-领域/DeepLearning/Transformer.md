---
title: Transformer
tags:
  - 深度学习
  - Transformer
created: 2026-07-09
updated: 2026-07-09
---
		
## Transformer

### 1. 什么是Transformer

Transformer 是一种基于**注意力机制**的深度学习模型，最初由Google在2017年发表的一篇论文**《Attention is All You Need》**中提出。


### 2. 为什么需要Transformer
Transformer 出现之前，NLP 领域主要依赖 RNN（循环神经网络）系列模型（如 LSTM、GRU），它们按顺序处理文本，存在两个关键缺陷。

1. 处理长文本的梯度消失问题；
2. 无法并行计算；顺序处理每一个词，无法利用 GPU 的并行计算能力，长文本处理超慢；

`Transformer的解决方案`：==完全放弃传统的逐词处理方式（RNN），使用自注意力机制让模型动态计算整语句中词语间的关联强度，从而实现更快的训练（将字词embedding成n*d的矩阵，GPU并行计算）和更强的的理解力（解决梯度消失）==。

注：n*d中，n是句子中单词的个数，d是==表示向量==的维度。

### 3.Transformer整体架构

下图是Transformer用于中英文翻译的整体结构，可以看到**Transformer由Encoder和Decoder两部分组成**，每个Encoder去给每个词语做embedding，得到的结果就是整句话的编码信息矩阵。

![](https://cdn.jsdelivr.net/gh/ZGuangJie/GoPicture@main/AI_Engineering20260617092428481.png)

Transformer 具体的工作流程大体如下：

- **第一步获取单词embedding后的向量X**：X由单词的 Embedding（Embedding就是从原始数据提取出来的Feature） 和**单词位置**的 **Embedding** 相加得到。

    ![](https://cdn.jsdelivr.net/gh/ZGuangJie/GoPicture@main/AI_Engineering20260617103914146.png)

- **第二步获取整句话的编码矩阵：**将得到的单词表示向量矩阵**x** 传入 Encoder 中，经过 6 个 Encoder block 后可以得到句子所有单词的编码信息矩阵 **C**，如下图。单词向量矩阵用 表示， n 是句子中单词个数，d 是表示向量的维度 (论文中 d=512)。每一个 Encoder block 输出的矩阵维度与输入完全一致。

![img](https://cdn.jsdelivr.net/gh/ZGuangJie/GoPicture@main/AI_Engineering20260617105106348.png)

- **第三步依次Decoder**：将 Encoder 输出的编码信息矩阵 **C**传递到 Decoder 中，Decoder 依次会根据当前翻译过的单词 1~ i 翻译下一个单词 i+1，如下图所示。在使用的过程中，翻译到单词 i+1 的时候需要通过 **Mask (掩盖)** 操作遮盖住 i+1 之后的单词。

![img](https://cdn.jsdelivr.net/gh/ZGuangJie/GoPicture@main/AI_Engineering/20260617110248185.png)

上图 Decoder 接收了 Encoder 的编码矩阵 **C**，然后首先输入一个翻译开始符 "<Begin>"，预测第一个单词 "I"；然后输入翻译开始符 "<Begin>" 和单词 "I"，预测单词 "have"，以此类推。这是 Transformer 使用时候的大致流程，接下来是里面各个部分的细节。==Decoder时从<Begin>字符开始，依次根绝前面的字符预测后面的字符，因为编码矩阵包含句子所有的信息，所以在预测时需要Mask后面的所有词语。==



### 4.实现细节

下图是论文中 Transformer 的内部结构图，包含下面这些部分：

- Multi-Head Attention层：由多个self-Attention组成
- Add & Norm 层：
    - **Add**是一种残差连接，通常用于解决多层网络训练的问题，可以让网络只关注当前差异的部分
    - **Norm**指 Layer Normalization，通常用于 RNN 结构，Layer Normalization 会将每一层神经元的输入都转成均值方差都一样的，这样可以加快收敛
- Feed Forward层：Feed Forward 层比较简单，是一个两层的全连接层，第一层的激活函数为 Relu，第二层不使用激活函数

左侧为 Encoder block，右侧为 Decoder block。可以看到左侧 Encoder block 包含一个 Multi-Head Attention，而 右侧 Decoder block 包含两个 Multi-Head Attention (其中有一个用到 Masked)。Multi-Head Attention 上方还包括一个 Add & Norm 层，Add 表示残差连接 ([Residual Connection](https://zhida.zhihu.com/search?content_id=163422979&content_type=Article&match_order=1&q=Residual+Connection&zhida_source=entity)) 用于防止网络退化，Norm 表示 [Layer Normalization](https://zhida.zhihu.com/search?content_id=163422979&content_type=Article&match_order=1&q=Layer+Normalization&zhida_source=entity)，用于对每一层的激活值进行归一化。



![img](https://cdn.jsdelivr.net/gh/ZGuangJie/GoPicture@main/AI_Engineering/20260617112540849.png)

#### 4.1单词Embedding

Transformer 中单词的输入表示 **x**由**单词 Embedding** 和**位置 Embedding** （Positional Encoding）相加得到。其中单词Embedding的方式多种多样。

![img](https://cdn.jsdelivr.net/gh/ZGuangJie/GoPicture@main/AI_Engineering/20260617111004189.png)



#### 4.2位置Embedding

Transformer 中除了单词的 Embedding，还需要使用位置 Embedding 表示单词出现在句子中的位置。**因为 Transformer是使用全局信息，不能利用单词的顺序信息，而这部分信息对于 NLP 来说非常重要。**所以 Transformer 中使用位置 Embedding 保存单词在序列中的相对或绝对位置。

位置 Embedding 用 **PE**表示，**PE** 的维度与单词 Embedding 是一样的。PE 可以通过训练得到，也可以使用某种公式计算得到。在 Transformer 中采用了后者，计算公式如下：

![img](https://cdn.jsdelivr.net/gh/ZGuangJie/GoPicture@main/AI_Engineering/20260617111654751.png)

其中，pos 表示单词在句子中的位置，d 表示 PE的维度 (与词 Embedding 一样)，2i 表示偶数的维度，2i+1 表示奇数维度 (即 2i≤d, 2i+1≤d)。使用这种公式计算 PE 有以下的好处：

- 使 PE 能够适应比训练集里面所有句子更长的句子，假设训练集里面最长的句子是有 20 个单词，突然来了一个长度为 21 的句子，则使用公式计算的方法可以计算出第 21 位的 Embedding。
- 可以让模型容易地计算出相对位置，对于固定长度的间距 k，**PE(pos+k)** 可以用 **PE(pos)** 计算得到。因为 Sin(A+B) = Sin(A)Cos(B) + Cos(A)Sin(B), Cos(A+B) = Cos(A)Cos(B) - Sin(A)Sin(B)。

将单词的词 Embedding 和位置 Embedding 相加，就可以得到单词的表示向量 **x**，**x** 就是 Transformer 的输入。



#### 4.3Self-Attention（自注意力机制）

>输入：
>
>—> 编码矩阵C 或者 上一个Encoder block的输出

因为 **Self-Attention**是 Transformer 的重点，所以我们重点关注 Multi-Head Attention 以及 Self-Attention，首先详细了解一下 Self-Attention 的内部逻辑。

在计算的时候需要用到矩阵**Q(查询),K(键值),V(值)**，而**Q,K,V**正是通过 Self-Attention 的输入进行线性变换得到的。在实际中，头一个 Self-Attention **接收的是输入是单词的表示向量x组成的编码矩阵**X、 后面接**收的是上一个 Encoder block 的输出。**

![img](https://cdn.jsdelivr.net/gh/ZGuangJie/GoPicture@main/AI_Engineering/20260617140032040.png)

#### 4.4 Q、K、V的计算

Self-Attention 的输入用矩阵X进行表示，则可以使用线性变阵矩阵**WQ,WK,WV**计算得到**Q,K,V**。计算如下图所示，**注意 X, Q, K, V 的每一行都表示一个单词。**

![img](https://cdn.jsdelivr.net/gh/ZGuangJie/GoPicture@main/AI_Engineering/20260617145221287.png)

#### 4.5 Self-Attention的输出

>输入：Q、K、V
>
>输出：**融合了全局上下文关系的新矩阵，与原编码矩阵C形状相同**

使用下面的公式计算：

![img](https://cdn.jsdelivr.net/gh/ZGuangJie/GoPicture@main/AI_Engineering/20260617145940441.png)

公式中计算矩阵**Q**和**K**每一行向量的内积，为了防止内积过大，因此除以d~k~的平方根。**Q**乘以**K**的转置后，得到的矩阵行列数都为 n，n 为句子单词数，这个矩阵可以表示单词之间的 attention 强度。下图为**Q**乘以K^T^ ，1234 表示的是句子中的单词。

![img](https://cdn.jsdelivr.net/gh/ZGuangJie/GoPicture@main/AI_Engineering/20260617150549202.png)

得到Q*K^T^之后，使用 Softmax 计算每一个单词对于其他单词的 attention 系数，公式中的 Softmax 是对矩阵的每一行进行 Softmax，即每一行的和都变为 1.

![img](https://cdn.jsdelivr.net/gh/ZGuangJie/GoPicture@main/AI_Engineering/20260617150652616.png)

得到 Softmax 矩阵之后可以和**V**相乘，得到最终的输出**Z**。

![img](https://cdn.jsdelivr.net/gh/ZGuangJie/GoPicture@main/AI_Engineering/20260617152233852.png)

上面的过程其实就是单词i之间的关系权重矩阵：列表示不同单词，使用Softmax做归一化

最终单词 1 的输出**Z~1~**等于所有单词 i 的值 **V~i~** 根据 attention 系数的比例加在一起得到，如下图所示：

![img](https://cdn.jsdelivr.net/gh/ZGuangJie/GoPicture@main/AI_Engineering/20260617155013521.png)

上面解释self-Attention是为了解释下面Multi-Head Attention的计算过程，其实就是通过使用多个低纬度的Wq, Wk, Wv的计算出不同角度的信息联系，然后再拼接到一起。

#### 4.6 Multi-Head Attention

>输入：
>
>​	> 编码矩阵C
>
>输出：
>
>​	> **形状相同、经过复杂的拆分、降维、并行计算、拼接的矩阵（包含多个子空间的信息）**

在上一节中，我们已经知道怎么通过 Self-Attention 计算得到输出矩阵 Z，而 Multi-Head Attention 是由多个 Self-Attention 组合形成的，下图是论文中 Multi-Head Attention 的结构图。（这个图乍一看有点蒙，Multi-Head其实就是使用多个低纬度的self-Attention，将信息映射到不同的子空间中，**从多个子空间（多角度）提取出来的全局上下文关系**）

![img](https://cdn.jsdelivr.net/gh/ZGuangJie/GoPicture@main/AI_Engineering/20260617160240579.png)

从上图可以看到 Multi-Head Attention 包含多个 Self-Attention 层，首先将输入**X**分别传递到 h 个不同的 Self-Attention 中，计算得到 h 个输出矩阵**Z**。下图是 h=8 时候的情况，此时会得到 8 个输出矩阵**Z**。

![img](https://cdn.jsdelivr.net/gh/ZGuangJie/GoPicture@main/AI_Engineering/20260617172126989.png)

得到 8 个输出矩阵Z~1~ 到 Z~8~之后，Multi-Head Attention 将它们拼接在一起 **(Concat)**，然后传入一个**Linear**层，得到 Multi-Head Attention 最终的输出**Z**。

![img](https://cdn.jsdelivr.net/gh/ZGuangJie/GoPicture@main/AI_Engineering/20260617172233856.png)

可以看到 Multi-Head Attention 输出的矩阵**Z**与其输入的矩阵**X**的维度是一样的。



### 5. Encoder结构

下图红色部分是 Transformer 的 Encoder block 结构，可以看到是由 Multi-Head Attention, **Add & Norm, Feed Forward, Add & Norm** 组成的。刚刚已经了解了 Multi-Head Attention 的计算过程，现在了解一下 Add & Norm 和 Feed Forward 部分。

![img](https://cdn.jsdelivr.net/gh/ZGuangJie/GoPicture@main/AI_Engineering/20260618134429176.png)

#### 5.1 Add & Norm

Add & Norm 层由 Add 和 Norm 两部分组成，表示**残差网络和正则化网络**其计算公式如下：

![img](https://cdn.jsdelivr.net/gh/ZGuangJie/GoPicture@main/AI_Engineering/20260618134648088.png)

其中 **X**表示 Multi-Head Attention 或者 Feed Forward 的输入，MultiHeadAttention(**X**) 和 FeedForward(**X**) 表示输出 (输出与输入 **X** 维度是一样的，所以可以相加)。

**Add**指 **X**+MultiHeadAttention(**X**)，是一种残差连接，通常用于解决多层网络训练的问题，可以让网络只关注当前差异的部分，在 ResNet 中经常用到：

![img](https://cdn.jsdelivr.net/gh/ZGuangJie/GoPicture@main/AI_Engineering/20260618135030624.png)

**Norm**指 Layer Normalization，通常用于 RNN 结构，Layer Normalization 会将每一层神经元的输入都转成均值方差都一样的，这样可以加快收敛。



#### 5.2 Feed Forward

Feed Forward 层比较简单，是一个两层的全连接层，第一层的激活函数为 Relu，第二层不使用激活函数，对应的公式如下。

![img](https://cdn.jsdelivr.net/gh/ZGuangJie/GoPicture@main/AI_Engineering/20260618135428819.png)

**X**是输入，Feed Forward 最终得到的输出矩阵的维度与**X**一致。



#### 5.3 组成 Encoder

>多个Encoder block组成一个Encoder，Encoder的输入输出如下：
>
>输入
>
>​	> 词原信息编码矩阵X
>
>输出
>
>​	> 包含多维子空间信息的编码矩阵C

通过上面描述的 Multi-Head Attention, Feed Forward, Add & Norm 就可以构造出一个 **Encoder block**，**Encoder block** 接收输入矩阵 X~(n*d)~，并输出一个矩阵 O~(n*d~)。通过多个 Encoder block 叠加就可以组成 Encoder。

![img](https://cdn.jsdelivr.net/gh/ZGuangJie/GoPicture@main/AI_Engineering/20260618135801205.png)



### 6. Decoder结构

![img](https://cdn.jsdelivr.net/gh/ZGuangJie/GoPicture@main/AI_Engineering/20260618140224224.png)

上图红色部分为 Transformer 的 Decoder block 结构，与 Encoder block 相似，但是存在一些区别：

- 包含两个 Multi-Head Attention 层。
- 第一个 Multi-Head Attention 层采用了 Masked 操作。
- 第二个 Multi-Head Attention 层的**K, V**矩阵使用 Encoder 的**编码信息矩阵C**进行计算，而**Q**使用上一个 Decoder block 的输出计算。
- 最后有一个 Softmax 层计算下一个翻译单词的概率。



#### 6.1 Masked Multi-Head Attention

>要理解下面的过程，需要深入了解self-Attention每步做的运算，以及Q、K、V的含义：
>
>- Q 代表“我需要什么信息”
>- K 代表“我包含什么信息”
>- V 是每个词携带的**真实内容信息**
>
>Q×K^T^ 算出来的结果，是**“关系矩阵”（打分矩阵）**。它表示了当前句子中，每一个词对其他所有词的“关注度”或“相关性”，而Masked正是要在分配注意力之前，**人为地、强行切断一些连接。让未来词的 V（信息）根本无法流入当前词的表示中。它被彻底屏蔽了**

`在上述理解的基础上，下面的过程就是一个正常的self-Attention加上Masked的计算过程`

Decoder block 的第一个 Multi-Head Attention 采用了 Masked 操作，因为在翻译的过程中是顺序翻译的，即翻译完第 i 个单词，才可以翻译第 i+1 个单词。通过 Masked 操作可以防止第 i 个单词知道 i+1 个单词之后的信息。下面以 "我有一只猫" 翻译成 "I have a cat" 为例，了解一下 Masked 操作。

下面的描述中使用了类似 [Teacher Forcing]() 的概念，在 Decoder 的时候，是需要根据之前的翻译，求解当前最有可能的翻译，如下图所示。首先根据输入 "<Begin>" 预测出第一个单词为 "I"，然后根据输入 "<Begin> I" 预测下一个单词 "have"。

![img](https://cdn.jsdelivr.net/gh/ZGuangJie/GoPicture@main/AI_Engineering/20260618141454441.png)

Decoder 可以在训练的过程中使用 [Teacher Forcing]() 并且并行化训练，即将正确的单词序列 (<Begin> I have a cat) 和对应输出 (I have a cat <end>) 传递到 Decoder。那么在预测第 i 个输出时，就要将第 i+1 之后的单词掩盖住，**注意 Mask 操作是在 Self-Attention 的 Softmax 之前使用的，下面用 0 1 2 3 4 5 分别表示 "<Begin> I have a cat <end>"。**

- **第一步：**是 Decoder 的输入矩阵和 **Mask** 矩阵，输入矩阵包含 "<Begin> I have a cat" (0, 1, 2, 3, 4) 五个单词的表示向量，**Mask** 是一个 5×5 的矩阵。在 **Mask** 可以发现单词 0 只能使用单词 0 的信息，而单词 1 可以使用单词 0, 1 的信息，即只能使用之前的信息。

    ![img](https://cdn.jsdelivr.net/gh/ZGuangJie/GoPicture@main/AI_Engineering/20260618141719436.png)

- **第二步：**接下来的操作和之前的 Self-Attention 一样，通过输入矩阵**X**计算得到**Q,K,V**矩阵。然后计算**Q**和K^T^的乘积**QK^T^**。

    ![img](https://cdn.jsdelivr.net/gh/ZGuangJie/GoPicture@main/AI_Engineering/20260618141922388.png)

- **第三步：**在得到**QK^T^**之后需要进行 Softmax，计算 attention score，我们在 Softmax 之前需要使用**Mask**矩阵遮挡住每一个单词之后的信息，遮挡操作如下：

    ![img](https://cdn.jsdelivr.net/gh/ZGuangJie/GoPicture@main/AI_Engineering/20260618142357631.png)

    ​	得到 **Mask QK^T^**之后在**Mask QK^T^** 上进行 Softmax，每一行的和都为 1。但是单词 0 在单词 1, 2, 3, 4 上的 attention score 都为 0。

- **第四步：**使用 **Mask QK^T^** 与矩阵 **V**相乘，得到输出 **Z**，则单词 1 的输出向量**Z~1~**是只包含单词 1 信息的。

    ![img](https://cdn.jsdelivr.net/gh/ZGuangJie/GoPicture@main/AI_Engineering/20260618142724191.png)

- **第五步：**通过上述步骤就可以得到一个 Mask Self-Attention 的输出矩阵Z~i~，然后和 Encoder 类似，通过 Multi-Head Attention 拼接多个输出**Z~i~**然后计算得到第一个 Multi-Head Attention 的输出**Z**，**Z**与输入**X**维度一样。



#### 6.2  Multi-Head Attention (又叫做Cross-Attention)

![](https://cdn.jsdelivr.net/gh/ZGuangJie/GoPicture@main/AI_Engineering/20260618140224224.png)

> 根据上面的图片中的红框部分，可以清楚地看到，第二个Multi-Head Attention是使用 Decoder block 的输出信息矩阵计算K、V的，而 Q 矩阵是使用 **Encoder Masked后的编码信息矩阵 C**计算的。

这样做得好处如下：

- #### 动态对齐（Alignment）

    - 人类语言的翻译从来不是词对词的顺序翻译。有时需要倒装，有时多个外语词对应一个母语词。
        Cross-Attention 让 Decoder 在生成**每一个**目标词时，都能自由地在整个源句子中“扫视”一遍，**动态地决定现在要把注意力放在源句子的哪个部分**。这就是所谓的“对齐”。

- #### 突破“记忆瓶颈”

    - 在 Transformer 出现之前，以前的架构（如早期的 RNN/LSTM Seq2Seq）要求 Encoder 把整句源文本压缩成一个**固定长度的单一向量**，然后再丢给 Decoder。如果句子很长，信息就会严重丢失（就像让你把一整篇文章压缩成一句话，细节全没了）。Cross-Attention 的做法是：Encoder 根本不压缩，它保留所有词的详细特征 (K、V) 形成一个“数据库”。Decoder 每次需要信息时，自己拿着 Q去数据库里检索。**句子再长，信息也不会丢失。**每一位单词都可以利用到 Encoder 所有单词的信息

- #### 结合多头机制（Multi-Head）威力倍增

    - 既然是多头，这就意味着 Decoder 会派出**好几个“分身”带着不同的** Q **去查 Encoder**：
        - 头 1 的 Q 可能去查：“哪个词在语法上对应我现在的状态？”
        - 头 2 的 Q 可能去查：“哪个词在指代关系上和我有联系？”
        - 头 3 的 Q 可能去查：“时态信息在源句子的哪里？”

	所有头查回来的信息经过 Concat 拼在一起，Decoder 就获得了极其丰满的、全方位的跨语言参考信息。



#### 6.3 Softmax 预测输出单词

Decoder block 最后的部分是利用 Softmax 预测下一个单词，在之前的网络层我们可以得到一个最终的输出 Z，因为 Mask 的存在，使得单词 0 的输出 Z~0~ 只包含单词 0 的信息，如下：

![img](https://cdn.jsdelivr.net/gh/ZGuangJie/GoPicture@main/AI_Engineering/20260618162245638.png)

Softmax 根据输出矩阵的每一行预测下一个单词：

![img](https://cdn.jsdelivr.net/gh/ZGuangJie/GoPicture@main/AI_Engineering/20260618162335381.png)

这就是 Decoder block 的定义，与 Encoder 一样，Decoder 是由多个 Decoder block 组合而成。



### 7. Transformer 总结

- Transformer 与 RNN 不同，可以比较好地并行训练。
- Transformer 本身是不能利用单词的顺序信息的，因此需要在输入中添加位置 Embedding，否则 Transformer 就是一个词袋模型了。
- Transformer 的重点是 Self-Attention 结构，其中用到的 **Q, K, V**矩阵通过输出进行线性变换得到。
- Transformer 中 Multi-Head Attention 中有多个 Self-Attention，可以捕获单词之间多种维度上的相关系数 attention score。





### 核心比喻：人类翻译官的工作模式

抛开复杂的数学，我们先用一个**极其生动的比喻**来揭开它的面纱。想象你是一个翻译官，正在把一句英文（源语言）翻译成中文（目标语言）。

1.  **Encoder（左边的模块）**：相当于你**反复阅读那句英文源文本**，彻底理解了它的意思、语法结构和每一个单词的上下文。这句英文现在深深印在你的脑子里。
2.  **Decoder 的第一层（Masked Attention）**：相当于你**看着自己目前已经写在纸上的中文翻译**。因为你不能偷看未来，所以你只能根据已经写下的中文，想想下一步大概该写什么。
3.  **Decoder 的第二层（Cross-Attention，也就是你的问题）**：这是最关键的一步——**你抬起头，带着你现在的“疑问”，去原版的英文句子里寻找线索！**

在这个比喻中，Q、K、V 的角色分配堪称绝妙：

*   **Q (Query / 查询) —— 来自 Decoder**：
    这是你目前写下半句中文后，脑子里产生的**“疑问”**。
    *例如：我已经翻译了“我”，下一个词应该是动词了，原句里对应的动词是什么？*
*   **K (Key / 键) —— 来自 Encoder**：
    这是英文原句中每个单词的**“标签”或“特征说明书”**。
    *例如：英文词 "love" 的 K 标签上写着：“我是一个动词，表示喜爱，跟在主语 I 后面”。*
*   **V (Value / 值) —— 来自 Encoder**：
    这是英文原句中每个单词的**“实际内容”**。
    *例如：英文词 "love" 的 V 就是它本身包含的语义信息。*

