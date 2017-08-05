#!/usr/bin/env bats

load ../../test_helper

#####################################################
PATH_EXPECTED_OUTPUT="make.output.txt"
DIR_CC_002="./002_*.cc"
DIR_OBJECTS="objects"
#####################################################


##================== TESTANDO MAKEFILES ==================##

@test "[002_testar_make_v1] Executar o comando 'make'" {
  comando=$(awk -F: '$2 ~ /make/ { print $3 ; exit 0 ; }' 002_1.make)
  [[ $comando ]] && run bash -c "$comando"
  assert_equal "g++ -Wall main1.cc -o main1.exe -lm" "${lines[0]}"
  assert_equal "g++ -Wall main2.cc -o main2.exe -lm" "${lines[1]}"
}

@test "[002_testar_clean_v1] Executar o comando 'make clean'" {
  comando=$(awk -F: '$2 ~ /make-clean/ { print $3 ; exit 0 ; }' 002_1.make)
  [[ $comando ]] && run bash -c "$comando"
  assert_equal "rm -rf main1.exe main2.exe" "${output}"
}

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

@test "[002_testar_clean_v2] Executar o comando 'make clean'" {
  comando=$(awk -F: '$2 ~ /make-clean/ { print $3 ; exit 0 ; }' 002_2.make)
  [[ $comando ]] && run bash -c "$comando"
  assert_equal "rm -rf main1.exe main2.exe objects" "${output}"
}
