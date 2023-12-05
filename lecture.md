## Gramáticas
- Uma **_linguagem_** é a expressão de pensamentos por frases e palavras.
- Uma **_gramática_** de uma linguagem é um conjunto de regras que permitem definir a linguagem.
- Pode haver mais de uma gramática para a mesma linguagem: **_gramáticas equivalentes_**.

## Compiladores
- _**Compilador**_ : programa que lê um programa descrito numa linguagem (fonte) e o traduz para outra linguagem (destino), reportando os erros quando eles ocorrem.
- **_Processo de compilação_** : designa o conjunto de tarefas que o compilador deve realizar para poder gerar uma linguagem a partir de outra.
- **_Fases do processo de compilação_** : análise (lexical, sintática e semântica) e síntese (geração e criação de código).
- **_Fase de análise_** : quebra o código fonte nas suas partes, e cria uma representação intermédia do programa (tabela de símbolos e árvore de _parsing_).
- **_Fase de síntese_** : constrói o programa-destino a partir de uma representação intermérdia (árvore, DAG ou dirigido pela sintaxe).
  
- **_Verificador_** : verifica se o formato de entrada obdece a um determinado formato. A resposta é, em geral **sim** ou **não**, _e.g._, _syntax highlighting_ em editores de texto, verificação de normas.

- **_Interpretador_** : executa o código, depois de o verificar, produzindo a partir de uma entrada de dados (distinta da entrada de código) um conjunto de resultados. O processo permite, segundo o código, executar cálculos, atualizar bases de dados, construir estruturas, ... (_e.g._, emuladores, interpretadores e simuladores).

- **_Tradutor_** : traduz uma determinada linguagem de entrada numa linguagem de saída. Quer a linguagem de entrada como a de saída podem ser de nível máquina, _assembly_ ou de alto nível (_e.g._, compiladores, _beauty printers_).


### Processo de Compilação ⬇️
<img src="https://github.com/AfonsoPaula/Sigma-compiler/assets/67978137/df34a221-de8b-4e26-adfd-d97814ce2f92"/>

<hr>

### 1. Análise Lexical

- Agrupa os caracteres em símbolos e verifica se pertencem à linguagem: palavras reservadas, identificadores, literais, operadores, delimitadores, comentários e separadores.
  
<p align="center">
  <img src="https://github.com/AfonsoPaula/Sigma-compiler/assets/67978137/bc39cb43-bea2-43bf-bd0d-93152322be41">
</p>

- Erro lexical (ortográfico): "A [**bolaxa**] comeu uma João"

### 2. Análise sintática

- Verifica se as sequências de símbolos são válidas na linguagem.
- O emparelhamento das regras resulta numa árvore sintática:

<p align="center">
  <img src="https://github.com/AfonsoPaula/Sigma-compiler/assets/67978137/9a5d0f61-2292-4a0f-a66e-5d5f299a371c">
</p>

- Erro sintático: "A bolacha comeu [**uma João**]"

### 3. Análise semântica
- Verifica se as entidades podem desempenhar as funções pretendidas e determiana como as devem desempenhar.
- Manipulação de identificadores: visibilidade, alcance e tabelas de símbolos.
- Tipificação (verificação de tipos): equivalência, subtipificaçãoe e polimorfismo.
- Árvores de ativação (chamadas a procedimentos): gestão de memória, registos de ativação (_stack frames_), ligação de nomes (_binding_).
- Por exemplo em **C** pode incluir: ```break``` fora de ciclos, ```return``` em funções ```void```, número ou tipo dos argumentos das funções incorreto, retirar o endereço de constantes, atribuições a constantes, utilização de variáveis não declaradas, redefinição de funções ou variáveis, _etc_.
- Erro semântico: "[A bolacha] [comeu] [o João]"

### 4. Tratamento e recuperação de erros
- O objetivo é a correção: não interessa quanto tempo se gasta para obter uma resposta a um erro!
- Todos os erros no texto são sintáticos, mas muitos erros não podem ser tratados por linguagens independentes do contexto.
- Erros não detetáveis, ou dificilmente detetáveis, com a tecnologia atual são processados pela análise semântica estática.
- Tipos de respostas a um erro: inaceitáveis (incorretas ou de pouca utilidade) e aceitáveis.
- Tratamento de erros através da inserção na gramática de regras específicas.

### 5. Interpretação
- Processamento das instruções de um programa por uma aplicação e não pelo processador da máquina.
- A cada primitiva da linguagem corresponde um pedaço de código no interpretador.
- A interpretação pode ser:
  - direta, linha a linha, do texto do programa.
  - dirigida pela sintaxe (em gramáticas simples).
  - dirigida pela árvore sintática (100x a 1000x mais lento).
  - execução de código para máquina virtual ou _threading_ (10x a 100x).
- Código intermédio para processadores ideais, com zero ou infinitos registos, pode ser interpretado ou usado para gerar código final.

### 6. Geração de Código
- Produção de código para um processamento específico, tendo em conta as suas características particulares.
- Tipos de processadores:
  - _Stack machines_: B5000, HP-3000 ou máquinas virtuais **Java**, e **.net**.
  - _Accumulator machines_: PDP-9, M6809, 68HC11.
  - _Load-store machines_ (RISC): ARM, RS/6000, MIPS-R, HP-PA, SPARC ou máquinas virtuais **dalvik** (android).
  - _Memory machines_ (CISC):
    - _Register-memory architecture_: i80x86, IBM-RT, M68k, IBM360.
    - _Memory-memory architecture_: VAX, PDP-11.
  - A compilação JIT a partir de um formato intermédio ou compilação _object-code_ a partir de código final.
 
### 7. Otimização de Código
- Preservar o significado do programa original (controlabilidade e obervabilidade).
- Em média, deve melhorar visivelmente o tempo de execução, uso de memória, consumo de bateria, calor libertado ou espaço ocupado pelo programa.
- Deve ter uma boa relação custo benefício.
- Processo: análise do fluxo de controlo, análise do fluxo de dados, transformações, ...
- Otimizações: alto-nível (programador), local, _peephole_, global.

<hr>

## ➡️ Analisador Lexical

- O estado inicial inclui transações vazias para os estados iniciais das expressões regulares.
- Na busca da maior expressão possível, como irão existir diversos estados terminais, deve-se avançar até um erro ou fim dos dados e procurar o último estado terminal aceite.
- A análise recomeça no estado inicial global, no carácter seguinte ao que conduziu ao estado final aceite.
- Na minimização, a partição inicial coloca em grupos diferentes os estados terminais de cada uma das expressões regulares.

Um analisador lexical produz os elementos lexicais de um programa com base numa linguagem regular:
- **modularidade** : permite separar a sintaxe em duas fases distintas: análise lexical e sintática.
- **legibilidade** : expressões regulares são, em geral, mais legíveis.
- **simplicidade** : permite simplificar significativamente o analisador sintático.
- **eficiência** : separação lexical e sintática permite que ambos os analisadores sejam mais eficientes (usa autómato sem pilha auxiliar).
- **portabilidade**  : variações entre ambientes, dispositivos ou sistemas operativos podem ficar contidos no analisador lexical.

### Tarefas do Analisador Lexical

Ficheiro, com extenção ```.l```, dividido em três zonas separadas por uma linha contendo apenas ```%%```:
1. **Declarações** : de macros, de agrupamentos e declarações da linguagem de apoio entre ```%{``` e ```%}```.
2. **Regras** : expressão regular separada da ação semântica por um ou mais espaços em branco. A ação semântica é uma instrução da linguagem ou bloco delimitado por chavetas.
3. **Código** : realização de funções, algumas das quais declaradas acima.

Gerar um analisador lexical, designado por ```lex.yy.c```, com o comando ```lex xxxx.l``` e compilado com o auxílio da biblioteca ```-ll``` (o **flex** utiliza a biblioteca ```-lfl```).

### Expressões Regulares

<p align="center">
  <img src="https://github.com/AfonsoPaula/Sigma-compiler/assets/67978137/1e07e89f-46d5-479b-8824-873951d9a7b0" width="50%">
</p>

### Tratamento de expressões regulares

Identificação da ação semântica a executar, quando mais de uma expressão regular é válida:
- A sequência de entrada mais comprida é a escolhida
- Em caso de igualdade de comprimento é usada a que se encontra primeiro no ficheiro de especificação.
Notar que não se trata da expressão regular maior, mas da sequ~encia de entrada maior:

```flex
%%
dependente  printf("Encontrei 'dependente'\n");
[a-z]*      ECHO;
```

### Funções

- **int yylex(void)** : rotina, gerada pelo **lex**, que realiza a análise lexical. Devolve o número do elemento lexical encontrado ou 0 (zero) quanto atinge o fim do processamento.
- **int yywrap(void)** : rotina, escrita pelo programador, que quando um ficheiro chega ao fim permite continuar o processamento noutro ficheiro. Caso não haja mais ficheiros a processar **yywrap** devolve 1 (um), caso contrário atualiza a variável **yyin** para o ficheiro seguinte e devolve 0 (zero).
- **void yymore(void)** : rotina, invocada numa ação semântica, que permite salvaguardar o texto reconhecido pela expressão regular para que seja concatenadoc om a expressão regular seguinte.
- **void yyless(int n)** : rotina, invocada numa ação semântica, que permite considerar apenas os primeiros **n** carácteres de **yytext**, sendo os restantes reconsiderados para processamento.

### Variáveis globais

- **char yytext[]** : cadeia de caracteres que contém o texto reconhecido pela expressão regular.
- **int yyleng** : comprimento da cadeia de caracteres que contém o texto reconhecido.
- **int yylineno** : número de linha do ficheiro de entrada onde se encontra o último.
- **FILE \*yyin** : ponteiro para o ficheiro de onde são lidos os carácteres a analisar.
- **FILE \*yyout** : ponteiro para o ficheiro de onde é escrito o texto através da macro ```ÈCHO```, ou outro texto que o programador deseje.
- **YYSTYPE yylval** : variável que transporta o valor do elemento lexical reconhecido para outra ferramenta.

### Macros Predefinidas

- **ECHO** : imprime o texto reconhecido (ou seja, **yytext**) pela expressão regular, ou acumulando de outras regras através de sucessivas invocações a **yymore()**. Na realidade está definido como ```#define ECHO fwrite(yytext, yyleng, 1, yyout)```.
- **REJECT** : depois de processada a ação semântica que inclui a chamada ao **REJECT** o processamento recomeça no início do texto reconhecido pela regra, mas ignorando a regra atual.

### Acesso direto a funções de entrada/saída

- **int input(void)** : esta rotina permite ler o carácter seguinte, a partir do ficheiro de entrada, sem que seja processado pelo analisador lexical. O valor ```-1``` (fim de ficheiro) é apenas devolvido no fi do processamento, pois a rotina **yywarp()** é chamada se necessário.
- **void output(int)** : imprime o carácter em **yyout**. Esta rotina não é suportada pelo **flex**.
- **void unput(int)** : reloca o carácter passado como argumento para processamento pelas expressões regulares seguintes. Notar que caso se pretenda recolocar diversos caracteres estes devem ser relocados pela ordem inversa.

### Substituições 

- As substituições permitem simplificar a escrita das expressões regulares.
- Expressão regular, na zona das declarações seguida do identificador da substituição.
- Usa-se, em expressões regulares subsequentes, entre-chavetas.
```flex
DIG [0-9]
INT {DIG}+
EXP [Ee][+-]?{INT}
REAL {INT}"."{INT}({EXP})?
```

### Agrupamentos

- Grupos de expressões regulares ativadas por ações 'BEGIN' e identificadas por '%s' na zona das declarações.
- As expressões regulares do agrupamento são precedidas do identificador entre < e >. O agrupamento 'INITIAL' identifica as regras globais, permanentemente ativas.
- Em cada instante, apenas estão ativas as regras globais e um dos agrupamentos, se tiver sido executada uma ação 'BEGIN'.
```flex
%s IN
%%
<IN>.|\n  ECHO;
<IN>^"%%" BEGIN INITIAL;
^"%%"     BEGIN IN;
.|\n      ;
```

### Ligação ao ```yacc```

- **Elementos a ignorar** : comentários ou espaços em branco, por exemplo.
- **Elementos lexicais úteis _(tokens)_** : descritos por
  - **tipo** : número inteiro devolvido (instrução ```return``` na ação semântica) pela rotina **yylex()** e que descreve o _token_ encontrado: valores de 1 a 255 para caracteres isolados ASCII e >256 para conjuntos de caracteres (devolve 0 para fim e 256 para erro).
  - **valor** : quantidade a guardar na variável global **yylval** para alguns _tokens_, por exemplo inteiros, identificadores ou cadeias de caracteres.
- **Descrição dos _tokens_ necessários** : produzido pelo **yacc**, com opção **-d**, no ficheiro **y.tab.h**. Contém as constantes > 256 a variável **yylval** e o seu tipo, devendo ser incluído nas declarações.

### Extensões do ```flex```

- **Modo mais compatível com _lex_** : gerar com **flex -l** ou incluir ```%option lex-compat``` nas declarações.
- **Acesso a _yylineno_** : usar o modo de compatibilidade com **lex** ou incluir ```%option yylineno``` nas declarações.
- **Agrupamentos exclusivos** : apenas as regras do agrupamento ativo estão válidas, não incluindo as globais, usando '%x' em vez de '%s'.
- **Pilha de agrupamentos** : ao incluir ```%option stack``` pode-se empilhar agrupamentos com: ```yy_push_state(int)```, ```yy_pop_state()``` e ```yy_top_state()```.
- **Modo debug** : gerar com **flex -d** e colocar a variável **yy_flex_debug** a 1.

### Eficiência de Processamento

- Tempo de processamento do autómato é propocional à dimensão do ficheiro a processar e não ao número de expressões regulares usadas (pode influir no número de estados e logo no espaço ocupado).
- Utilizar o mais possível de expressões regulares e fazer o mínimo em **C**.
- Regras mais específicas no princípio da especificação (palavras reservadas, por exemplo) e regras mais genéricas no fim da especificação (identificadores, por exemplo).
