win+r
shell:startup #用户开机自启动（程序开机自启动只针对当前登录的用户）
shell:common startup #系统开机自启动（程序开机自启动针对所有用户，是系统级别的自启动）

```bat
隐藏bat窗口
if "%1"=="hide" goto work
start mshta vbscript:createobject("wscript.shell").run("""%~0"" hide",0)(window.close)&&exit
:work
```


netstat -ano|findstr "5556"
 TCP    0.0.0.0:5556           0.0.0.0:0              LISTENING       2776
 TCP    192.168.0.22:5556      192.168.0.20:52459     TIME_WAIT       0
 TCP    192.168.0.22:5556      192.168.0.20:56439     ESTABLISHED     2776
 TCP    [::]:5556              [::]:0                 LISTENING       2776

taskkill /pid 2776
