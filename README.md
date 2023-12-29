# Sigma-Compiler
O objetivo deste projeto é desenvolver um compilador capaz de interpretar um conjunto de queries fornecidas pelo utilizador, permitindo a manipulação de bases de dados através da aplicação do Lexer(Analisador Léxico) e do Parser(Analisador Sintático).

## Queries Reconhecidas pelo Compilador
### CREATE DATABASE db_nome;
- Permite a criação de uma nova base de dados. 

<p align="center">
  <img src="https://github.com/AfonsoPaula/Sigma-compiler/blob/fd390d2dcc6ae6430f4f93ab0b0c1b106ee7facf/create.png"/>
</p>

No contexto deste exemplo, o nome atribuído à base de dados é "mysql".

### DROP DATABASE db_nome;
Permite a remoção de uma base de dados, caso ela exista. Neste exemplo, a base de dados a ser removida é "db_nome".
### SHOW DATABASES;
Exibe todas as bases de dados criadas ao utilizador.
### WHICH DATABASE;
### USE db_nome;
### CREATE TABLE table_name;



  
