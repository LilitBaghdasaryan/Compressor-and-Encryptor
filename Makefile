CC = g++
CFLAGS = -std=c++11
LDFLAGS = -lssl -lcrypto

SRCDIR = src
INCDIR = inc
BINDIR = bin

SRCS = $(wildcard $(SRCDIR)/*.cpp)
OBJS = $(patsubst $(SRCDIR)/%.cpp,$(BINDIR)/%.o,$(SRCS))
TARGET = $(BINDIR)/myprogram

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

$(BINDIR)/%.o: $(SRCDIR)/%.cpp
	$(CC) $(CFLAGS) -I$(INCDIR) -c $< -o $@

clean:
	rm -rf $(BINDIR)/*.o $(TARGET)

