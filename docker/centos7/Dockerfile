FROM esgl-gitlab.jsc.nasa.gov:5005/ci_build/centos_7_with_nasa_certs:latest

RUN yum -y groupinstall "Development Tools" && \
    yum -y erase swig && \
    yum install -y bison clang flex git llvm make maven swig3 cmake clang-devel \
    gcc gcc-c++ java-11-openjdk-devel libxml2-devel llvm-devel llvm-static \
    ncurses-devel openmotif openmotif-devel perl perl-Digest-MD5 udunits2 \
    udunits2-devel which zlib-devel gtest-devel libX11-devel libXt-devel python-devel \
    zip \
    which python3 cmake3 python-enum34 lcov python2-numpy.noarch python-matplotlib \
    texlive-latex texlive-ifmtarg texlive-appendix texlive-wasysym texlive-metafont \
    texlive-bibtex texlive-wasy texlive-mfware texlive-changebar texlive-ec texlive-mdwtools \
    texlive-amscls texlive-latex-bin texlive-cm texlive-pdftex-def texlive-cm-super \
    python2-pip python3-pip \
    cppcheck && \
    pip2 install flawfinder && pip3 install flawfinder metrixpp

# Set up and build trick on the image under /data/
RUN mkdir /data/ && \
    export TRICK_CFLAGS=-g && export TRICK_CXXFLAGS=-g && export MAKEFLAGS=-j6 && \
    cd /data/ && git clone https://github.com/nasa/trick && \
    cd /data/trick && git checkout 19.6.0 -b r19.6.0 && ./configure && make && \
    chmod -R ugo+rx /data/trick*

ENV CSPICE_URL http://naif.jpl.nasa.gov/pub/naif/toolkit/C/PC_Linux_GCC_64bit/packages
RUN mkdir -p /data && \
    yum install -y wget csh && \
    cd /data && wget ${CSPICE_URL}/cspice.tar.Z && \
        wget ${CSPICE_URL}/importCSpice.csh && \
        csh -f importCSpice.csh

# Run a bash shell
CMD ["/bin/bash"]
