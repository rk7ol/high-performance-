GCCARG = -g

HEADFILESPATH = headers
VPATH = src/c:headers:out

#dir
OBJDIR = out
TARGETDIR = target

OBJS = main.o cLinkedList.o cHashMap.o cArrayList.o avro_serializer.o kafka_producer.o base64.o communicator.o heat_conduct.o
TARGET = HP
OBJOUT = $(patsubst %,$(OBJDIR)/%,$(OBJS))



$(TARGET) : $(OBJS)
	@mkdir -p $(TARGETDIR) || true
	gcc -o $(TARGETDIR)/$(TARGET) $(OBJOUT) -lavro -lrdkafka -lmpi

$(OBJS) : %.o : %.c
	@mkdir -p $(OBJDIR) || true
	gcc -o $(OBJDIR)/$@ $(GCCARG) -I$(HEADFILESPATH) -c $^ 


.PHONY=clean

clean:
	rm -f $(TARGETDIR)/* $(OBJDIR)/*.o
	@echo clean finish