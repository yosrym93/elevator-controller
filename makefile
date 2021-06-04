.PHONY: test clean build

build:
	pio run --environment default -v

test:
	pio test --environment native -v

clean:
	$(RM) -r .pio
