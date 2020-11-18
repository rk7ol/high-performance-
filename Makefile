GCCARG = -g

HEADFILESPATH = headers
VPATH = src:headers:out

#dir
OBJDIR = out
TARGETDIR = target


OBJS = main.o cLinkedList.o cHashMap.o cArrayList.o
TARGET = HP
OBJOUT = $(patsubst %,$(OBJDIR)/%,$(OBJS))



$(TARGET) : $(OBJS)
	@mkdir -p $(TARGETDIR) || true
	gcc -o $(TARGETDIR)/$(TARGET) $(OBJOUT)

$(OBJS) : %.o : %.c
	@mkdir -p $(OBJDIR) || true
	gcc -o $(OBJDIR)/$@ $(GCCARG) -I$(HEADFILESPATH) -c $^ 


.PHONY=clean

clean:
	rm -f $(TARGETDIR)/* $(OBJDIR)/*.o
	@echo clean finish