FROM esgl-gitlab.jsc.nasa.gov:5005/ci_build/oracle_8_with_nasa_certs:latest

RUN yum-config-manager --enable ol8_codeready_builder

RUN dnf -y groupinstall "Development Tools" && \
    dnf -y module install python27 && \
    dnf install -y bison clang flex git llvm make maven swig cmake clang-devel \
    gcc gcc-c++ java-11-openjdk-devel libxml2-devel llvm-devel llvm-static \
    ncurses-devel openmotif openmotif-devel perl perl-Digest-MD5 udunits2 \
    udunits2-devel which zlib-devel libX11-devel libXt-devel  \
    python3-devel diffutils \
    texlive-latex texlive-ifmtarg texlive-appendix texlive-wasysym texlive-metafont \
    texlive-bibtex texlive-wasy texlive-mfware texlive-changebar texlive-ec \
    texlive-mdwtools texlive-amscls texlive-tabu texlive-dvips texlive-multirow \
    texlive-adjustbox texlive-sectsty texlive-fancyhdr texlive-natbib texlive-tocloft \
    texlive-helvetic texlive-courier texlive-epstopdf texlive-tex4ht texlive-gsftopk \
    texlive-collection-latex texlive-collection-latexrecommended \
    which python3 cmake3 python2-enum34 lcov python2-numpy python3-numpy python2-devel \
    python2-pip python3-pip python3-regex python3-mako \
    cppcheck vim emacs doxygen && \
    pip2 install matplotlib flawfinder && pip3 install flawfinder matplotlib metrixpp

# Set up and build trick on the image under /data/ 
RUN mkdir /data/ && \
    export TRICK_CFLAGS=-g && export TRICK_CXXFLAGS=-g && export MAKEFLAGS=-j6 && \
    cd /data/ && git clone https://github.com/nasa/trick && \
    cd /data/trick && git checkout 19.6.0 -b r19.6.0 && ./configure && make && \
    chmod -R ugo+rx /data/trick*

ENV CSPICE_URL http://naif.jpl.nasa.gov/pub/naif/toolkit/C/PC_Linux_GCC_64bit/packages
RUN mkdir -p /data && \
    dnf install -y wget csh && \
    cd /data && wget ${CSPICE_URL}/cspice.tar.Z && \
        wget ${CSPICE_URL}/importCSpice.csh && \
        csh -f importCSpice.csh

# Run a bash shell
CMD ["/bin/bash"]
