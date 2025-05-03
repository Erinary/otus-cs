#!/usr/bin/env bash

set -e
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
docker build -t bison-flex-gen "${SCRIPT_DIR}/grammar" && docker run --rm -v "${SCRIPT_DIR}:/target" bison-flex-gen
