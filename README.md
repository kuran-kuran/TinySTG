# TinySTG
縦スクロールシューティングゲーム TinySTG です。

https://github.com/kuran-kuran/TinySTG

## はじめに

TinyArcade、PyBadge、Windowsで動作します。
Waveshare19340 を接続した RaspberryPI Pico ではまだ画面サイズの調整がまだだったり音が出なかったりするためテスト版になっています。

##Linuxでのビルド
- g++ -c ../../src/Boss1.cpp ../../src/Boss2.cpp ../../src/Boss3.cpp ../../src/Boss4.cpp ../../src/Boss5.cpp ../../src/Boss6.cpp ../../src/Bullet.cpp ../../src/Enemy.cpp ../../src/Enemy1.cpp ../../src/Enemy2.cpp ../../src/Enemy3.cpp ../../src/Enemy4.cpp ../../src/Enemy5.cpp ../../src/Explode.cpp ../../src/Game.cpp ../../src/Global.cpp ../../src/MainLoop.cpp ../../src/Player.cpp ../../src/PowerUp.cpp ../../src/Resource.cpp ../../src/Shot.cpp ../../src/Sound.cpp ../../src/TinyLibrary/AdpcmDecorder.cpp ../../src/TinyLibrary/Mathmatics.cpp ../../src/TinyLibrary/SoundMixer.cpp ../../src/TinyLibrary/SoundPlayer.cpp ../../src/TinyLibrary/SDL2/Controller.cpp ../../src/TinyLibrary/SDL2/main.cpp ../../src/TinyLibrary/SDL2/Screen.cpp ../../src/TinyLibrary/SDL2/SoundStream.cpp ../../src/TinyLibrary/SDL2/TinyFile.cpp ../../src/TinyLibrary/SDL2/RingBuffer.cpp `sdl2-config --cflags --libs` -D SDL2

- g++ -o TinySTG_exe *.o `sdl2-config --cflags --libs`


## ライセンス
TinySTGはMIT Licenseとします。

## 作者について
- Twitter

https://twitter.com/kuran_kuran

- kuran_kuran page!

https://daimonsoft.info/kuran_kuran/
