
default:	build

clean:
	rm -rf Makefile objs

.PHONY:	default clean

build:
	$(MAKE) -f objs/Makefile

install:
	$(MAKE) -f objs/Makefile install

modules:
	$(MAKE) -f objs/Makefile modules

upgrade:
	/d/work/myGit/lovesomuch/nginx/bin/nginx.exe -t

	kill -USR2 `cat /d/work/myGit/lovesomuch/nginx/bin/logs/nginx.pid`
	sleep 1
	test -f /d/work/myGit/lovesomuch/nginx/bin/logs/nginx.pid.oldbin

	kill -QUIT `cat /d/work/myGit/lovesomuch/nginx/bin/logs/nginx.pid.oldbin`

.PHONY:	build install modules upgrade
