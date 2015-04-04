#!/bin/bash

success="TEST_SUCCESS"
had_failures="0"
tmp_file=".tmp_err_output"  # stderr of parser stored here


reduce_path_to_test_name () {
    local fullpath=$1
    local filename="${fullpath##*/}" # strip the preceding path
    test_name="${filename%.*}"       # strip the file type & set a global variable
}

run_test() {
    local should_fail=$1

    # stdout of parser
    $(g++ -std=c++11 $file &> $tmp_file)
    outcome=`cat $tmp_file`
    # empty if compiled, errors otherwise

    if [[ $should_fail && $outcome ]] ||  [[ ! $should_fail && ! $outcome ]]
    then
        echo "success: $test_name"
    else
        echo "FAIL:    $test_name"
        echo "$outcome"
        had_failures="1"
    fi
}

find tests -name *\.cpp

test_files=$(find tests -name "*.cpp")
echo "CHECKING LIBRARY ERRORS"
for file in $test_files
do
    reduce_path_to_test_name "$file"
    fail_test=$(echo "$file" |  grep "fail_")
    run_test "$fail_test"
done

rm -f "$tmp_file"
rm -f a.out
exit $had_failures
