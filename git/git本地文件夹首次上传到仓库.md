## 本地文件夹首次上传到仓库

### 1、进入项目所在文件夹，鼠标右键 Git Bash Here ，使用命令 git init 将项目所在文件夹初始化为git 可以管理的仓库

```git
git init
```

### 2、把文件添加到版本库中，使用命令 git add . 添加到暂存区里面去，不要忘记后面的小数点“.”，意为添加文件夹下的所有文件

```git
git add .
```

### 3、用命令 git commit 提交文件提交到仓库 -m 后面的是提交说明与备注

```git
git commit -m 'first commit'
```

### 4、创建仓库并设置仓库URL

```git
# 复制的远程代码仓库地址
git remote add origin URL
# 替换URL
git remote set-url origin URL
```

### 5、获取远程库与本地同步合并（如果远程库不为空必须做这一步，否则后面的提交会失败(不必要，可暂时不做)

```git
# master 和 main的区别
git pull --rebase origin master
```

### 6、把本地库的内容推送到远程，使用 git push 命令，把当前分支 master 推送到远程。

```git
git push -u origin main
```



## git介绍

### 1、git原理

Git是目前世界上最先进的分布式版本控制系统。原理如下：

![](E:\技术使用总结笔记\git\img\git原理.png)

### 2、常用命令

