FROM debian:8-slim

MAINTAINER louisblin "https://github.com/louisblin"

#
# Install packages dependencies
#

# Adding cxxtest to apt (and missing GPG keys)
RUN apt-get update && \
    apt-get install -my wget gnupg && \
    apt-key adv --recv-keys --keyserver keyserver.ubuntu.com 40976EAF437D05B5 3B4FE6ACC0B21F32 && \
    echo "deb http://us.archive.ubuntu.com/ubuntu vivid main universe" >> /etc/apt/sources.list

# Install dependencies
RUN apt-get upgrade -y && \
    apt-get install -y build-essential cmake git libusb-dev libcurl4-gnutls-dev cxxtest && \
    apt-get clean

#
# Install ChampyLight
#

# Clone the git repository and checkout to the current branch
ARG GIT_CHECKOUT_TO
RUN echo "==> "$GIT_CHECKOUT_TO
RUN dir=/root/champylight       && git clone -b "$GIT_CHECKOUT_TO" https://github.com/louisblin/ChampyLight.git $dir
RUN bld=/root/champylight/build && rm -rf $bld && mkdir $bld && cd $bld && cmake ../ && make
