#!/bin/bash

echo 
echo "Running gem5"


./Gem5/gem5/build/X86/gem5.opt \
    Gem5/gem5/configs/deprecated/example/se.py \
    --cpu-type=TimingSimpleCPU --cpu-clock=400MHz \
	--mem-size="32MB" --mem-type=SimpleMemory \
	--caches --l1d_size=32kB --l1i_size=32kB \
    --cmd=./TestedOBCAppX86 \
