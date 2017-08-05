#!/usr/bin/env bats

load ../../test_helper

#####################################################
PATH_EXPECTED_OUTPUT="make.output.txt"
DIR_EXEC1="main1.exe"
DIR_EXEC2="main2.exe"
DIR_OBJECTS="objects"
#####################################################


##================== TESTANDO MAKEFILE 1 ==================##

@test "[002_testar_make_v1] Executar o comando 'make'" {
  comando=$(awk -F: '$2 ~ /make/ { print $3 ; exit 0 ; }' 002_1.make)
  [[ $comando ]] && run bash -c "$comando"
  assert_equal "g++ -Wall main1.cc -o main1.exe -lm" "${lines[0]}"
  assert_equal "g++ -Wall main2.cc -o main2.exe -lm" "${lines[1]}"
}

@test "[002_testar_main1_v1] Verificar saída de '${DIR_EXEC1}'" {
  executable_exists "${DIR_EXEC1}" || skip "'${DIR_EXEC1}' não encontrado!"
  run ./"$DIR_EXEC1"
  assert_equal "saída do arquivo main1.cc" "${output}"
}

@test "[002_testar_main2_v1] Verificar saída de '${DIR_EXEC2}'" {
  executable_exists "${DIR_EXEC2}" || skip "'${DIR_EXEC2}' não encontrado!"
  run ./"$DIR_EXEC2"
  assert_equal "saída do arquivo main2.cc" "${output}"
}

@test "[002_testar_clean_v1] Executar o comando 'make clean'" {
  comando=$(awk -F: '$2 ~ /make-clean/ { print $3 ; exit 0 ; }' 002_1.make)
  [[ $comando ]] && run bash -c "$comando"
  assert_equal "rm -rf main1.exe main2.exe" "${output}"
}

##================== TESTANDO MAKEFILE 2 ==================##

@test "[002_testar_make_v2] Executar o comando 'make'" {
  comando=$(awk -F: '$2 ~ /make/ { print $3 ; exit 0 ; }' 002_2.make)
  [[ $comando ]] && run bash -c "$comando"
  assert_equal "mkdir -p objects" "${lines[0]}"
  assert_equal "cc -c -o objects/main1.o -c main1.cc" "${lines[1]}"
  assert_equal "g++ -Wall objects/main1.o -o main1.exe -lm" "${lines[2]}"
  assert_equal "mkdir -p objects" "${lines[3]}"
  assert_equal "cc -c -o objects/main2.o -c main2.cc" "${lines[4]}"
  assert_equal "g++ -Wall objects/main2.o -o main2.exe -lm" "${lines[5]}"
}

@test "[002_testar_main1_v2] Verificar saída de '${DIR_EXEC1}'" {
  executable_exists "${DIR_EXEC1}" || skip "'${DIR_EXEC1}' não encontrado!"
  run ./"$DIR_EXEC1"
  assert_equal "saída do arquivo main1.cc" "${output}"
}

@test "[002_testar_main2_v2] Verificar saída de '${DIR_EXEC2}'" {
  executable_exists "${DIR_EXEC2}" || skip "'${DIR_EXEC2}' não encontrado!"
  run ./"$DIR_EXEC2"
  assert_equal "saída do arquivo main2.cc" "${output}"
}

@test "[002_testar_clean_v2] Executar o comando 'make clean'" {
  comando=$(awk -F: '$2 ~ /make-clean/ { print $3 ; exit 0 ; }' 002_2.make)
  [[ $comando ]] && run bash -c "$comando"
  assert_equal "rm -rf main1.exe main2.exe objects" "${output}"
}
