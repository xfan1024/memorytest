#!/bin/sh

set -e

node_count=`lscpu | fgrep 'NUMA node(s)' | grep -Eo '[0-9]+$'`
if [ -z "$node_count" ]; then
    echo "Error: cannot get NUMA node count"
fi

for run_node in `seq 0 $((node_count-1))`; do
    for mem_node in `seq 0 $((node_count-1))`; do
        if [ $run_node -eq $mem_node ]; then
            echo "Run on node $run_node, allocate on node $mem_node"
            numactl --cpunodebind=$run_node --membind=$mem_node "$@"
        fi
    done
done
