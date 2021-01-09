#g++ -o mixEvents.exe mix_events.C /net/cms26/cms26r0/milliqan/milliDAQ/libMilliDAQ.so `root-config --cflags --glibs` -Wno-narrowing -fpermissive
g++ -o mixEvents.exe mix_events.C ./libMilliDAQ.so `root-config --cflags --glibs` -Wno-narrowing -fpermissive
