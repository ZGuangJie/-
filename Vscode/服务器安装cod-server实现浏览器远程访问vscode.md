## 一、下载code-server

[查看官网版本下载自己需要的版本](https://github.com/coder/code-server/releases)

[code-server-3.9.0下载地址](https://github.com/coder/code-server/releases/download/v3.9.0/code-server-3.9.0-linux-amd64.tar.gz)

1、Ubuntu 使用wget下载

```linux
wget https://github.com/coder/code-server/releases/download/v3.19.0/code-server-3.19.0-linux-amd64.tar.gz
# 安装下载包
dpkg -i code-server_3.9.0_amd64.deb
systemctl enable --now code-server@$USER
```

2、使用nginx映射

```linux
# 安装linux
sudo apt-get update
sudo apt-get install nginx
# 默认安装路径/etc/nginx

# 配置Nginx, 新建一个Nginx配置文件, 输入
server {
    listen 80;
    server_name your_domain.com;  # 替换成您的域名或者服务器 IP

    location / {
        proxy_pass http://127.0.0.1:YOUR_INTERNAL_PORT;  # 替换成您想要映射的内部端口
        proxy_set_header Host $host;
        proxy_set_header Upgrade $http_upgrade;
        proxy_set_header Connection upgrade;
        proxy_set_header Accept-Encoding gzip;
    }
}

# 启用Nginx配置：创建符号链接将配置文件链接到“sites-enabled”目录
sudo ln -s /etc/nginx/sites-available/port-mapping /etc/nginx/sites-enabled/


# 重启Nginx
sudo systemctl restart nginx
```

