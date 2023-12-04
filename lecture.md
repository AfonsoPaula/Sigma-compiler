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

## Analisador Lexical

- O estado inicial
