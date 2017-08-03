#!/usr/bin/env bats

load ../test_helper

#####################################################
PATH_EXPECTED_OUTPUT_001="../exemplo.output.txt"

PATH_CC_TESTSFILES="."
DIR_CC_001="${PATH_CC_TESTSFILES}/001_*.cc"
DIR_EXE_001="${DIR_CC_001/%.cc/.exe}"

ALL_EXECUTABLE="*.exe*"
#####################################################



##================== TESTANDO AMBIENTE ==================##

@test "[001_compilar] Compilar códigos (independentes) {${DIR_CC_001}}" {
  for file in ${DIR_CC_001}
  do
    comando="$(awk -F: '$2 ~ /^compile/ { print $3 ; exit 0 }' ${file})"
    [[ $comando ]] && run bash -c "$comando"
    assert_success
  done
}


# @test "[TC002] Compilar Código '$CODE_EXERCICIO1'" {
#   executable_exists "$EXEC_EXERCICIO1" && skip "'$CODE_EXERCICIO1' já compilado!"
#   run g++ -std=c++11 -I../../lib $CODE_EXERCICIO1 -o $EXEC_EXERCICIO1 -lpthread -D MILLISECONDS -D DEBUG
#   assert_success
# }


##================== TESTANDO EXECUTÁVEL ==================##

@test "[001_testar_1] Comparar saída de '001_1.exe' com '$PATH_EXPECTED_OUTPUT_001'" {
  diff -w "$PATH_EXPECTED_OUTPUT_001" <(./001_1.exe)
  assert_success
}

@test "[001_testar_2] Comparar saída de '001_2.exe' com '$PATH_EXPECTED_OUTPUT_001'" {
  diff -w "$PATH_EXPECTED_OUTPUT_001" <(./001_2.exe)
  assert_success
}

@test "[001_testar_3] Comparar saída de '001_3.exe' com '$PATH_EXPECTED_OUTPUT_001'" {
  diff -w "$PATH_EXPECTED_OUTPUT_001" <(./001_3.exe)
  assert_success
}


# @test "[xxx] testar todos os 001" {
  # for executable in ${DIR_EXE_001}
  # do
    # diff -w "saida" <(./$executable)
    # assert_success
  # done
# }


##================== FIM DOS TESTES ==================##

@test "[END] Apagar Arquivos Gerados" {
  rm -f $ALL_EXECUTABLE
  assert_success
}
