FROM debian:bookworm-slim AS build

RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    git \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /src

COPY . .

RUN ls -la

RUN rm -rf build; mkdir -p build

RUN cmake -S . -B build && cmake --build build --target otus_cs_hw08

FROM debian:bookworm-slim

COPY --from=build /src/build/otus_cs_hw08 /usr/local/bin/interpreter

ENTRYPOINT ["/usr/local/bin/interpreter"]