# windows vscode 调试nginx

- 下载mingw64
    设置全局环境变量
    setx PATH C:\mingw64\bin;%PATH% /m

- 下载 安装 git vscode msys2
https://www.msys2.org/
    设置继承全局变量，以便使用mingw64
    setx MSYS2_PATH_TYPE inherit /m

- 使用msys2命令行编译nginx， 并添加第三方的哈希一致性的负载均衡模块
```shell
./auto/configure --with-cc=gcc --with-cc-opt=-O0 --with-debug --prefix=D:/work/myGit/lovesomuch/nginx/bin --conf-path=conf/nginx.conf --pid-path=logs/nginx.pid --http-log-path=logs/access.log --error-log-path=logs/error.log --sbin-path=nginx.exe --http-client-body-temp-path=temp/client_body_temp --http-proxy-temp-path=temp/proxy_temp --http-fastcgi-temp-path=temp/fastcgi_temp --http-scgi-temp-path=temp/scgi_temp --http-uwsgi-temp-path=temp/uwsgi_temp --without-http_rewrite_module --without-http_gzip_module  --with-select_module --add-module=D:/work/myGit/lovesomuch/nginx/ngx_http_consistent_hash-master
make -j8
make install

不要依赖下面的三方库，否则需要处理复杂的编译关系
--with-http_ssl_module --with-openssl=D:/work/myGit/lovesomuch/nginx/3rd/openssl-1.1.1g --with-openssl-opt=no-asm --with-zlib=D:/work/myGit/lovesomuch/nginx/3rd/zlib-1.2.13 --with-pcre=D:/work/myGit/lovesomuch/nginx/3rd/pcre-8.34

```

- 配置并调试
    根据编译选项，需要手动在nginx.exe的同级目录建立 temp目录，调试的时候才能成功
```conf
upstream www.baidu.com {
        consistent_hash $request_uri;
        server 127.0.0.1:80;
        server 192.168.1.196:80;
    }

    server {
        listen       80;
        server_name  localhost;

        location / {
            #root   html;
            #index  index.html index.htm;
            #配置代理
            proxy_pass http://192.168.1.196;
        }
    }
```