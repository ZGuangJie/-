## 一、Latex介绍

​	Latex是tex的衍生品，是一款强大的文章排版利器，相比word,它有着不可替代的文档**编辑排版**的优势，尤其是在==数学公式编辑==方面。

==总结：==

- LaTeX,是一类用于编辑和排版的软件，用于生成PDF文档。
- LaTeX编辑和排版的核心思想在于，通过section和paragraph等语句，规定了每一句话在文章中所从属的层次，从而极大方便了对各个层次批量处理。
- LaTeX在使用体验方面，最不易被Word替代的有四个方面：方便美观的数学公式编辑、不会乱动的退格对齐、
  非所见即所得因比可以在编辑的时候用退格和换行整理思路但生成PDF出来不影响美观、部分导师和刊物不接
  受Word排版的文章。

## 二、Latex与WinEdit的关系

​		Latex是**编译器**，CTEX是相当于一个IDE（**集成开发环境**），WinEdit相当于是一个基于Latex编程的文档**编辑工具**。三者分别类推为：C++编译器、VS、VScode。







## 使用



### 插入图片

```latex
%begin开始，然后居中，includegraphics指定长款、导入路径，
\begin{figure}[h]
	\centering
	\includegraphics[width=0.8\linewidth]{fig/.png}
	\caption{}
	\label{fig}
\end{figure}

%在caption中可以增加label以方便引用
\label{X}
~\ref{X}

% 插入多张图片
\begin{figure}[h]
    \centering
	
	%下面的[]换成{}代表不要序号
    \subfigure[]{\includegraphics[width=0.4\linewidth]{fig/.png}}
    \subfigure[]{\includegraphics[width=0.4\linewidth]{fig/.png}}

    \caption{}
    \label{fig}
\end{figure}
	
```

### 插入公式

```latex
% 自动编号
\begin{equation}
	
\end{equation}
% 手动编号
\begin{equation}
	latex公式
	\tag{...}
\end{equation}
% 使用\\进行换行，最后一行的\\可写可不写
$$
f(x)=2x+1 \\
=2+1 \\
=3
$$
$$
f(x)=2x+1 \\
=2+1 \\
=3
$$
```

### 有序序列

```latex
\begin{enumerate}
	\item ...
	\item ...
	\item ...
\end{enumerate}
```

### 插入一些特殊字符

```latex
%插入一些数学符号
$\theta$ %θ
$\alpha$ %α
$\beta$ %β

%插入下标
X_{.}
```

### 插入参考文献

```latex
%将参考文件转化成bib格式，保存在reference.bib文件中
@article{sedov2016fuzzy, %相当于参考文献的id
  title={The fuzzy model of ships collision risk rating in a heavy traffic zone},
  author={Sedov, Viktor A and Sedova, Nelly A and Glushkov, Sergey V},
  journal={Vibroengineering Procedia},
  volume={8},
  pages={453--458},
  year={2016},
  publisher={JVE International Ltd.}
}

%导入参考文献样式
\bibliographystyle{cas-model2-names}
% 导入参考文献文件
\bibliography{reference.bib}

%在需要的地方引用，使用上面的id
X \cite{sedov2016fuzzy}
```

使用\usepackage[authoryear]{natbib}包，==~\citep{.}==，参考文献不是用序号排序，**按首字母排序**。

<img src="E:\技术使用总结笔记\latex\img\reference.png" alt="reference" style="zoom:33%;" />  <img src="E:\技术使用总结笔记\latex\img\reference_style.png" alt="style" style="zoom: 50%;" />



### 插入表格

```latex
%占满单元格
\begin{table}[H]
	\caption{Regression analysis of the mediating effect of mental resilience.\label{tab3}}
	\begin{adjustwidth}{-\extralength}{0cm}
		\newcolumntype{C}{>{\centering\arraybackslash}X}
		\begin{tabularx}{\fulllength}{ccccccc}
			
		\end{tabularx}
		\noindent{\footnotesize{* SE: Standard Error (same as below).}}
	\end{adjustwidth}
\end{table}

%部分单元格

```



