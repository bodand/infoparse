#!/usr/bin/env bash

echo [Boosting Time]

mkdir -p -- /usr/local/include/boost
curl -Ls https://isdevnet.com/boost/include | tar xJC /usr/local/include/boost

mkdir -p -- /usr/local/lib
curl -Ls https://isdevnet.com/boost/lib | tar xJC /usr/local/lib

