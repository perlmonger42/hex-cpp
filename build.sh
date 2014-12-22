export SRC_ROOT=`pwd`
rm -rf /tmp/hex-build \
  && mkdir /tmp/hex-build \
  && cd /tmp/hex-build \
  && echo SRC_ROOT: $SRC_ROOT \
  && cmake -Dtest=ON "$SRC_ROOT" \
  && make \
  && make test
