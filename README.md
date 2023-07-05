# TinySTG
縦スクロールシューティングゲーム TinySTG です。  

https://github.com/kuran-kuran/TinySTG

## ダウンロード
https://github.com/kuran-kuran/TinySTG/releases/tag/1.0.0  

## はじめに
TinyArcade、PyBadge、Windows10、Linux、RaspberryPI OS、RGNano(FunKeyOS)、Waveshare19340 を接続した RaspberryPI Picoで動作します。  
Waveshare19340 を接続した RaspberryPI Pico ではまだ画面サイズの調整がまだだったり音が出なかったりするためテスト版になっています。  

## ビルド方法
### Windows版(CreateDIBSection+DirectSound)をVisualStudio2019でビルド
VisualStudio2019でTinySTG\VisualStudio2019にあるTinySTG.slnを読み込みビルドする

### Windows版(SDL1.2)をVisualStudio2019でビルド
VisualStudio2019でTinySTG\SDL\VisualStudio2019にあるTinySTG.slnを読み込みビルドする

### Windows版(SDL2)をVisualStudio2019でビルド
VisualStudio2019でTinySTG\SDL2\VisualStudio2019にあるTinySTG.slnを読み込みビルドする

### Linux版(SDL1.2)をビルド
LinuxにSDL1.2の開発環境を作成する。(Linuxに関しては詳しくないためググってください)  
```
$ cd TinySTG/SDL/Linux
$ make
```

### RaspberryPI版(SDL2)をビルド  
RaspberryPI OSにSDL2の開発環境を作成する。(Linuxに関しては詳しくないためググってください)  
```
$ cd TinySTG/SDL2/RaspberryPI
$ make
```

### RG Nano(FunKeyOS)版(SDL1.2)をビルド  
- LinuxにFunKeyOSの開発環境を作成する。  
https://daimonsoft.info/kuran_kuran/index.php?RG%20Nano/%E9%96%8B%E7%99%BA%E7%92%B0%E5%A2%83
- 以下を実行してビルドする  
```
$ cd TinySTG/SDL/RGnano
$ make
```
- OpenPackage Creatorでopkファイルを作成する  
Windows上に「TinySTG_exe」フォルダを作成する。  
TinySTG_exeフォルダの中に「TinySTG」フォルダを作成して「bgm.adp」と「TinySTG.png」をコピーする。  
TinySTG_exeフォルダの中にLinuxでビルドした「TinySTG_exe」ファイルをコピーする。  
TinySTG_exeフォルダの中に「TinySTG.funkey-s.desktop 」ファイルを作成して以下の内容に編集する。  
```
[Desktop Entry]
Name=TinySTG
Comment=A simple shooting game app
Exec=TinySTG_exe
Icon=TinySTG.png
Categories=applications
```
opkファイルを作成するためのフォルダ構成  
```
+\TinySTG_exe (フォルダ)
|+-\TinySTG (リソース用フォルダ)
|  +- bgm.adp (ADPCM形式のBGMファイル)
+- TinySTG_exe (実行ファイル)
+- TinySTG.png (アイコンファイル)
+- TinySTG.funkey-s.desktop (FunkeyOS用設定ファイル)
```
WindowsでOpenPackage Creatorを起動して「TinySTG_exe」フォルダをドロップする。  
ダイアログが表示されるので「Use existing as is」を選択する。  
packボタンを押してopkファイルを作成する。  

### RG Nano(AnbernicOS/FunKeyOS)版(SDL1.2)をビルド  
- Linuxにmiyooの開発環境を作成する。  
https://github.com/MiyooCFW/toolchain/releases/download/v2.0.0/miyoo-toolchain-v2.0.0-arm-buildroot-linux-musleabi_sdk-buildroot.tar.gz  
```
$ cd ~
$ wget https://github.com/MiyooCFW/toolchain/releases/download/v2.0.0/miyoo-toolchain-v2.0.0-arm-buildroot-linux-musleabi_sdk-buildroot.tar.gz
$ cd /opt/
$ sudo tar -zxvf ~/miyoo-toolchain-v2.0.0-arm-buildroot-linux-musleabi_sdk-buildroot.tar.gz
$ sudo mv arm-buildroot-linux-musleabi_sdk-buildroot miyoo
```
- 以下を実行してビルドする  
```
$ cd TinySTG/SDL/RGnano-miyoo
$ make
```
あとは上のRG Nano(FunKeyOS)版(SDL1.2)をビルドと同じです。  

### TinyArcade版をビルド
ArduinoIDEをインストールして以下の設定をする。  
https://daimonsoft.info/kuran_kuran/index.php?TinyArcade/%E9%96%8B%E7%99%BA%E7%92%B0%E5%A2%83%E3%82%A4%E3%83%B3%E3%82%B9%E3%83%88%E3%83%BC%E3%83%AB  
ArduinoIDEでTinySTG.inoを読み込んでビルドする。  

### PyBadge版をビルド
ArduinoIDEをインストールして以下の設定をする。  
https://daimonsoft.info/kuran_kuran/index.php?pyBadge/%E9%96%8B%E7%99%BA%E7%92%B0%E5%A2%83%E3%82%A4%E3%83%B3%E3%82%B9%E3%83%88%E3%83%BC%E3%83%AB%28Arduino%20IDE%29  
ArduinoIDEでTinySTG.inoを読み込んでビルドする。  

### RaspberrryPI Pico + Waveshare19340版をビルド  
- WindowsにRaspberrryPI Pico SDKをインストールする。  
- SDK内にある「Developer Command Prompt for Pico」を起動する  
```
> mkdir build
> cd build
> cmake -G "NMake Makefiles" ..
> nmake
```
## ライセンス
TinySTGはMIT Licenseとします。  
SDL 2.0はzlibライセンス, SDL 1.2はGNU LGPLライセンスです。  

## 作者について
- Twitter

https://twitter.com/kuran_kuran

- kuran_kuran page!

https://daimonsoft.info/kuran_kuran/
