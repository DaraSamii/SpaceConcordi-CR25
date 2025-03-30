#!/bin/bash

salloc --account=def-tembelym --time=02:00:00 --ntasks=1 --cpus-per-task=1 --mem=12G \
srun --ntasks=1 --cpus-per-task=1 --mem=12G --pty bash -c '
    module purge
    module load paraview/5.13
    pvserver --force-offscreen-rendering
'