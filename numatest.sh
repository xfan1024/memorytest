#!/bin/sh

set -e

node_count=`lscpu | fgrep 'NUMA node(s)' | grep -Eo '[0-9]+$'`
if [ -z "$node_count" ]; then
    echo "Error: cannot get NUMA node count"
    exit 1
fi

if [ -z "$*" ]; then
    echo "Usage: $0 <command>"
    exit 1
fi

for run_node in `seq 0 $((node_count-1))`; do
    for mem_node in `seq 0 $((node_count-1))`; do
        echo "Run on node $run_node, allocate on node $mem_node"
        numactl --cpunodebind=$run_node --membind=$mem_node "$@"
    done
done
