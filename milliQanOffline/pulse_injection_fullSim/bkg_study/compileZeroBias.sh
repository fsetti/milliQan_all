#g++ -o mixEvents.exe mix_events.C /net/cms26/cms26r0/milliqan/milliDAQ/libMilliDAQ.so `root-config --cflags --glibs` -Wno-narrowing -fpermissive
g++ -o mixZeroBias_fullSim.exe process_zeroBias.C ./libMilliDAQ.so `root-config --cflags --glibs` -Wno-narrowing -fpermissive
