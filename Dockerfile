FROM golang:latest
LABEL maintainer="Tran. <tranndc@vng.com.vn>"
WORKDIR /server
COPY go.mod go.sum ./
RUN go mod download
COPY source .
RUN make build-server
CMD ["./server"]
