CFLAGS+=-Wall -Wextra
CFLAGS+=`pkg-config --cflags gtk+-3.0`
LIBS+=`pkg-config --libs gtk+-3.0`
PROG=gnotesc

build:
	$(CC) $(CFLAGS) -o $(PROG) src/*.c $(LIBS)

clean:
	rm -f $(PROG)

install:
	install -D -m 755 $(PROG) /usr/bin/
	install -D -m 624 gnotesc.desktop /usr/share/applications/

uninstall:
	rm -f /usr/bin/$(PROG)
