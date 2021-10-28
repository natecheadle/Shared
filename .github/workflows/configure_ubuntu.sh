apt-get install -y \
build-essential tar zip unzip git wget \
ninja-build python c++filt xz-utils curl

wget -q https://github.com/llvm/llvm-project/releases/download/llvmorg-13.0.0/clang+llvm-13.0.0-x86_64-linux-gnu-ubuntu-20.04.tar.xz && \
tar -xf clang+llvm-13.0.0-x86_64-linux-gnu-ubuntu-20.04.tar.xz && \
mv clang+llvm-13.0.0-x86_64-linux-gnu-ubuntu-20.04 /usr/bin/llvm_13 && \
rm clang+llvm-13.0.0-x86_64-linux-gnu-ubuntu-20.04.tar.xz