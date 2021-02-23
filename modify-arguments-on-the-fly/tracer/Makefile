APP=tracer
ROOT=$(realpath ..)

build:
	go build -o $(APP)

run:
	sudo ./$(APP) $(ROOT)/tracee/tracee 'main.greet'

clean:
	rm -f $(APP)
