CC = arm-funkey-linux-musleabihf-gcc.br_real
CPPFLAG = -lSDL -D SDL -D RGNANO -lstdc++
LIBS = 
INCDIR = 
LIBDIR = 
TARGET = TinySTG_exe

OBJS = main.o \
Boss1.o \
Boss2.o \
Boss3.o \
Boss4.o \
Boss5.o \
Boss6.o \
Bullet.o \
Enemy.o \
Enemy1.o \
Enemy2.o \
Enemy3.o \
Enemy4.o \
Enemy5.o \
Explode.o \
Game.o \
Global.o \
MainLoop.o \
Player.o \
PowerUp.o \
Resource.o \
Shot.o \
Sound.o \
AdpcmDecorder.o \
Mathmatics.o \
Report.o \
SoundMixer.o \
SoundPlayer.o \
Controller.o \
Screen.o \
SoundStream.o \
TinyFile.o \
RingBuffer.o

$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(CPPFLAG) $(LIBDIR) $(LIBS)

main.o: ../../src/TinyLibrary/SDL/main.cpp
	$(CC) -c $(CPPFLAG) $(INCDIR) $<

Boss1.o: ../../src/Boss1.cpp
	$(CC) -c $(CPPFLAG) $(INCDIR) $<

Boss2.o: ../../src/Boss2.cpp
	$(CC) -c $(CPPFLAG) $(INCDIR) $<

Boss3.o: ../../src/Boss3.cpp
	$(CC) -c $(CPPFLAG) $(INCDIR) $<

Boss4.o: ../../src/Boss4.cpp
	$(CC) -c $(CPPFLAG) $(INCDIR) $<

Boss5.o: ../../src/Boss5.cpp
	$(CC) -c $(CPPFLAG) $(INCDIR) $<

Boss6.o: ../../src/Boss6.cpp
	$(CC) -c $(CPPFLAG) $(INCDIR) $<

Bullet.o: ../../src/Bullet.cpp
	$(CC) -c $(CPPFLAG) $(INCDIR) $<

Enemy.o: ../../src/Enemy.cpp
	$(CC) -c $(CPPFLAG) $(INCDIR) $<

Enemy1.o: ../../src/Enemy1.cpp
	$(CC) -c $(CPPFLAG) $(INCDIR) $<

Enemy2.o: ../../src/Enemy2.cpp
	$(CC) -c $(CPPFLAG) $(INCDIR) $<

Enemy3.o: ../../src/Enemy3.cpp
	$(CC) -c $(CPPFLAG) $(INCDIR) $<

Enemy4.o: ../../src/Enemy4.cpp
	$(CC) -c $(CPPFLAG) $(INCDIR) $<

Enemy5.o: ../../src/Enemy5.cpp
	$(CC) -c $(CPPFLAG) $(INCDIR) $<

Explode.o: ../../src/Explode.cpp
	$(CC) -c $(CPPFLAG) $(INCDIR) $<

Game.o: ../../src/Game.cpp
	$(CC) -c $(CPPFLAG) $(INCDIR) $<

Global.o: ../../src/Global.cpp
	$(CC) -c $(CPPFLAG) $(INCDIR) $<

MainLoop.o: ../../src/MainLoop.cpp
	$(CC) -c $(CPPFLAG) $(INCDIR) $<

Player.o: ../../src/Player.cpp
	$(CC) -c $(CPPFLAG) $(INCDIR) $<

PowerUp.o: ../../src/PowerUp.cpp
	$(CC) -c $(CPPFLAG) $(INCDIR) $<

Resource.o: ../../src/Resource.cpp
	$(CC) -c $(CPPFLAG) $(INCDIR) $<

Shot.o: ../../src/Shot.cpp
	$(CC) -c $(CPPFLAG) $(INCDIR) $<

Sound.o: ../../src/Sound.cpp
	$(CC) -c $(CPPFLAG) $(INCDIR) $<

AdpcmDecorder.o: ../../src/TinyLibrary/AdpcmDecorder.cpp
	$(CC) -c $(CPPFLAG) $(INCDIR) $<

Mathmatics.o: ../../src/TinyLibrary/Mathmatics.cpp
	$(CC) -c $(CPPFLAG) $(INCDIR) $<

Report.o: ../../src/TinyLibrary/Report.cpp
	$(CC) -c $(CPPFLAG) $(INCDIR) $<

SoundMixer.o: ../../src/TinyLibrary/SoundMixer.cpp
	$(CC) -c $(CPPFLAG) $(INCDIR) $<

SoundPlayer.o: ../../src/TinyLibrary/SoundPlayer.cpp
	$(CC) -c $(CPPFLAG) $(INCDIR) $<

Controller.o: ../../src/TinyLibrary/SDL/Controller.cpp
	$(CC) -c $(CPPFLAG) $(INCDIR) $<

Screen.o: ../../src/TinyLibrary/SDL/Screen.cpp
	$(CC) -c $(CPPFLAG) $(INCDIR) $<

SoundStream.o: ../../src/TinyLibrary/SDL/SoundStream.cpp
	$(CC) -c $(CPPFLAG) $(INCDIR) $<

TinyFile.o: ../../src/TinyLibrary/SDL/TinyFile.cpp
	$(CC) -c $(CPPFLAG) $(INCDIR) $<

RingBuffer.o: ../../src/TinyLibrary/SDL/RingBuffer.cpp
	$(CC) -c $(CPPFLAG) $(INCDIR) $<

clean:
	-rm -f $(OBJS) $(TARGET) *.d
