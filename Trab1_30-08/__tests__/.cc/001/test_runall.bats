#!/usr/bin/env bats

load ../../test_helper

#####################################################
PATH_EXPECTED_OUTPUT="exemplo.output.txt"
PATH_INPUT="../exemplo.input.csv"
DIR_CC_001="./001_*.cc"
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
  skip "~ não consegue ler CSV mal formatado {deprecated}"
  executable_exists "001_1.exe" || skip "'001_1.exe' não encontrado!"
  diff -w "$PATH_EXPECTED_OUTPUT" <(./001_1.exe)
  assert_success
}

@test "[001_testar_v2] Comparar saída de '001_2.exe' com '$PATH_EXPECTED_OUTPUT'" {
  skip "~ não consegue ler CSV mal formatado {deprecated}"
  executable_exists "001_2.exe" || skip "'001_2.exe' não encontrado!"
  diff -w "$PATH_EXPECTED_OUTPUT" <(./001_2.exe)
  assert_success
}

@test "[001_testar_v3] Comparar saída de '001_3.exe' com '$PATH_EXPECTED_OUTPUT'" {
  skip "~ não consegue ler CSV mal formatado {deprecated}"
  executable_exists "001_3.exe" || skip "'001_3.exe' não encontrado!"
  diff -w "$PATH_EXPECTED_OUTPUT" <(./001_3.exe)
  assert_success
}

@test "[001_testar_v4] Comparar saída de '001_4.exe' com 'leu 6 registros'" {
  executable_exists "001_4.exe" || skip "'001_4.exe' não encontrado!"
  run ./001_4.exe "$PATH_INPUT"
  assert_equal "leu 7 registros" "${output}"
}

@test "[001_testar_v5] Comparar saída de '001_5.exe' com '$PATH_EXPECTED_OUTPUT'" {
  executable_exists "001_5.exe" || skip "'001_5.exe' não encontrado!"
  diff -w "$PATH_EXPECTED_OUTPUT" <(./001_5.exe "$PATH_INPUT")
  assert_success
}

@test "[001_testar_v6] Verificar saída de '001_6.exe'" {
  executable_exists "001_6.exe" || skip "'001_6.exe' não encontrado!"
  run ./001_6.exe "$PATH_INPUT" "_saida.bin"
  assert_equal "Análise do arquivo:"             "${lines[0]}"

  rm -f "_saida.bin"
}


##================== FIM DOS TESTES ==================##

@test "[END] Apagar Arquivos Gerados" {
  rm $ALL_EXECUTABLE
  assert_success
}
