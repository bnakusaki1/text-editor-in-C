#!/usr/bin/env bash
set -e

make
exec ./kilo "$@"