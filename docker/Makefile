# This setup builds docker containers capable of jeod testing
.PHONY: check-env \
        build-centos7 run-centos7 run-centos7-local \
				build-centos8 run-centos8 run-centos8-local \
        build-ubuntu18 run-ubuntu18 run-ubuntu18-local

# CentOS 7 targets
build-centos7: 
	docker build --build-arg myuser=$(shell whoami) -t jeod_centos7 centos7/

run-centos7: build-centos7
	docker run -ti --rm jeod_centos7

run-centos7-local: build-centos7
	docker run -ti \
      -e HOME=${HOME} \
      -v "${HOME}:${HOME}/" \
      -v /etc/group:/etc/group:ro \
      -v /etc/passwd:/etc/passwd:ro \
      --security-opt seccomp=unconfined \
      -u $(shell id -u ${USER} ):$(shell id -g ${USER} ) \
      --rm jeod_centos7


# CentOS 8 targets
build-centos8:
	docker build --build-arg myuser=$(shell whoami) -t jeod_centos8 centos8/

run-centos8: build-centos8
	docker run -ti --rm jeod_centos8

run-centos8-local: build-centos8
	docker run -ti \
      -e HOME=${HOME} \
      -v "${HOME}:${HOME}/" \
      -v /etc/group:/etc/group:ro \
      -v /etc/passwd:/etc/passwd:ro \
      --security-opt seccomp=unconfined \
      -u $(shell id -u ${USER} ):$(shell id -g ${USER} ) \
      --rm jeod_centos8

# Ubuntu 18 targets
build-ubuntu18:
	docker build --build-arg myuser=$(shell whoami) -t jeod_ubuntu18 ubuntu18/

run-ubuntu18: build-ubuntu18
	docker run -ti --rm jeod_ubuntu18

run-ubuntu18-local: build-ubuntu18
	docker run -ti \
      -e HOME=${HOME} \
      -v "${HOME}:${HOME}/" \
      -v /etc/group:/etc/group:ro \
      -v /etc/passwd:/etc/passwd:ro \
      --security-opt seccomp=unconfined \
      -u $(shell id -u ${USER} ):$(shell id -g ${USER} ) \
      --rm jeod_ubuntu18


all: build-centos7 build-centos8 build-ubuntu18


guard-%:
	@ if [ "${${*}}" = "" ]; then \
        echo "Environment variable $* not set. See 'make help'"; \
        exit 1; \
    fi


all: build-centos7 build-centos8 build-ubuntu18

help:
	#  Target            Action
	# -----------+----------------------------------------------
	#  help              Display this message
	# -----------+----------------------------------------------
	# build-centos7      Build the jeod CentOS 7 docker image.
	#                    This requires the environment var
	#                    CSPICE_LOCATION to be defined since
	#                    the build copies cspice into the image.
	# run-centos7        Create and run a jeod CentOS 7 docker
	#                    container as root user. This reflects
	#                    how Gitlab CI will run the container.
	# run-centos7-local  Create and run a jeod CentOS 7 docker
	#                    container as your user. This will mount
	#                    your home directory and milage may vary
	#                    on different flavors of Linux on host.
	# build-centos8      Build the jeod CentOS 8 docker image.
	#                    This requires the environment var
	#                    CSPICE_LOCATION to be defined since
	#                    the build copies cspice into the image.
	# run-centos8        Create and run a jeod CentOS 8 docker
	#                    container as root user. This reflects
	#                    how Gitlab CI will run the container.
	# run-centos8-local  Create and run a jeod CentOS 8 docker
	#                    container as your user. This will mount
	#                    your home directory and milage may vary
	#                    on different flavors of Linux on host.
	# build-ubuntu18     Build the jeod Ubuntu 18 docker image.
	#                    This requires the environment var
	#                    CSPICE_LOCATION to be defined since
	#                    the build copies cspice into the image.
	# run-ubuntu18       Create and run a jeod Ubuntu 18 docker
	#                    container as root user. This reflects
	#                    how Gitlab CI will run the container.
	# run-ubuntu18-local Create and run a jeod Ubuntu 18 docker
	#                    container as your user. This will mount
	#                    your home directory and milage may vary
	#                    on different flavors of Linux on host.
