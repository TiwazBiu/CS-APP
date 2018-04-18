FROM ubuntu:latest

# optmize
RUN apt-get update && apt-get install -y \
    zsh \
    vim \
    build-essential \
    gcc-multilib \
    gdb \
    man \
    inetutils-ping \
    net-tools \
    git-core && rm -rf /var/lib/apt/lists/*
    && cp /usr/share/vim/vim80/vimrc_example.vim  ~/.vimrc
# config .vimrc
RUN echo "set number" >> /root/.vimrc \
    && echo "set relativenumber" >> /root/.vimrc

# install oh-my-zsh
RUN git clone https://github.com/robbyrussell/oh-my-zsh.git ~/.oh-my-zsh \
    && cp ~/.oh-my-zsh/templates/zshrc.zsh-template ~/.zshrc \
    && chsh -s /bin/zsh



ENV SHELL /bin/zsh

