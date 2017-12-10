TARGET_DYNAMIC = digest_dynamic
TARGET_STATIC = digest_static

CFLAGS = -std=c99 -MP -MMD
SRCS = main.c calculate.c print.c

all: $(TARGET_DYNAMIC) $(TARGET_STATIC)

DYNAMIC_FLAGS = -lz
STATIC_FLAGS = -Wl,-Bstatic -lz -Wl,-Bdynamic

$(TARGET_DYNAMIC): $(SRCS:%.c=%.o)
	$(CC) $(CFLAGS) $(DYNAMIC_FLAGS) -o $@ $(SRCS:%.c=%.o)

$(TARGET_STATIC): $(SRCS:%.c=%.o)
	$(CC) $(CFLAGS) $(STATIC_FLAGS) -o $@ $(SRCS:%.c=%.o)

main.o : calculate.h structure.h print.h
calculate.o : calculate.h
print.o : print.h

.PHONY: clean all

clean:
	rm -f *.o $(TARGET_DYNAMIC) $(TARGET_STATIC) *.d

-include $(SRCS:%.c=%.d)