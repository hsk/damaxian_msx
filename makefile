#! make -f
#
# makefile - start
#



# directory
#

# source file directory
SRCDIR			=	sources

# include file directory
INCDIR			=	sources

# object file directory
OBJDIR			=	objects

# binary file directory
BINDIR			=	bin

# output file directory
OUTDIR			=	disk

# tool directory
TOOLDIR			=	./tools

# vpath search directories
VPATH			=	$(SRCDIR):$(INCDIR):$(OBJDIR):$(BINDIR)


# assembler
#

# assembler command
AS				=	sdasz80

# assembler flags
ASFLAGS			=	-ls -I$(INCDIR) -I.


# c compiler
#

# c compiler command
CC				=	sdcc

# c compiler flags
CFLAGS			=	-mz80 --opt-code-speed -I$(INCDIR) -I.


# linker
#

# linker command
LD				=	sdcc

# linker flags
LDFLAGS			=	-mz80 --no-std-crt0 --nostdinc --nostdlib --code-loc 0x8420 --data-loc 0xd400


# suffix rules
#
.SUFFIXES:			.s .c .rel

# assembler source suffix
.s.rel:
	$(AS) $(ASFLAGS) -o $(OBJDIR)/$@ $<

# c source suffix
.c.rel:
	$(CC) $(CFLAGS) -o $(OBJDIR)/$@ -c $<


# project files
#

# target name
TARGET			=	DAMAXIAN

# assembler source files
ASSRCS			=	crt0.s \

# c source files
CSRCS			=	main.c System.c Math.c \
					App.c Back.c Game.c Ship.c Shot.c Enemy.c Bullet.c pattern.c bg.c

# object files
OBJS			=	$(ASSRCS:.s=.rel) $(CSRCS:.c=.rel)

r: $(TARGET).com
	openmsx -machine Sanyo_PHC-70FD2 -diska $(OUTDIR)

# build project target
#
$(TARGET).com: tool $(OBJDIR) $(BINDIR) $(OBJS)
	$(LD) $(LDFLAGS) -o $(BINDIR)/$(TARGET).ihx $(foreach file,$(OBJS),$(OBJDIR)/$(file))
	$(TOOLDIR)/ihx2bload $(BINDIR)/$(TARGET).ihx -o $(OUTDIR)/$(TARGET).BIN

tool:
	cd tools; make

$(OBJDIR):
	mkdir -p $(OBJDIR)
$(BINDIR):
	mkdir -p $(BINDIR)
# clean project
#
clean:
	cd tools; make clean
	rm -rf $(OBJDIR)
	rm -rf $(BINDIR)
##	@del /F /Q makefile.depend


# build depend file
#
depend:
##	ifneq ($(strip $(CSRCS)),)
##		$(CC) $(CFLAGS) -MM $(foreach file,$(CSRCS),$(SRCDIR)/$(file)) > makefile.depend
##	endif


# build resource file
#
resource:
	cd resources/chr; python pattern.py > ../../sources/pattern.c
	cd resources/bg; python bg.py > ../../sources/bg.c


# phony targets
#
.PHONY:				clean depend


# include depend file
#
-include makefile.depend



# makefile - end
