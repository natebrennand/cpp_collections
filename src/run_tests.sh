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
    $(g++ -std=c++11 -pthread $file &> $tmp_file)
    outcome=`cat $tmp_file`
    # empty if compiled, errors otherwise

    if [[ ! $should_fail && ! $outcome ]]
    then
        $(./a.out &> /dev/null)
        if [ $? -ne 0 ]
        then
            had_failures="1"
            echo "FAIL:    $test_name"
        else
            echo "SUCCESS: $test_name"
        fi
    elif [[ $should_fail && $outcome ]]
    then
        echo "SUCCESS: $test_name"
    else
        echo "FAIL:    $test_name"
        echo "$outcome"
        had_failures="1"
    fi
}

if [ -z "$1" ] # if there's no argument0
then
    # run all tests
    echo "checking full library"
    test_files=$(find tests -name "*.cpp")
else
    # run whatever tests match
    echo "checking library fn's matching '$1'"
    test_files=$(find tests -name "*.cpp" | grep "$1")
fi

echo "=========================="
for file in $test_files
do
    reduce_path_to_test_name "$file"
    fail_test=$(echo "$file" |  grep "fail_")
    run_test "$fail_test"
done

rm -f "$tmp_file"
rm -f a.out
exit $had_failures
