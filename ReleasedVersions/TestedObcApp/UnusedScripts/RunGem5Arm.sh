#!/bin/bash

echo 
echo "Running gem5"


./Gem5/gem5/build/ARM/gem5.opt \
    Gem5/gem5/configs/example/arm/starter_se.py \
    --cpu="minor" --mem-size="2GB" \
    -- TestedOBCAppArm \
