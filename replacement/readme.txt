
1: lru bfs rand 256kB
./build/X86/gem5.opt --outdir=tests configs/example/se.py --cpu-type=AtomicSimpleCPU --caches --l1i_size=32kB --l1d_size=32kB --l2cache --l2_size=256kB --l2_rpp="LRURP()" -c benchmark/bin/BFS_opt -o "benchmark_assign2/BFS_randLocalGraph_J_5_120000"
2: lru bfs rmat 256kB
./build/X86/gem5.opt --outdir=tests configs/example/se.py --cpu-type=AtomicSimpleCPU --caches --l1i_size=32kB --l1d_size=32kB --l2cache --l2_size=256kB --l2_rpp="LRURP()" -c benchmark/bin/BFS_opt -o "benchmark_assign2/BFS_rMatGraph_J_5_90000"
3: lru mst rand 256kB
./build/X86/gem5.opt --outdir=tests configs/example/se.py --cpu-type=AtomicSimpleCPU --caches --l1i_size=32kB --l1d_size=32kB --l2cache --l2_size=256kB --l2_rpp="LRURP()" -c benchmark/bin/MST_opt -o "benchmark_assign2/MST_randLocalGraph_WE_5_100000"
4: lru mst rmat 256kB 
./build/X86/gem5.opt --outdir=tests configs/example/se.py --cpu-type=AtomicSimpleCPU --caches --l1i_size=32kB --l1d_size=32kB --l2cache --l2_size=256kB --l2_rpp="LRURP()" -c benchmark/bin/MST_opt -o "benchmark_assign2/MST_rMatGraph_WE_5_35000"
5: lru bfs rand 1MB
./build/X86/gem5.opt --outdir=tests configs/example/se.py --cpu-type=AtomicSimpleCPU --caches --l1i_size=32kB --l1d_size=32kB --l2cache --l2_size=1MB --l2_rpp="LRURP()" -c benchmark/bin/BFS_opt -o "benchmark_assign2/BFS_randLocalGraph_J_5_120000"
6: lru bfs rmat 1MB
./build/X86/gem5.opt --outdir=tests configs/example/se.py --cpu-type=AtomicSimpleCPU --caches --l1i_size=32kB --l1d_size=32kB --l2cache --l2_size=1MB --l2_rpp="LRURP()" -c benchmark/bin/BFS_opt -o "benchmark_assign2/BFS_rMatGraph_J_5_90000"
7: lru mst rand 1MB
./build/X86/gem5.opt --outdir=tests configs/example/se.py --cpu-type=AtomicSimpleCPU --caches --l1i_size=32kB --l1d_size=32kB --l2cache --l2_size=1MB --l2_rpp="LRURP()" -c benchmark/bin/MST_opt -o "benchmark_assign2/MST_randLocalGraph_WE_5_100000"
8: lru mst rmat 1MB
./build/X86/gem5.opt --outdir=tests configs/example/se.py --cpu-type=AtomicSimpleCPU --caches --l1i_size=32kB --l1d_size=32kB --l2cache --l2_size=1MB --l2_rpp="LRURP()" -c benchmark/bin/MST_opt -o "benchmark_assign2/MST_rMatGraph_WE_5_35000"
9: lru bfs rand 4MB
./build/X86/gem5.opt --outdir=tests configs/example/se.py --cpu-type=AtomicSimpleCPU --caches --l1i_size=32kB --l1d_size=32kB --l2cache --l2_size=4MB --l2_rpp="LRURP()" -c benchmark/bin/BFS_opt -o "benchmark_assign2/BFS_randLocalGraph_J_5_120000"
10: lru bfs rmat 4MB
./build/X86/gem5.opt --outdir=tests configs/example/se.py --cpu-type=AtomicSimpleCPU --caches --l1i_size=32kB --l1d_size=32kB --l2cache --l2_size=4MB --l2_rpp="LRURP()" -c benchmark/bin/BFS_opt -o "benchmark_assign2/BFS_rMatGraph_J_5_90000"
11: lru mst rand 4MB
./build/X86/gem5.opt --outdir=tests configs/example/se.py --cpu-type=AtomicSimpleCPU --caches --l1i_size=32kB --l1d_size=32kB --l2cache --l2_size=4MB --l2_rpp="LRURP()" -c benchmark/bin/MST_opt -o "benchmark_assign2/MST_randLocalGraph_WE_5_100000"
12: lru mst rmat 4MB
./build/X86/gem5.opt --outdir=tests configs/example/se.py --cpu-type=AtomicSimpleCPU --caches --l1i_size=32kB --l1d_size=32kB --l2cache --l2_size=4MB --l2_rpp="LRURP()" -c benchmark/bin/MST_opt -o "benchmark_assign2/MST_rMatGraph_WE_5_35000"
13: srrip bfs rand 256kB
./build/X86/gem5.opt --outdir=tests configs/example/se.py --cpu-type=AtomicSimpleCPU --caches --l1i_size=32kB --l1d_size=32kB --l2cache --l2_size=256kB --l2_rpp="BRRIPRP()" -c benchmark/bin/BFS_opt -o "benchmark_assign2/BFS_randLocalGraph_J_5_120000"
14: srrip bfs rmat 256KB
./build/X86/gem5.opt --outdir=tests configs/example/se.py --cpu-type=AtomicSimpleCPU --caches --l1i_size=32kB --l1d_size=32kB --l2cache --l2_size=256kB --l2_rpp="BRRIPRP()" -c benchmark/bin/BFS_opt -o "benchmark_assign2/BFS_rMatGraph_J_5_90000"
15: srrip mst rand 256kB
./build/X86/gem5.opt --outdir=tests configs/example/se.py --cpu-type=AtomicSimpleCPU --caches --l1i_size=32kB --l1d_size=32kB --l2cache --l2_size=256kB --l2_rpp="BRRIPRP()" -c benchmark/bin/MST_opt -o "benchmark_assign2/MST_randLocalGraph_WE_5_100000"
16: srrip mst rmat 256kB
./build/X86/gem5.opt --outdir=tests configs/example/se.py --cpu-type=AtomicSimpleCPU --caches --l1i_size=32kB --l1d_size=32kB --l2cache --l2_size=256kB --l2_rpp="BRRIPRP()" -c benchmark/bin/MST_opt -o "benchmark_assign2/MST_rMatGraph_WE_5_35000"
17: srrip bfs rand 1MB
./build/X86/gem5.opt --outdir=tests configs/example/se.py --cpu-type=AtomicSimpleCPU --caches --l1i_size=32kB --l1d_size=32kB --l2cache --l2_size=1MB --l2_rpp="BRRIPRP()" -c benchmark/bin/BFS_opt -o "benchmark_assign2/BFS_randLocalGraph_J_5_120000"
18: srrip bfs rmat 1MB
./build/X86/gem5.opt --outdir=tests configs/example/se.py --cpu-type=AtomicSimpleCPU --caches --l1i_size=32kB --l1d_size=32kB --l2cache --l2_size=1MB --l2_rpp="BRRIPRP()" -c benchmark/bin/BFS_opt -o "benchmark_assign2/BFS_rMatGraph_J_5_90000"
19: srrip mst rand 1MB
./build/X86/gem5.opt --outdir=tests configs/example/se.py --cpu-type=AtomicSimpleCPU --caches --l1i_size=32kB --l1d_size=32kB --l2cache --l2_size=1MB --l2_rpp="BRRIPRP()" -c benchmark/bin/MST_opt -o "benchmark_assign2/MST_randLocalGraph_WE_5_100000"
20: srrip mst rmat 1MB
./build/X86/gem5.opt --outdir=tests configs/example/se.py --cpu-type=AtomicSimpleCPU --caches --l1i_size=32kB --l1d_size=32kB --l2cache --l2_size=1MB --l2_rpp="BRRIPRP()" -c benchmark/bin/MST_opt -o "benchmark_assign2/MST_rMatGraph_WE_5_35000"
21: srrip bfs rand 4MB
./build/X86/gem5.opt --outdir=tests configs/example/se.py --cpu-type=AtomicSimpleCPU --caches --l1i_size=32kB --l1d_size=32kB --l2cache --l2_size=4MB --l2_rpp="BRRIPRP()" -c benchmark/bin/BFS_opt -o "benchmark_assign2/BFS_randLocalGraph_J_5_120000"
22: srrip bfs rmat 4MB
./build/X86/gem5.opt --outdir=tests configs/example/se.py --cpu-type=AtomicSimpleCPU --caches --l1i_size=32kB --l1d_size=32kB --l2cache --l2_size=4MB --l2_rpp="BRRIPRP()" -c benchmark/bin/BFS_opt -o "benchmark_assign2/BFS_rMatGraph_J_5_90000"
23: srrip mst rand 4MB
./build/X86/gem5.opt --outdir=tests configs/example/se.py --cpu-type=AtomicSimpleCPU --caches --l1i_size=32kB --l1d_size=32kB --l2cache --l2_size=4MB --l2_rpp="BRRIPRP()" -c benchmark/bin/MST_opt -o "benchmark_assign2/MST_randLocalGraph_WE_5_100000"
24: srrip mst rmat 4MB
./build/X86/gem5.opt --outdir=tests configs/example/se.py --cpu-type=AtomicSimpleCPU --caches --l1i_size=32kB --l1d_size=32kB --l2cache --l2_size=4MB --l2_rpp="BRRIPRP()" -c benchmark/bin/MST_opt -o "benchmark_assign2/MST_rMatGraph_WE_5_35000"
25: ship bfs rand 256kB
./build/X86/gem5.opt --outdir=tests configs/example/se.py --cpu-type=AtomicSimpleCPU --caches --l1i_size=32kB --l1d_size=32kB --l2cache --l2_size=256kB --l2_rpp="SHiPRP()" -c benchmark/bin/BFS_opt -o "benchmark_assign2/BFS_randLocalGraph_J_5_120000"
26: ship bfs rmat 256kB
./build/X86/gem5.opt --outdir=tests configs/example/se.py --cpu-type=AtomicSimpleCPU --caches --l1i_size=32kB --l1d_size=32kB --l2cache --l2_size=256kB --l2_rpp="SHiPRP()" -c benchmark/bin/BFS_opt -o "benchmark_assign2/BFS_rMatGraph_J_5_90000"
27: ship mst rand 256kB
./build/X86/gem5.opt --outdir=tests configs/example/se.py --cpu-type=AtomicSimpleCPU --caches --l1i_size=32kB --l1d_size=32kB --l2cache --l2_size=256kB --l2_rpp="SHiPRP()" -c benchmark/bin/MST_opt -o "benchmark_assign2/MST_randLocalGraph_WE_5_100000"
28: ship mst rmat 256kB
./build/X86/gem5.opt --outdir=tests configs/example/se.py --cpu-type=AtomicSimpleCPU --caches --l1i_size=32kB --l1d_size=32kB --l2cache --l2_size=256kB --l2_rpp="SHiPRP()" -c benchmark/bin/MST_opt -o "benchmark_assign2/MST_rMatGraph_WE_5_35000"
29: ship bfs rand 1MB
./build/X86/gem5.opt --outdir=tests configs/example/se.py --cpu-type=AtomicSimpleCPU --caches --l1i_size=32kB --l1d_size=32kB --l2cache --l2_size=1MB --l2_rpp="SHiPRP()" -c benchmark/bin/BFS_opt -o "benchmark_assign2/BFS_randLocalGraph_J_5_120000"
30: ship bfs rmat 1MB
./build/X86/gem5.opt --outdir=tests configs/example/se.py --cpu-type=AtomicSimpleCPU --caches --l1i_size=32kB --l1d_size=32kB --l2cache --l2_size=1MB --l2_rpp="SHiPRP()" -c benchmark/bin/BFS_opt -o "benchmark_assign2/BFS_rMatGraph_J_5_90000"
31: ship mst rand 1MB
./build/X86/gem5.opt --outdir=tests configs/example/se.py --cpu-type=AtomicSimpleCPU --caches --l1i_size=32kB --l1d_size=32kB --l2cache --l2_size=1MB --l2_rpp="SHiPRP()" -c benchmark/bin/MST_opt -o "benchmark_assign2/MST_randLocalGraph_WE_5_100000"
32: ship mst rmat 1MB
./build/X86/gem5.opt --outdir=tests configs/example/se.py --cpu-type=AtomicSimpleCPU --caches --l1i_size=32kB --l1d_size=32kB --l2cache --l2_size=1MB --l2_rpp="SHiPRP()" -c benchmark/bin/MST_opt -o "benchmark_assign2/MST_rMatGraph_WE_5_35000"
33: ship bfs rand 4MB
./build/X86/gem5.opt --outdir=tests configs/example/se.py --cpu-type=AtomicSimpleCPU --caches --l1i_size=32kB --l1d_size=32kB --l2cache --l2_size=4MB --l2_rpp="SHiPRP()" -c benchmark/bin/BFS_opt -o "benchmark_assign2/BFS_randLocalGraph_J_5_120000"
34: ship bfs rmat 4MB
./build/X86/gem5.opt --outdir=tests configs/example/se.py --cpu-type=AtomicSimpleCPU --caches --l1i_size=32kB --l1d_size=32kB --l2cache --l2_size=4MB --l2_rpp="SHiPRP()" -c benchmark/bin/BFS_opt -o "benchmark_assign2/BFS_rMatGraph_J_5_90000"
35: ship mst rand 4MB
./build/X86/gem5.opt --outdir=tests configs/example/se.py --cpu-type=AtomicSimpleCPU --caches --l1i_size=32kB --l1d_size=32kB --l2cache --l2_size=4MB --l2_rpp="SHiPRP()" -c benchmark/bin/MST_opt -o "benchmark_assign2/MST_randLocalGraph_WE_5_100000"
36: ship mst rmat 4MB
./build/X86/gem5.opt --outdir=tests configs/example/se.py --cpu-type=AtomicSimpleCPU --caches --l1i_size=32kB --l1d_size=32kB --l2cache --l2_size=4MB --l2_rpp="SHiPRP()" -c benchmark/bin/MST_opt -o "benchmark_assign2/MST_rMatGraph_WE_5_35000"