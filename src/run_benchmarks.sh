#!/bin/bash
success="TEST_SUCCESS"
had_failures="0"
tmp_file=".tmp_err_output"  # stderr of parser stored here

reduce_path_to_test_name () {
    local fullpath=$1
    local filename="${fullpath##*/}" # strip the preceding path
    test_name="${filename%.*}"       # strip the file type & set a global variable
}

run_bench() {
    # stdout of parser
    $(g++ -std=c++0x -O2 $file &> $tmp_file)
    outcome=`cat $tmp_file`
    # empty if compiled, errors otherwise

    if [[ ! $outcome ]]
    then
        $(./a.out &> $tmp_file)
        outcome=`cat $tmp_file`
        echo "$test_name: $outcome"
    else
        echo "$test_name: didn't compile"
        echo "$outcome"
    fi
}


if [ -z "$1" ] # if there's no argument0
then
    # run all benchmarks
    echo "running all benchmarks"
    bench_files=$(find benchmarks -name "*.cpp")
else
    # run whatever benchmarks match
    echo "running benchmarks matching '$1'"
    bench_files=$(find benchmarks -name "*.cpp" | grep "$1")
fi

echo "=========================="
for file in $bench_files
do
    reduce_path_to_test_name "$file"
    run_bench "$file"
done

rm -f "$tmp_file"
rm -f a.out
exit 0
