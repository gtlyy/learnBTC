# 编译官方btc客户端

## Clone
```
git clone https://github.com/bitcoin/bitcoin.git
```

## 安装依赖
```
## 查看 INSTALL.md，里面有fedora的内容。
sudo dnf install qrencode qrencode-devel sqlite-devel libevent-devel libupnp-devel
sudo dnf install qt5-qtbase-devel qt5-linguist miniupnpc-devel libsecp256k1-devel
sudo dnf install libdb-devel libdb-cxx-devel autoconf automake libtool boost boost-devel
```
## 编译
```
./autogen.sh
./configure --with-incompatible-bdb
make
sudo make install
```

## 同步区块（估计要一个月。）

