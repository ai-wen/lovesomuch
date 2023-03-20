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

# 配置反向代理并调试
    根据编译选项，需要手动在nginx.exe的同级目录建立 temp目录，调试的时候才能成功
```conf
    upstream consistent_hash_server {
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
            proxy_pass http://consistent_hash_server;
        }
    }
```

# 负载均衡策略
### 轮询
upstream bck_testing_01 {
  # 默认所有服务器权重为 1
  server 192.168.250.220:8080
  server 192.168.250.221:8080
  server 192.168.250.222:8080
}
### 加权轮询
upstream bck_testing_01 {
  server 192.168.250.220:8080   weight=3
  server 192.168.250.221:8080              # default weight=1
  server 192.168.250.222:8080              # default weight=1
}
### 最少连接
upstream bck_testing_01 {
  least_conn;

  # with default weight for all (weight=1)
  server 192.168.250.220:8080
  server 192.168.250.221:8080
  server 192.168.250.222:8080
}
### 加权最少连接
upstream bck_testing_01 {
  least_conn;

  server 192.168.250.220:8080   weight=3
  server 192.168.250.221:8080              # default weight=1
  server 192.168.250.222:8080              # default weight=1
}
### IP Hash
upstream bck_testing_01 {

  ip_hash;

  # with default weight for all (weight=1)
  server 192.168.250.220:8080
  server 192.168.250.221:8080
  server 192.168.250.222:8080

}
### 普通 Hash
upstream bck_testing_01 {

  hash $request_uri;

  # with default weight for all (weight=1)
  server 192.168.250.220:8080
  server 192.168.250.221:8080
  server 192.168.250.222:8080

}

# 反向代理多个 webapp 的配置
```config
http {
	#此处省略一些基本配置

	upstream product_server{
		server www.helloworld.com:8081;
	}

	upstream admin_server{
		server www.helloworld.com:8082;
	}

	upstream finance_server{
		server www.helloworld.com:8083;
	}

	server {
		#此处省略一些基本配置
        server_name  helloworld.com;
		#默认指向product的server
		location / {
			proxy_pass http://product_server;
		}

		location /product/{
			proxy_pass http://product_server;
		}

		location /admin/ {
			proxy_pass http://admin_server;
		}

		location /finance/ {
			proxy_pass http://finance_server;
		}
	}
}
```


# 静态站点
只需要在 nginx.conf 中指定首页
```config
server {
    listen       80;
    server_name  static.zp.cn;

    location / {
        root /app/dist;
        index index.html;
        #转发任何请求到 index.html
    }
}
```    
# 文件服务器
- 将 autoindex 开启可以显示目录，默认不开启。
- 将 autoindex_exact_size 开启可以显示文件的大小。
- 将 autoindex_localtime 开启可以显示文件的修改时间。
- root 用来设置开放为文件服务的根路径。
- charset 设置为 charset utf-8,gbk;，可以避免中文乱码问题（windows 服务器下设置后，依然乱码，本人暂时没有找到解决方法）
```conf
autoindex on;# 显示目录
autoindex_exact_size on;# 显示文件大小
autoindex_localtime on;# 显示文件时间

server {
    charset      utf-8,gbk; # windows 服务器下设置后，依然乱码，暂时无解
    listen       9050 default_server;
    listen       [::]:9050 default_server;
    server_name  _;
    root         /share/fs;
}
```


# 跨域
各自独立的 web app 在互相访问时，势必存在跨域问题。解决跨域问题一般有两种思路：
- CORS
    在后端服务器设置 HTTP 响应头，把你需要允许访问的域名加入 Access-Control-Allow-Origin 中。
- jsonp
    把后端根据请求，构造 json 数据，并返回，前端用 jsonp 跨域。

```conf
upstream front_server{
  server www.helloworld.com:9000;
}
upstream api_server{
  server www.helloworld.com:8080;
}

server {
  listen       80;
  server_name  www.helloworld.com;

  location ~ ^/api/ {
    include enable-cors.conf;
    proxy_pass http://api_server;
    rewrite "^/api/(.*)$" /$1 break;
  }

  location ~ ^/ {
    proxy_pass http://front_server;
  }
}


# allow origin list
set $ACAO '*';

# set single origin
if ($http_origin ~* (www.helloworld.com)$) {
  set $ACAO $http_origin;
}

if ($cors = "trueget") {
	add_header 'Access-Control-Allow-Origin' "$http_origin";
	add_header 'Access-Control-Allow-Credentials' 'true';
	add_header 'Access-Control-Allow-Methods' 'GET, POST, OPTIONS';
	add_header 'Access-Control-Allow-Headers' 'DNT,X-Mx-ReqToken,Keep-Alive,User-Agent,X-Requested-With,If-Modified-Since,Cache-Control,Content-Type';
}

if ($request_method = 'OPTIONS') {
  set $cors "${cors}options";
}

if ($request_method = 'GET') {
  set $cors "${cors}get";
}

if ($request_method = 'POST') {
  set $cors "${cors}post";
}
```