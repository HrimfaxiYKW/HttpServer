#!/bin/bash
SOURCE_DIR=`pwd`
BUILD_DIR=$SOURCE_DIR/build
BUILD_CLIENT_OPTION=false
BUILD_SERVER_OPTION=false

NUM_CPU_CORES=`cat /proc/cpuinfo | grep processor -c`

TEMP=`getopt -o csa --long build-client,build-server,build-all -n 'example.bash' -- "$@"`
if [ $? != 0 ] ; then echo "Terminating..." >&2 ; exit 1 ; fi
eval set -- "$TEMP"

while true ; do
  #echo "$1"
  case "$1" in
    -c|--build-client) echo "Build client."; BUILD_CLIENT_OPTION=true; shift ;;
    -s|--build-server) echo "Build server."; BUILD_SERVER_OPTION=true; shift ;;
    -a|--build-all) echo "Build client and server."; BUILD_SERVER_OPTION=true; BUILD_CLIENT_OPTION=true;shift ;;
    --) shift; break ;;
    *) echo "Internale error!"; exit 1 ;;
  esac
done

if [ -d $BUILD_DIR ]
then
  rm -rf $BUILD_DIR
fi

echo "Start compiling and linking."
mkdir -p $BUILD_DIR/ \
  && cd $BUILD_DIR/

CMAKE_OPTIONS=""
if [ "$BUILD_CLIENT_OPTION" = true ]
then
  echo "Real build client."
  CMAKE_OPTIONS="${CMAKE_OPTIONS} -DBUILD_CLIENT=true"
fi

if [ "$BUILD_SERVER_OPTION" = true ]
then
  echo "Real build server."
  CMAKE_OPTIONS="${CMAKE_OPTIONS} -DBUILD_SERVER=true"
fi

cmake ${SOURCE_DIR} ${CMAKE_OPTIONS}
# 多核编
make -j${NUM_CPU_CORES}

if [ $? != 0 ]
then
  echo "Fail to compile httpserver"
  exit 1
fi

echo "${SOURCE_DIR} ${CMAKE_OPTIONS}"
echo "Build finished"
exit 0
