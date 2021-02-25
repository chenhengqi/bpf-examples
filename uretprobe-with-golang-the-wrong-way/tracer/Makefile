APP=tracer
ROOT=$(realpath ..)

build:
	go build -o $(APP)

run:
	sudo ./$(APP) $(ROOT)/tracee/tracee 'main.foobar'

clean:
	rm -f $(APP)
