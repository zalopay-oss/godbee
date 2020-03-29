FROM golang:latest
LABEL maintainer="zalopay-oss"
WORKDIR /server
COPY go.mod go.sum ./
RUN go mod download
COPY . .
RUN ./build-server.sh
CMD ["./server"]
