#!/bin/bash

salloc --account=def-tembelym --time=02:00:00 --ntasks=1 --cpus-per-task=1 --mem=1G \
srun --ntasks=1 --cpus-per-task=1 --mem=1G --pty bash -c '
    module load paraview
    pvserver --force-offscreen-rendering
'