FROM debian:latest

RUN apt-get update
RUN apt-get install -y make clang netcat-traditional

RUN mkdir -p /app
COPY . /app
WORKDIR /app
RUN rm -rf build

RUN make

CMD ["./ft_shield"]