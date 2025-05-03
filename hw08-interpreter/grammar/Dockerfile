FROM debian:bookworm-slim

# Install required packages
RUN apt-get update && \
    DEBIAN_FRONTEND=noninteractive apt-get install -y \
    bison \
    flex \
    gcc \
    make \
    build-essential \
    && rm -rf /var/lib/apt/lists/*

# Set working directories
WORKDIR /workspace

# Copy source grammar files
COPY lexer.lex parser.y ./grammar/

# Create output directory
RUN mkdir /output

# Generate parser .h/.c with Bison
RUN bison -d -o /output/parser.c grammar/parser.y
# Generate lexer .h/.c with flex
RUN flex -o /output/lexer.c --header-file=/output/lexer.h grammar/lexer.lex

ENTRYPOINT ["bash"]
# Copy to output dir (hw08-interpreter dir must be passed as volume at /target inside container)
CMD ["-c", "rm -rf /target/generated && cp -r /output /target/generated"]
