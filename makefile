modern_flags = -ansi -m32 -D_GNU_SOURCE
ccf = -w

src = $(wildcard src/*.c)
obj = $(src:src/%.c=obj/%.o)

all: rc

rc: $(obj)
	@echo -e '\e[35mLD $^\e[0m'
	@cc $^ -o $@

obj/%.o: src/%.c
	@echo -e '\e[32mCC $<\e[0m'
	@mkdir -p obj
	@cc -c $< -o $@ $(ccf) $(modern_flags)

.PHONY: all clean install

clean:
	rm -rf rc obj

install: rc rc.1
	cp rc /bin
	cp rc.1 /usr/share/man/man1/rc.1
