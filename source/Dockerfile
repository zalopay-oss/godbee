FROM golang:latest
LABEL maintainer="Tran. <tranndc@vng.com.vn>"
WORKDIR /server
COPY go.mod go.sum config.yaml ./
RUN go mod download
COPY . .
RUN make build-server
CMD ["./server"]
