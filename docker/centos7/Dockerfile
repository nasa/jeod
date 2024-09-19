FROM esgl-gitlab.jsc.nasa.gov:5005/ci_build/centos_7_with_nasa_certs:latest

# Needed to work after the CentOS7 end of life.
RUN sed -i 's/mirrorlist/#mirrorlist/g' /etc/yum.repos.d/CentOS-*
RUN sed -i 's|#baseurl=http://mirror.centos.org|baseurl=http://vault.centos.org|g' /etc/yum.repos.d/CentOS-*

RUN yum -y groupinstall "Development Tools" && \
    yum -y erase swig && \
    yum install -y bison clang flex git llvm make maven swig3 cmake clang-devel \
    gcc gcc-c++ java-11-openjdk-devel libxml2-devel llvm-devel llvm-static \
    ncurses-devel openmotif openmotif-devel perl perl-Digest-MD5 udunits2 \
    udunits2-devel which zlib-devel gtest-devel libX11-devel libXt-devel python-devel \
    zip \
    which python3 cmake3 python-enum34 lcov python3-numpy \
    texlive-latex texlive-ifmtarg texlive-appendix texlive-wasysym texlive-metafont \
    texlive-bibtex texlive-wasy texlive-mfware texlive-changebar texlive-ec texlive-mdwtools \
    texlive-amscls texlive-latex-bin texlive-cm texlive-pdftex-def texlive-cm-super \
    python3-pip python3-devel \
    cppcheck && \
    pip3 install flawfinder metrixpp matplotlib

# Set up and build trick on the image under /data/
RUN mkdir /data/ && \
    export TRICK_CFLAGS=-g && export TRICK_CXXFLAGS=-g && export MAKEFLAGS=-j6 && \
    cd /data/ && git clone https://github.com/nasa/trick && \
    cd /data/trick && git checkout 19.7.2 -b r19.7.2 && ./configure && make && \
    chmod -R ugo+rx /data/trick*

ENV CSPICE_URL=http://naif.jpl.nasa.gov/pub/naif/toolkit/C/PC_Linux_GCC_64bit/packages
RUN mkdir -p /data && \
    yum install -y wget csh && \
    cd /data && wget ${CSPICE_URL}/cspice.tar.Z && \
        wget ${CSPICE_URL}/importCSpice.csh && \
        csh -f importCSpice.csh

# Run a bash shell
CMD ["/bin/bash"]
