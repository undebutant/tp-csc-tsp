#!/bin/bash

BIN="tube_4096 tube_416 tube_malloc tableau_semaphore cond file_messages file_messages_posix"

echo -n "" > results.log
for exec in $BIN ; do
    echo -en "$exec\t" |tee -a results.log
    ./$exec 2000000 2>&1 |tee -a results.log
done
