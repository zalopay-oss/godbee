FROM golang:latest
LABEL maintainer="zalopay-oss"
WORKDIR /server
COPY go.mod go.sum ./
RUN go mod download
COPY source .
RUN make build-server
CMD ["./server"]
