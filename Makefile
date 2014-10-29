TARGET = reshp
CC = g++
CFLAGS = -Wall -Wextra -Os -ansi
LFLAGS = 
SRCDIR = src
OBJDIR = obj
SOURCES := $(wildcard $(SRCDIR)/*.cpp) $(wildcard $(SRCDIR)/*/*.cpp)
OBJECTS := $(SOURCES:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)

$(TARGET): $(OBJECTS)
	$(CC) $(LFLAGS) -o $@ $(OBJECTS)

$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.cpp
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

all: $(TARGET)

clean:
	rm -Rf $(OBJDIR) $(TARGET)