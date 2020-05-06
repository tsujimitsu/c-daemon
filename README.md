# c-daemon

## コンパイル・実行

```shell
# gcc -o daemon daemon.c
# ./daemon
# ps -ef | grep daemon 
```

## 説明

### daemon 関数

```c
#include <unistd.h>
int daemon(int nochdir, int noclose);
```

daemon(0, 0)の場合、現在の作業ディレクトリを「/」に変更し標準入力・標準出力・標準エラーを「/dev/null」にリダイレクト。
daemonの中ではforkが実行され、成功すると親プロセスではexitが実行される。成功した場合「0」が返却される。
POSIX準拠ではないのでLinux向けの時に使う。

## [systemd](https://qiita.com/a_yasui/items/f2d8b57aa616e523ede4)

- [SYSTEMD のユニットファイルの作成および変更](https://access.redhat.com/documentation/ja-jp/red_hat_enterprise_linux/7/html/system_administrators_guide/sect-managing_services_with_systemd-unit_files)

fork して終了するタイプのプログラムの場合。

```text
# cat /etc/systemd/system/daemon.service 
[Unit]
Description=Super simple daemon

[Service]
Type=forking
ExecStart=/opt/c-daemon/daemon
Restart = always

[Install]
WantedBy=multi-user.target
```

自身でループする（フォアグラウンドで実行が継続される）タイプのプログラムの場合。

```text
# cat /etc/systemd/system/daemon.service 
[Unit]
Description=Super simple daemon

[Service]
Type=simple
ExecStart=/opt/c-daemon/daemon
Restart = always

[Install]
WantedBy=multi-user.target
```

## プロセス

```shell
# systemctl status daemon
● daemon.service - Super simple daemon
   Loaded: loaded (/etc/systemd/system/daemon.service; enabled; vendor preset: disabled)
   Active: active (running) since 水 2020-05-06 20:25:12 JST; 21s ago
  Process: 15237 ExecStart=/opt/c-daemon/daemon (code=exited, status=0/SUCCESS)
 Main PID: 15238 (daemon)
   CGroup: /system.slice/daemon.service
           └─15238 /opt/c-daemon/daemon

# cd /proc/15238
# cat cmdline 
/opt/c-daemon/daemon
# ls -l exe 
lrwxrwxrwx. 1 root root 0  5月  6 20:25 exe -> /opt/c-daemon/daemon
# cat environ 
LANG=ja_JP.UTF-8PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin
```

## プロセスファイル（pid file)

```shell
# cat /var/run/daemon.pid 
18677
```

## 参考

- [ラズパイで自動起動するデーモンを自作する](https://takamints.hatenablog.jp/entry/2015/06/07/211150)
- [Linuxでデーモンプロセスを作ってみた](https://qiita.com/ryuichi1208/items/dce61fe0b98a0e0c41a7)
- [Systemdを使ってさくっと自作コマンドをサービス化してみる](https://qiita.com/DQNEO/items/0b5d0bc5d3cf407cb7ff)
- [jirihnidek/daemon](https://github.com/jirihnidek/daemon)