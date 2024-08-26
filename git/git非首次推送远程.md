==推送时一定要关闭代理，因为[git](https://so.csdn.net/so/search?q=git&spm=1001.2101.3001.7020)在拉取或者提交项目时，中间会有git的http和https代理，但是我们本地环境本身就有SSL协议了，所以取消git的https代理即可，不行再取消http的代理。==

## 非首次推送到远程仓库

### 1、把文件添加到版本库中，使用命令 git add . 添加到暂存区里面去，不要忘记后面的小数点“.”，意为添加文件夹下的所有文件

```git
git add .
```

### 2、用命令 git commit 提交文件提交到仓库 -m 后面的是提交说明与备注

```git
git commit -m 'first commit'
```

### 3、获取远程库与本地同步合并（如果远程库不为空必须做这一步，否则后面的提交会失败

```git
# 选择分支
git pull  origin main
```

### 4、把本地库的内容推送到远程，使用 git push 命令，把当前分支 master 推送到远程。

```git
# 合并推送
git push -u origin main

# 强制替换推送
git push --force origin main
```

## 5、设置远程仓库

```git remote set-url origin
git remote set-url origin URL
```

## 6、强制推送到remote

```
git push --force origin main
```

