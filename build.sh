#!/usr/bin/env bash

cd src
eosio-cpp betbos.cpp -o ../betbos.wasm -abigen -I ../include -I ./ -R ../resources
