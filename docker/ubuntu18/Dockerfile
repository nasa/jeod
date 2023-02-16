FROM ubuntu:18.04

ENV TZ=US/Central

# Preliminaries
# Avoid interaction in install due to tzdata
RUN apt-get update
RUN apt install -y build-essential vim emacs git git-lfs && \
     ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone && \
     apt-get install -y tzdata

RUN apt-get install -y bison clang flex git llvm make maven swig cmake \
     curl g++ libx11-dev libxml2-dev libxt-dev libmotif-common libmotif-dev \
     python3-dev zlib1g-dev llvm-dev libclang-dev libudunits2-dev \
     libgtest-dev openjdk-11-jdk zip \
     vim emacs \
     python-enum34 python-numpy python-matplotlib \
     flawfinder apt-file \
     lcov \
     cppcheck \
     python3-pip && \
     pip3 install metrixpp

# Set up and build trick on the image under /data/
RUN mkdir -p /data/ && \
    export TRICK_CFLAGS=-g && export TRICK_CXXFLAGS=-g && export MAKEFLAGS=-j6 && export PYTHON_VERSION=3 && \
    cd /data/ && git clone https://github.com/nasa/trick && \
    cd /data/trick && git checkout 19.5.1 -b r19.5.1 && ./configure && make && \
    chmod -R ugo+rx /data/trick*

ENV CSPICE_URL http://naif.jpl.nasa.gov/pub/naif/toolkit/C/PC_Linux_GCC_64bit/packages
RUN mkdir -p /data && \
    apt-get install -y wget csh && \
    cd /data && wget ${CSPICE_URL}/cspice.tar.Z && \
        wget ${CSPICE_URL}/importCSpice.csh && \
        csh -f importCSpice.csh

ENV CMAKE_URL https://github.com/Kitware/CMake/releases/download/v3.24.1/cmake-3.24.1-linux-x86_64.sh
RUN mkdir -p /data && \
    cd /data && wget https://github.com/Kitware/CMake/releases/download/v3.24.1/cmake-3.24.1-linux-x86_64.sh && \
    sh cmake-3.24.1-linux-x86_64.sh --prefix=/usr/local --skip-license

RUN apt-get install -y texlive-latex-base texlive-latex-extra

# Force default system shell to bash so that regression script calls to pushd/popd work
RUN ln -sf /bin/bash /bin/sh

# Run a bash shell
CMD ["/bin/bash"]
