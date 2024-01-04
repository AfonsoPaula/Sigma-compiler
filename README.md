<p align="center">
  <h1 align="center">Sigma Query Language - Compiler 🐉</h1>
</p>

## ➡️Contextualização

Este projeto encaixa-se no contexto académico da disciplina Linguagens de Programação e Compiladores (Mestrado em Engenharia Informática), onde o desafio lançado passa por desenvolver um compilador/interpretador com uma identidade única, indo além do convencional, de maneira a aplicar os conhecimentos adquiridos ao longo semestre.

A Sigma Query Language Compiler é um programa desenvolvido para interpretar comandos de uma linguagem semelhante à SQL, permitindo a manipulação de bases de dados e tabelas. Os objetivos primários deste projeto incluíram o desenvolvimento de um compilador capaz de interpretar comandos "sigma" fornecidos pelo utilizador, de modo a obter, como produto final, um simulador de um sistema de gestão de bases de dados em C. A implementação foi guiada pelo uso do Lexer, atuando como analisador léxico, e do Parser, funcionando como analisador semântico, com operações fundamentais implementadas em C.

<hr>

## ➡️Estrutura do Projeto

- A diretoria ```/Ast``` contém o ficheiro relacionado com a implementação da árvore sintática abstrata.
- ```lexer.l```: Especificação do analisador léxico usando a ferramenta Flex.
- ```parser.y```: Especificação do analisador sintático usando a ferramenta Bison.
- O ficheiro ```main.c``` é o ponto de entrada do programa.
  - Contém a função 'main' que inicia a execução do Sigma Compiler.
  - Gere a interação, onde possui as funções relevantes, incluindo um conjunto de operações que correspondem às operações pretendidas pelo utilizador, conforme a interpretação feita.
- O ficheiro ```Makefile``` é responsável por compilar e construir o projeto.
  - Define as regras para a compilação, como ```make all```, ```make run``` e ```make clean```.
  - Facilita a compilação utilizando o comando ```make```.
- A diretoria ```/Tests``` contém casos de teste para verificar a corretude do compilador.

<hr>

## ➡️Requisitos e Dependências

Para compilar e executar este projeto, é necessário ter algumas ferramentas instaladas no sistema. Aqui estão as instruções para instalação no Linux e Windows:

### 1. Flex (Fast Lexical Analyzer Generator)

**Linux**:

```bash
sudo apt-get install flex
```

**Windows**:

Faça o download da ferramenta Flex [aqui](https://gnuwin32.sourceforge.net/packages/flex.htm).

### 2. Bison (GNU Parser Generator)

**Linux**:

```bash
sudo apt-get install bison
```

**Windows**:

Faça o download da ferramenta Bison [aqui](https://www.gnu.org/software/bison/).

### 3. Make (GNU Make)

**Linux**:

```bash
sudo apt-get install make
```

**Windows**:

Faça o download da ferramenta Make [aqui](https://gnuwin32.sourceforge.net/packages/make.htm).


Certifique-se de ajustar o PATH em ambos os sistemas operativos para incluir as diretorias onde estão instalados o Flex, Bison e o Make. Deste modo, permitirá a execução destas ferramentas a partir de qualquer diretoria do sistema.

<hr>

## ➡️Guia de Utilização

Para utilizar o Sigma Compiler, siga estes passos:

1. Clone o repositório:

```bash
git clone https://github.com/AfonsoPaula/Sigma-compiler.git
```

2. Navegue até à diretoria do projeto:

```bash
cd Sigma-compiler
```

3. Compile os ficheiros fonte:

```bash
make
```

4. Execute o Sigma-compiler:

```bash
make run
```

Posto isto, irá inicializar o Sigma na linha de comandos, permitindo a interação direta com o compilador.

<hr>

## ➡️Funcionalidades
- **Base de Dados**:
  - Criar, listar e excluir Bases de Dados.
  - Selecionar uma dada Base de Dados.
  - Informar qual a Base de Dados ativa.
 
- **Tabelas**:
  - Criar, listar, exibir tabelas.
  - Atualizar dados de uma dada linha.
  - Excluir tanto linhas como colunas.
 
- **Interatividade**:
  - Interface de linha de comandos para entrada de comandos.
 
- **Limitações**:
  - O número máximo de bases de dados, tabelas, colunas e linhas é limitado por constantes.

<hr>

## ➡️Exemplo de Comandos 

```
                             /* Para Bases de Dados */

CREATE DATABASE dbName;

SHOW DATABASES;

USE dbName;

WHICH dbName;

DROP DATABASE dbName;

                                /* Para Tabelas */

CREATE TABLE tableName (numColumns);

SHOW TABLES;

INSERT INTO tableName VALUES;

SELECT tableName;

DELETE ROW numRow FROM tableName;

DELETE COL colName FROM tableName;

UPDATE ROW numRow FROM tableName;

```
