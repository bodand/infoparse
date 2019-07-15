#!/usr/bin/env bash

mkdir -p -- /usr/local/include
curl -Ls https://isdevnet.com/boost/include | tar xJC /usr/local/include

mkdir -p -- /usr/local/lib
curl -Ls https://isdevnet.com/boost/lib | tar xJC /usr/local/lib

