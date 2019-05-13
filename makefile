TARGET 		= eirb3d

CC 		= g++ 

CFLAGS 		= -W -Wall 

LINKER 		= g++ -o

LFLAGS 		= -Wall -pg -I. -lm -lSDL2 

SRCDIR 		= src
OBJDIR 		= obj
BINDIR 		= bin

SOURCES 	:= $(wildcard $(SRCDIR)/*.c)
INCLUDES 	:= $(wildcard $(SRCDIR)/*.h)
OBJECTS 	:= $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
rm 		= rm -f


mainMake : remove all execDiablo

diablo : remove all execDiablo
head   : remove all execHead
body   : remove all execBody

execDiablo :
	./bin/eirb3d bin/data/diablo.obj bin/data/diablo_diffuse.tga

execHead :
	./bin/eirb3d bin/data/head.obj bin/data/head_diffuse.tga

execBody :
	./bin/eirb3d bin/data/body.obj bin/data/body_diffuse.tga


all: $(BINDIR)/$(TARGET)

$(BINDIR)/$(TARGET): $(OBJECTS)
	@$(LINKER) $@ $(OBJECTS) $(LFLAGS) 

$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.c
	@$(CC) $(CFLAGS) -c $< -o $@


.PHONY: clean
clean:
	@$(rm) $(OBJECTS)

.PHONY: remove
remove: clean
	@$(rm) $(BINDIR)/$(TARGET)
