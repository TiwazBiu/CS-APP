FROM ubuntu:latest

# necessary packages
RUN apt-get update && apt-get install -y \
    zsh \
    vim \
    build-essential \
    gcc-multilib \
    gdb \
    clang \
    valgrind \
    git-core \
    inetutils-ping \
    net-tools \
    man  && \
    apt-get clean && \
    rm -rf /var/lib/apt/lists/* /tmp/* /var/tmp/*

# install oh-my-zsh
RUN git clone https://github.com/robbyrussell/oh-my-zsh.git ~/.oh-my-zsh \
    && cp ~/.oh-my-zsh/templates/zshrc.zsh-template ~/.zshrc \
    && chsh -s /bin/zsh

# config vim
COPY supertab.vmb /root/.vim/supertab.vmb
COPY vimrc /root/.vimrc
RUN vim -S /root/.vim/supertab.vmb -c ':q!'  2>&1


# Set the locale
RUN apt-get update && apt-get install -y locales
RUN sed -i -e 's/# en_US.UTF-8 UTF-8/en_US.UTF-8 UTF-8/' /etc/locale.gen && \
    locale-gen
ENV LANG en_US.UTF-8  
ENV LANGUAGE en_US:en  
ENV LC_ALL en_US.UTF-8

# config  zsh
COPY zshrc /root/.zshrc
COPY zsh_history /root/.zsh_history

# git config
RUN git config --global user.email "zhiwenxie1900@outlook.com" && \
    git config --global user.name "xiezhiwen" && \
    git config --global push.default simple && \
    git config --global credential.helper 'cache --timeout=7200'

ENV SHELL /bin/zsh

