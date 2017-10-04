-- 1. criação de uma tabela sem restrições
-- 2. inserção de valores sem causar duplicatas
-- 3. inserção das restrições

-- criando tabela sem restrições
CREATE TABLE IF NOT EXISTS cidade (
  id SERIAL, -- PRIMARY KEY,
  nome CHARACTER VARYING(100), -- NOT NULL,
  uf CHARACTER VARYING(2) -- NOT NULL
  -- UNIQUE (nome, uf)
);


-- inserção insegura:
INSERT INTO cidade (id, nome, uf)
VALUES (DEFAULT, 'Manaus', 'AM');

-- inserção segura:
-- admitindo que a tupla (nome, uf) será uma chave única (ao inserir as restrições)
INSERT INTO cidade (nome, uf)
SELECT 'Manaus', 'AM'
WHERE NOT EXISTS (
  SELECT id FROM cidade
  WHERE
    nome = 'Manaus' AND uf = 'AM'
  );


-- adicionando restrições (caso alguma tupla viole, causará erro)
ALTER TABLE cidade
  ADD CONSTRAINT cidade_pkey PRIMARY KEY (id),
  ADD CONSTRAINT cidade_nome_uf_key UNIQUE (nome, uf),
  ALTER COLUMN nome SET NOT NULL,
  ALTER COLUMN uf SET NOT NULL;
