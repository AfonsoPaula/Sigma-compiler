# Sigma-Compiler 🐉

**Main Goal**: Desenvolver um compilador para queries SQL que leve em consideração uma tabela específica.
<p align="center">
  <img src="https://github.com/AfonsoPaula/Sigma-compiler/assets/67978137/c43479ea-bed0-4aad-8b72-6413ba1b9887"/>
</p>

## Definir a linguagem:
- **Especificar a Sintaxe**:
  - Incluir operações como SELECT, FROM, WHERE, JOIN, GROUP BY, ORDER BY, etc...

- **Identificar palavras chave**:
  - Listar palavras chaves essenciais para a linguagem SQL, bem como as extensões específicas que poderemos ver.
 
## Mapear a estrutura da Tabela:
- **Esquema da tabela**:
  - Definir a estrutura da tabela que será usada nas queries. Envolver identificar campos, tipos de dados, chaves primárias, estrangeiras, etc...
 
## Permitir Operações SQL Básicas:
- **Operações de Seleção Básicas (SELECT)**:
  - Permitir que o compilador selecione colunas específicas da tabela.
- **Operações de Filtro (WHERE)**:
  - Implementar a capacidade de filtrar resultados com base em condições específicas.
 
## **Tartar de junções (JOIN)**:
- **Tipos de Junção**:
 - Adiconar suporte para diferentes tipos de junção, como INNER JOIN, LEFT JOIN, RIGHT JOIN, ...
- **Condições de Junção**:
  - Permitir que o compilador lide com condições de junção entre tabelas.
 
## **Considerar GROUP BY e ORDER BY**:
- **Agrupamentos (ORDER BY)**:
  - Implementar a capacidade de agrupar resultados com base em colunas específicas.
- **Ordenação (ORDER BY)**:
  - Permitir que os resultados sejam ordenados com base em colunas específicas.

## **Projetar a integração de Tabelas**:
- **Definir a Tabela de Dados**:
  - Ter uma maneira de carregar a tabela de dados no compilador.
- **Verificação de Campos**:
  - Garantir que as queries geradas estejam de acordo com o esquema da tabela, evitando referências a campos inexistentes.
 
## **Testar**
- **Testar com Dados de Exemplos**:
  - Criar casos de teste usando uma tabela específica e verificar se as queries geradas produzem os resultados esperados.

<hr>

#### 1. Estrutura do Projeto

```
sigma_compile/
│
├── src/
│   ├── lexer.l            # Arquivo fonte do lexer
│   ├── parser.y           # Arquivo fonte do parser
│   ├── main.c             # Código principal (Main)
│   └── ...                # Outros arquivos necessários
│
├── include/
│   └── ...                # Cabeçalhos necessários
│
└── Makefile               # Script de compilação
```
#### 2. Compilação do Lexer e Parser
#### 3. Criação do Executável Main
#### 4. JSON com Tabelas
```JSON
{
  "tabelas": [
    {
      "nome": "usuarios",
      "colunas": ["id", "nome", "idade", "email"],
      "dados": [
        [1, "João", 25, "joao@email.com"],
        [2, "Maria", 30, "maria@email.com"]
        // ... outros dados
      ]
    },
    // ... outras tabelas
  ]
}
```
#### 5. Execução do Compilador (```./main fichiero_tabelas.json```)
#### 6. Entrada de Queries
#### 7. Obter Output
