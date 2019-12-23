server:
	go build ./cmd/server/
	./server
	rm server

build-server:
	go build ./cmd/server/

cli:
	go build ./cmd/client/ && ./client
	rm client

build-cli:
	go build ./cmd/client/

locust:
	locust -f ./locust/locustfile.py --master --master-bind-host=127.0.0.1 --master-bind-port=5557  --web-port=7000

benchmark:
	go build ./benchmark/run

clean:
	rm data/*

test:
	go test ./service -v
