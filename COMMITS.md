# Padrão de Commit
## Formato
- tipo(escopo opcional): mensagem curta

## Tipos 

| Tipo      | Uso                                                                 |
|-----------|----------------------------------------------------------------------|
| `feat`    | Nova funcionalidade                                                  |
| `fix`     | Correção de bugs                                                     |
| `docs`    | Mudanças na documentação                                             |
| `style`   | Formatação, espaços, ponto e vírgula, etc. (sem alteração de lógica)|
| `refactor`| Refatoração de código (sem adicionar nem corrigir funcionalidade)   |
| `test`    | Adição ou modificação de testes                                      |
| `chore`   | Tarefas de build, configs, dependências, etc.                        |
| `build`   | Alterações no sistema de build ou dependências                       |
| `ci`      | Mudanças em arquivos de CI (GitHub Actions, etc.)                   |
| `perf`    | Melhorias de performance                                             |
| `revert`  | Reversão de um commit anterior                                       |

### 📌 Exemplos válidos

- `feat(core): adiciona suporte a multiplataforma`
- `fix(utils): corrige bug no parser de argumentos`
- `docs: atualiza instruções de build`
- `style: remove espaços em branco`
- `refactor(main): separa lógica de entrada em função`
- `test: adiciona testes unitários para AuthService`
- `chore: adiciona extensões ao .gitignore`
- `ci: adiciona configuração do GitHub Actions`

