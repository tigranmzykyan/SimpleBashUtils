#!/bin/bash


S21_CAT="./s21_cat"


compare_output() {
    local flags=$1
    local file=$2


    cat $flags "$file" > original_output.txt
    $S21_CAT $flags "$file" > custom_output.txt

    # Сравниваем результаты обеих программ
    if diff -u original_output.txt custom_output.txt > /dev/null; then
        echo "Тест [Флаги: $flags, Файл: $file]: Успешно"
    else
        echo "Тест [Флаги: $flags, Файл: $file]: Ошибка"
        echo "Различия:"
        diff -u original_output.txt custom_output.txt
    fi


    rm original_output.txt custom_output.txt
}

files_to_test=(
    "test_files/test_1_cat.txt"
    "test_files/test_2_cat.txt"
    "test_files/test_3_cat.txt"
    "test_files/test_4_cat.txt"
    "test_files/test_5_cat.txt"
    "test_files/test_case_cat.txt"

)

flags_to_test=(
    ""
    "-b"
    "-n"
    "-s"
    "-e"
    "-t"
    "-v"
)


echo "AUTO TESTS"
echo "---------------------------------------------------------"

for file in "${files_to_test[@]}"; do
    for flags in "${flags_to_test[@]}"; do
        if [[ -f $file ]]; then
            compare_output "$flags" "$file"
        else
            echo "Файл не найден: $file"
        fi
    done
    echo "---------------------------------------------------------"
done

echo "Тестирование завершено."
