#!/usr/bin/env bash

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

docker build --progress=plain -f "${SCRIPT_DIR}/Dockerfile" -t my-simple-interpreter \
  "$(realpath "${SCRIPT_DIR}/..")" # Context should be project root
