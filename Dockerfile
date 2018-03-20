FROM ubuntu:latest

# optmize
RUN apt-get update && apt-get install -y \
    zsh \
    vim \
    build-essential \
    gcc-multilib \
    git-core && rm -rf /var/lib/apt/lists/*
RUN git clone https://github.com/robbyrussell/oh-my-zsh.git ~/.oh-my-zsh \
    && cp ~/.oh-my-zsh/templates/zshrc.zsh-template ~/.zshrc \
    && chsh -s /bin/zsh

ENV SHELL /bin/zsh

CMD ["zsh", "--version"]
