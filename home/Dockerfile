FROM ubuntu:jammy

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && \
    apt-get install -y \
      sudo tmux emacs git gdb cmake build-essential net-tools psmisc \
      openssh-server iputils-ping libssl-dev zlib1g-dev \
      coccinelle libcurl4-openssl-dev libexpat-dev tcl gettext autoconf

# Create a non-root user account
RUN adduser git --disabled-password

COPY ./ /home/git/
RUN chown -R git:git /home/git

# Switch over to the 'git' user to download and build the code
USER git
WORKDIR /home/git

RUN git config --global user.email "ctf@github.com"
RUN git config --global user.name "GitHub Security Lab"

RUN git clone https://github.com/git/git git-local
RUN git clone --mirror https://github.com/git/git git.git

# Create new tag and build the code
RUN cd git-local && \
    git checkout v2.34.8 && \
    git apply ~/patch.txt && \
    cp ~/flag1.txt . && \
    cp ~/Dockerfile . && \
    git add . && \
    GIT_AUTHOR_DATE=2023-07-17T23:05:17+01:00 GIT_COMMITTER_DATE=2023-07-17T23:05:17+01:00 git commit -s -m "Git 2.34.9" && \
    git tag -a v2.34.9 -m "Git 2.34.9" && \
    cp ~/flag2.txt . && \
    git add . && \
    git commit -m "Add second flag" && \
    git tag -a secondflag -m "Second flag" && \
    git log -1 --format=%H > ~/flagref && \
    git checkout v2.34.9 && \
    git tag -d secondflag && \
    make configure && \
    ./configure && \
    make -j${nproc} && \
    cp -r contrib/git-shell-commands ~

# Update mirror
RUN cd git.git && \
    git remote add local ~/git-local && \
    git fetch local --tags && \
    git remote remove local && \
    git config --global uploadpack.allowanysha1inwant 1

USER root
RUN cd /home/git/git-local && make install

USER git
