GCCARG = -g

HEADFILESPATH = -Iheaders -I${JAVA_HOME}/include -I${JAVA_HOME}/include/linux
VPATH = src/c:headers:out

#dir
OBJDIR = out
TARGETDIR = target


OBJS = main.o cLinkedList.o cHashMap.o cArrayList.o avro_serializer.o kafka_producer.o base64.o communicator.o heat_conduct.o HighPerformanceHC_native_implement_Invoker.o utils.o
TARGET_LIBRARY = libHP.so
TARGET_EXEC = HP
OBJOUT = $(patsubst %,$(OBJDIR)/%,$(OBJS))



list : $(TARGET_LIBRARY) $(TARGET_EXEC)


$(TARGET_LIBRARY) : $(OBJS)
	@mkdir -p $(TARGETDIR) || true
	gcc -shared -o $(TARGETDIR)/$(TARGET_LIBRARY) $(OBJOUT) -lavro -lrdkafka -lmpi

$(TARGET_EXEC) : $(OBJS)
	@mkdir -p $(TARGETDIR) || true
	gcc  -o $(TARGETDIR)/$(TARGET_EXEC) $(OBJOUT) -lavro -lrdkafka -lmpi

$(OBJS) : %.o : %.c
	@mkdir -p $(OBJDIR) || true
	gcc -fPIC -o $(OBJDIR)/$@ $(GCCARG) $(HEADFILESPATH) -c $^


.PHONY=clean

clean:
	rm -f $(TARGETDIR)/* $(OBJDIR)/*.o
	@echo clean finish