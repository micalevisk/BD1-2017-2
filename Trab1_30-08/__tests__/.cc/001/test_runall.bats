#!/usr/bin/env bats

load ../../test_helper

#####################################################
PATH_EXPECTED_OUTPUT="exemplo.output.txt"

PATH_CC_TESTSFILES="."
DIR_CC_001="${PATH_CC_TESTSFILES}/001_*.cc"

ALL_EXECUTABLE="*.exe*"
#####################################################



##================== TESTANDO AMBIENTE ==================##

@test "[001_compilar] Compilar versões {${DIR_CC_001}}" {
  for file in ${DIR_CC_001}
  do
    comando="$(awk -F: '$2 ~ /^compile/ { print $3 ; exit 0 }' ${file})"
    [[ $comando ]] && run bash -c "$comando"
    assert_success
  done
}


##================== TESTANDO EXECUTÁVEL ==================##

@test "[001_testar_v1] Comparar saída de '001_1.exe' com '$PATH_EXPECTED_OUTPUT'" {
  executable_exists "001_1.exe" || skip "'001_1.exe' não encontrado!"
  diff -w "$PATH_EXPECTED_OUTPUT" <(./001_1.exe)
  assert_success
}

@test "[001_testar_v2] Comparar saída de '001_2.exe' com '$PATH_EXPECTED_OUTPUT'" {
  executable_exists "001_2.exe" || skip "'001_2.exe' não encontrado!"
  diff -w "$PATH_EXPECTED_OUTPUT" <(./001_2.exe)
  assert_success
}

@test "[001_testar_v3] Comparar saída de '001_3.exe' com '$PATH_EXPECTED_OUTPUT'" {
  executable_exists "001_3.exe" || skip "'001_3.exe' não encontrado!"
  diff -w "$PATH_EXPECTED_OUTPUT" <(./001_3.exe)
  assert_success
}


##================== FIM DOS TESTES ==================##

@test "[END] Apagar Arquivos Gerados" {
  rm $ALL_EXECUTABLE
  assert_success
}
