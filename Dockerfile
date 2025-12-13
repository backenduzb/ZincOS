FROM ubuntu:latest

RUN apt update && apt install -y \
    nasm \
    gcc-multilib \
    binutils \
    grub-common \
    grub-pc-bin \
    xorriso \
    make \
    cmake

CMD ["bash"]
