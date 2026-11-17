# Use this flag with GCC versions after 1999.
post_ansi_flags = -ansi -m32 -D_GNU_SOURCE
ccf = -w

src = $(wildcard src/*.c)
hdr = $(wildcard src/*.h)
obj = $(src:src/%.c=obj/%.o)

all: rc

rc: $(obj)
	@echo -e '\e[35mLD $^\e[0m'
	@cc $^ -o $@ -m32

obj/%.o: src/%.c $(hdr)
	@echo -e '\e[32mCC $<\e[0m'
	@mkdir -p obj
	@cc -c $< -o $@ $(ccf) $(post_ansi_flags)

.PHONY: all clean install

clean:
	rm -rf rc obj

install: rc rc.1
	cp rc /bin
	cp rc.1 /usr/share/man/man1/rc.1
