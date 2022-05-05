FROM ubuntu:20.04

ENV TZ=Asia/Dubai
RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone

RUN apt update
RUN apt install -y tzdata

RUN apt-get --no-install-recommends -y install \
        build-essential gcc git make flex bison \
        software-properties-common curl python \
        bin86 gdb bcc liblzma-dev python-dev gettext iasl \
        uuid-dev libncurses5-dev libncursesw5-dev pkg-config \
        libgtk2.0-dev libyajl-dev sudo time default-jdk

ADD uclid.tar.gz /

VOLUME /uclid_artefact

ENV PATH="/uclid_artefact/bin:/uclid_artefact/uclid-0.9.5/bin:/uclid_artefact/uclid/z3/bin:${PATH}"
ENV LD_LIBRARY_PATH="/uclid_artefact/uclid/z3/bin:${PATH}"
WORKDIR uclid_artefact
