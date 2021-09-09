				# "main.cpp",
				# "hiredis/alloc.c",
				# "hiredis/async.c",
				# "hiredis/hiredis.c",
				# "hiredis/net.c",
				# "hiredis/read.c",
				# "hiredis/sds.c",
				# "hiredis/sockcompat.c",
				# "hiredis_cluster/adlist.c",
				# "hiredis_cluster/command.c",
				# "hiredis_cluster/crc16.c",
				# "hiredis_cluster/dict.c",
				# "hiredis_cluster/hiarray.c",
				# "hiredis_cluster/hircluster.c",
				# "hiredis_cluster/hiutil.c",

#!/bin/bash
set -e
g++ -o redis main.cpp redis.cpp  hiredis/alloc.c hiredis/async.c hiredis/hiredis.c hiredis/net.c hiredis/read.c hiredis/sds.c hiredis/sockcompat.c hiredis_cluster/adlist.c hiredis_cluster/command.c hiredis_cluster/crc16.c hiredis_cluster/dict.c hiredis_cluster/hiarray.c hiredis_cluster/hircluster.c hiredis_cluster/hiutil.c -I ./ -fpermissive -w -std=c++11
# ./redis