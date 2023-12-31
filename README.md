# Sigma-Compiler
O objetivo deste projeto é desenvolver um compilador capaz de interpretar um conjunto de queries fornecidas pelo utilizador, permitindo a manipulação de bases de dados através da aplicação do **Lexer** (Analisador Léxico) e do **Parser** (Analisador Sintático). 

O compilador oferece aos utilizadores uma maneira mais **acessível**, **eficiente** e **controlada** de interagir e efetuar operações com as bases de dados, simplificando tarefas e melhorando a produtividade.

## Queries Reconhecidas pelo Compilador
falta colocar imagens
### CREATE DATABASE db_nome;
- Permite a criação de uma nova base de dados. 

<p align="center">
  <img src="https://github.com/AfonsoPaula/Sigma-compiler/blob/fd390d2dcc6ae6430f4f93ab0b0c1b106ee7facf/create.png"/>
</p>

No contexto deste exemplo, o nome atribuído pelo utilizador à base de dados é "mysql".
### CREATE TABLE table_name (numColumns);
- oo
### DROP DATABASE db_nome;
- Permite a remoção de uma base de dados, caso ela exista. Neste exemplo, a base de dados a ser removida é "db_nome".
### DROP TABLE tableName;
- Permite a remoção de uma tabela, caso esta exista, correspondente à base de dados que está a ser utilizada.
### SHOW DATABASES;
- Exibe todas as bases de dados criadas ao utilizador.
### SHOW TABLES;
- Exibe todas as tabelas criadas ao utilizador.
### WHICH DATABASE;
- Exibe ao utilizador a base de dados que está a ser utilizada.
### USE DATABASE db_nome;
- Permite definir a base de dados que será utilizada para operações posteriores, como a criação de tabelas, por exemplo.
### INSERT INTO tableName VALUES;
- Permite inserir dados numa tabela específica da base de dados em utilização.
### SELECT tableName;
- 
### DELETE ROW numRow FROM tableName;
- Elimina uma linha específica de uma tabela.
### DELETE COL colName FROM tableName;
- Elimina uma coluna específica de uma tabela.
### UPDATE ROW numRow FROM tableName;
-
## Requisitos e Dependências
- **[Ubuntu](https://ubuntu.com/download/desktop) (Terminal);**
- **Instalação do [bison](https://ccm.net/computing/linux/1711-how-to-install-flex-and-bison-under-ubuntu/) no terminal;**

```
$sudo apt-get install bison
```
- **Instalação do [make](https://pt.linux-console.net/?p=15359) no terminal.**

```
$sudo apt install make
```

## Guia de Utilização
1. Na parte superior da lista de ficheiros, clique em **"<> Code"**. 

<p align="center">
  <img src="https://github.com/AfonsoPaula/Sigma-compiler/blob/6591e67a3b799346fa9c0ef5da1cac5d06cc3d28/p1.png"/>
</p>

2. Clicar em **"Download ZIP"**.

<p align="center">
  <img src="https://github.com/AfonsoPaula/Sigma-compiler/blob/147128c61bc52a86209375bd93663a13cb34dfb4/p2.png"/>
</p>

3. Utilizando o terminal do **Ubuntu**, navegue do diretório atual até ao arquivo **ZIP**.

4. Digitar o seguinte comando no terminal:

```
$make
```

O comando **"make"** irá fazer a compilação dos ficheiros necessários para a existência do compilador.

5. Digitar o seguinte comando no terminal:

```
$make run
```

O comando **"make run"** irá executar o compilador e permitir a utilização do mesmo.

6. Por último, digitar o seguinte comando no terminal:

```
$make clear
```

O comando **"make clear"** permitirá eliminar todos os ficheiros temporários que foram criados durante a utilização do compilador.
   
   


  
