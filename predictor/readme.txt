BFS_LocalBP_8K_stat.txt
build/X86/gem5.opt --outdir=assignment3/1 configs/example/se.py --cpu-type=DerivO3CPU --caches --l2cache --bp-type=Local8KBP -c benchmark/bin/BFS -o "benchmark/inputs/R10k.graph"
BFS_LocalBP_32K_stat.txt
build/X86/gem5.opt --outdir=assignment3/2 configs/example/se.py --cpu-type=DerivO3CPU --caches --l2cache --bp-type=Local32KBP -c benchmark/bin/BFS -o "benchmark/inputs/R10k.graph"
MST_LocalBP_8K_stat.txt
build/X86/gem5.opt --outdir=assignment3/3 configs/example/se.py --cpu-type=DerivO3CPU --caches --l2cache --bp-type=Local8KBP -c benchmark/bin/MST -o "benchmark/inputs/rand-weighted-small.graph"
MST_LocalBP_32K_stat.txt
build/X86/gem5.opt --outdir=assignment3/4 configs/example/se.py --cpu-type=DerivO3CPU --caches --l2cache --bp-type=Local32KBP -c benchmark/bin/MST -o "benchmark/inputs/rand-weighted-small.graph"
queens_LocalBP_8K_stat.txt
build/X86/gem5.opt --outdir=assignment3/5 configs/example/se.py --cpu-type=DerivO3CPU --caches --l2cache --bp-type=Local8KBP -c benchmark/bin/queens -o "-c 11"
queens_LocalBP_32K_stat.txt
build/X86/gem5.opt --outdir=assignment3/6 configs/example/se.py --cpu-type=DerivO3CPU --caches --l2cache --bp-type=Local32KBP -c benchmark/bin/queens -o "-c 11"
BFS_TournamentBP_8K_stat.txt
build/X86/gem5.opt --outdir=assignment3/7 configs/example/se.py --cpu-type=DerivO3CPU --caches --l2cache --bp-type=Tournament8KBP -c benchmark/bin/BFS -o "benchmark/inputs/R10k.graph"
BFS_TournamentBP_32K_stat.txt
build/X86/gem5.opt --outdir=assignment3/8 configs/example/se.py --cpu-type=DerivO3CPU --caches --l2cache --bp-type=Tournament32KBP -c benchmark/bin/BFS -o "benchmark/inputs/R10k.graph"
MST_TournamentBP_8K_stat.txt
build/X86/gem5.opt --outdir=assignment3/9 configs/example/se.py --cpu-type=DerivO3CPU --caches --l2cache --bp-type=Tournament8KBP -c benchmark/bin/MST -o "benchmark/inputs/rand-weighted-small.graph"
MST_TournamentBP_32K_stat.txt
build/X86/gem5.opt --outdir=assignment3/10 configs/example/se.py --cpu-type=DerivO3CPU --caches --l2cache --bp-type=Tournament32KBP -c benchmark/bin/MST -o "benchmark/inputs/rand-weighted-small.graph"
queens_TournamentBP_8K_stat.txt
build/X86/gem5.opt --outdir=assignment3/11 configs/example/se.py --cpu-type=DerivO3CPU --caches --l2cache --bp-type=Tournament8KBP -c benchmark/bin/queens -o "-c 11"
queens_TournamentBP_32K_stat.txt
build/X86/gem5.opt --outdir=assignment3/12 configs/example/se.py --cpu-type=DerivO3CPU --caches --l2cache --bp-type=Tournament32KBP -c benchmark/bin/queens -o "-c 11"
BFS_gDACBP_8K_stat.txt
build/X86/gem5.opt --outdir=assignment3/13 configs/example/se.py --cpu-type=DerivO3CPU --caches --l2cache --bp-type=gDAC8KBP -c benchmark/bin/BFS -o "benchmark/inputs/R10k.graph"
BFS_gDACBP_32K_stat.txt
build/X86/gem5.opt --outdir=assignment3/14 configs/example/se.py --cpu-type=DerivO3CPU --caches --l2cache --bp-type=gDAC32KBP -c benchmark/bin/BFS -o "benchmark/inputs/R10k.graph"
MST_gDACBP_8K_stat.txt
build/X86/gem5.opt --outdir=assignment3/15 configs/example/se.py --cpu-type=DerivO3CPU --caches --l2cache --bp-type=gDAC8KBP -c benchmark/bin/MST -o "benchmark/inputs/rand-weighted-small.graph"
MST_gDACBP_32K_stat.txt
build/X86/gem5.opt --outdir=assignment3/16 configs/example/se.py --cpu-type=DerivO3CPU --caches --l2cache --bp-type=gDAC32KBP -c benchmark/bin/MST -o "benchmark/inputs/rand-weighted-small.graph"
queens_gDACBP_8K_stat.txt
build/X86/gem5.opt --outdir=assignment3/17 configs/example/se.py --cpu-type=DerivO3CPU --caches --l2cache --bp-type=gDAC8KBP -c benchmark/bin/queens -o "-c 11"
queens_gDACBP_32K_stat.txt
build/X86/gem5.opt --outdir=assignment3/18 configs/example/se.py --cpu-type=DerivO3CPU --caches --l2cache --bp-type=gDAC32KBP -c benchmark/bin/queens -o "-c 11"
