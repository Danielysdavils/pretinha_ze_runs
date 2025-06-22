# Nome do executável final
TARGET = pretinha_ze_battle

# Compilador
CC = gcc

# Diretórios de include
INCLUDES = \
    -Iservices/data \
    -Iservices/joystick

# Arquivos-fonte
SRCS = \
    entities/players/player.c \
    entities/villains/villian.c \
    entities/gun/gun.c \
    entities/gun/bullet.c \
    entities/elements/life.c \
    services/level/escene.c \
    services/level/level.c \
    services/joystick/joystick.c \
    entities/utilities/position.c \
    services/data/data.c \
    pretinha_ze_battle.c

# Flags do compilador
CFLAGS = -g -fdiagnostics-color=always $(INCLUDES)

# Bibliotecas Allegro
LIBS = \
    -lallegro \
    -lallegro_audio \
    -lallegro_acodec \
    -lallegro_primitives \
    -lallegro_image \
    -lallegro_font \
    -lallegro_ttf \
    -lallegro_main

# Regra padrão: compilar o executável
all: $(TARGET)

# Como gerar o executável
$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

# Limpa os arquivos gerados
clean:
	rm -f $(TARGET)
