
# Makefile para Aplicativo de Recarga - Terminal PAX D230
# SDK Prolin 3.5

# Compilador e flags
CC = arm-none-linux-gnueabi-gcc
CFLAGS = -O0 -g2 -Wall -funwind-tables
LDFLAGS = -lpthread -lm

# Diretórios
INCLUDE_DIRS = -I./inc -I$(PROLIN_SDK)/include -I$(PROLIN_SDK)/include/freetype2

# Arquivos de código-fonte
SOURCES = main.c posutils.c
OBJECTS = $(SOURCES:.c=.o)
EXECUTABLE = recarga

# Regra padrão
all: $(EXECUTABLE)

# Regra para o executável
$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

# Regra para objetos
%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDE_DIRS) -c $< -o $@

# Limpeza
clean:
	rm -f $(OBJECTS) $(EXECUTABLE)

.PHONY: all clean
