.PHONY: test clean build

build:
	pio run --environment default

test:
	pio test --environment native

clean:
	$(RM) -r .pio
