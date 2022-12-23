CC      = g++
CFLAGS  = `sdl2-config --cflags --libs` -D SDL2
TARGET  = TinySTG
SRCS    = ../../src/Boss1.cpp \
../../src/Boss2.cpp \
../../src/Boss3.cpp \
../../src/Boss4.cpp \
../../src/Boss5.cpp \
../../src/Boss6.cpp \
../../src/Bullet.cpp \
../../src/Enemy.cpp \
../../src/Enemy1.cpp \
../../src/Enemy2.cpp \
../../src/Enemy3.cpp \
../../src/Enemy4.cpp \
../../src/Enemy5.cpp \
../../src/Explode.cpp \
../../src/Game.cpp \
../../src/Global.cpp \
../../src/MainLoop.cpp \
../../src/Player.cpp \
../../src/PowerUp.cpp \
../../src/Resource.cpp \
../../src/Shot.cpp \
../../src/Sound.cpp \
../../src/TinyLibrary/AdpcmDecorder.cpp \
../../src/TinyLibrary/Mathmatics.cpp \
../../src/TinyLibrary/SoundMixer.cpp \
../../src/TinyLibrary/SoundPlayer.cpp \
../../src/TinyLibrary/SDL2/Controller.cpp \
../../src/TinyLibrary/SDL2/main.cpp \
../../src/TinyLibrary/SDL2/Screen.cpp \
../../src/TinyLibrary/SDL2/SoundStream.cpp \
../../src/TinyLibrary/SDL2/TinyFile.cpp \
../../src/TinyLibrary/SDL2/RingBuffer.cpp

OBJS    = $(SRCS:.cpp=.o)

INCDIR  = 

LIBDIR  = 

LIBS    = -lSDL2_image

$(TARGET): $(OBJS)
    $(CC) -o $@ $^ $(LIBDIR) $(LIBS)

$(OBJS): $(SRCS)
    $(CC) $(CFLAGS) $(INCDIR) -c $(SRCS)

all: clean $(OBJS) $(TARGET)

clean:
    -rm -f $(OBJS) $(TARGET) *.d
