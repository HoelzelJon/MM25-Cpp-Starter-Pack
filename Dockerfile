FROM buildpack-deps:stretch
RUN apt-get update
RUN apt-get install -y clang libc++-dev  libc++abi-dev libboost-all-dev
WORKDIR /app
COPY . .
RUN make clean
RUN make server
CMD ./server
