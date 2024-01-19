#!/bin/bash

S21_GREP="./s21_grep"

compare_output() {
    local flags=$1
    local pattern=$2
    local file=$3

    grep $flags "$pattern" "$file" > original_output.txt
    $S21_GREP $flags "$pattern" "$file" > custom_output.txt

    # Сравниваем результаты обеих программ
    if diff -u original_output.txt custom_output.txt > /dev/null; then
        echo "Тест [Флаги: \"$flags\" Шаблон: \"$pattern\" Файл: \"$file\"]: Успешно"
    else
        echo "Тест [Флаги: \"$flags\" Шаблон: \"$pattern\" Файл: \"$file\"]: Ошибка"
        echo "Различия:"
        diff -u original_output.txt custom_output.txt
    fi

    rm original_output.txt custom_output.txt
}

compare_output_f() {
    local flags=$1
    local file_pattern=$2
    local file=$3

    grep $flags "$file_pattern" "$file" > original_output.txt
    $S21_GREP $flags "$file_pattern" "$file" > custom_output.txt

    # Сравниваем результаты обеих программ
    if diff -u original_output.txt custom_output.txt > /dev/null; then
        echo "Тест [Флаги: \"$flags\" Шаблон из файла: \"$file_pattern\" Файл: \"$file\"]: Успешно"
    else
        echo "Тест [Флаги: \"$flags\" Шаблон из файла: \"$file_pattern\" Файл: \"$file\"]: Ошибка"
        echo "Различия:"
        diff -u original_output.txt custom_output.txt
    fi

    rm original_output.txt custom_output.txt
}


compare_output_e() {
    local flags=$1
    local pattern=$2
    local file=$3

    grep $flags "$file_pattern" "$file" > original_output.txt
    $S21_GREP $flags "$file_pattern" "$file" > custom_output.txt

    # Сравниваем результаты обеих программ
    if diff -u original_output.txt custom_output.txt > /dev/null; then
        echo "Тест [Флаги: \"$flags\" Шаблон из файла: \"$file_pattern\" Файл: \"$file\"]: Успешно"
    else
        echo "Тест [Флаги: \"$flags\" Шаблон из файла: \"$file_pattern\" Файл: \"$file\"]: Ошибка"
        echo "Различия:"
        diff -u original_output.txt custom_output.txt
    fi

    rm original_output.txt custom_output.txt
}

compare_multiple_patterns() {
    local file=$1
    shift
    local patterns=("$@")
    local grep_flags=""
    local s21_grep_flags=""

    for pat in "${patterns[@]}"; do
        grep_flags+=" -e $pat"
        s21_grep_flags+=" -e $pat"
    done

    grep $grep_flags "$file" > original_output.txt
    $S21_GREP $s21_grep_flags "$file" > custom_output.txt

    if diff -u original_output.txt custom_output.txt > /dev/null; then
        echo "Тест [Флаги: \"$s21_grep_flags\"  Шаблоны: \"${patterns[@]}\" Файл: \"$file\"]: Успешно"
    else
        echo "Тест [Флаги: \"$s21_grep_flags\"  Шаблоны: \"${patterns[@]}\" Файл: \"$file\"]: Ошибка"
        diff -u original_output.txt custom_output.txt
    fi

    rm original_output.txt custom_output.txt
}


files_to_test=(
    "test_files/test_1_grep.txt"
    "test_files/test_2_grep.txt"
    "test_files/test_3_grep.txt"


)

patterns_to_test=(
    "lcs"
    "test"
    "pattern"
    "king with no subjects"
)

file_patterns_to_test=(
  "test_files/pattern_1_grep.txt"
  "test_files/pattern_2_grep.txt"
  )

flags_to_test=(
    ""
    "-e"
    "-i"
    "-v"
    "-c"
    "-l"
    "-n"
    "-h"
    "-s"
    "-f"
    "-o"
    "-iv"
    "-in"
    "-nv"
    "-ic"

    # Добавьте другие флаги, которые вы хотите протестировать
)

echo "AUTO TESTS"
echo "---------------------------------------------------------"


for file in "${files_to_test[@]}"; do
    for flags in "${flags_to_test[@]}"; do
        if [[ $flags == "-f" ]]; then
            for file_pattern in "${file_patterns_to_test[@]}"; do
                if [[ -f $file ]]; then
                    compare_output_f "$flags" "$file_pattern" "$file"
                else
                    echo "Файл не найден: $file"
                fi
            done
        else
            for pattern in "${patterns_to_test[@]}"; do
                if [[ -f $file ]]; then
                    compare_output "$flags" "$pattern" "$file"
                else
                    echo "Файл не найден: $file"
                fi
            done
        fi

        # Добавляем вызов compare_multiple_patterns
        if [[ $flags == "-e" ]]; then
            if [[ -f $file ]]; then
                compare_multiple_patterns "$file" "${patterns_to_test[@]}"
            else
                echo "Файл не найден: $file"
            fi
        fi
        echo "----------------------------------------------------------------------------------"
    done
    echo "--------------------------------------------------------------------------------------"
done

echo "Тестирование завершено."
