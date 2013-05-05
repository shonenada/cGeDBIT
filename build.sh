#!/usr/bin/env sh

build() {
    mkdir target
    cd target
    cmake ../
    make
}

(build)

