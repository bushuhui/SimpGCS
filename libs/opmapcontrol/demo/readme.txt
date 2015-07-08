source /opt/qt-5.4.0/setenv.sh
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:../lib

qmake demo.pro
make

