# Minishell

GRAMMAR::
<command-line> ::= "(" <command-line> ")" | <command>
<command> ::= <simple_command> (<operator> <simple_command>)*
<operator> ::= "|" | ";" | "&&" | "||"
<simple_command> ::= <redirection>* <word> <word>* <redirection>*
<redirection> ::= ">" <file> | ">>" <file> | "<" <file> | "2>" <file> | "2>>" <file>
<word> ::= "$" <identifier> | <identifier> | "*" | "$?"
<identifier> ::= [a-zA-Z_-\s][a-zA-Z0-9_-\s]* | "<identifier>" | '<identifier>'
<file> ::= <word>

# To-Do List for Creating a Shell Parser

## **1. Define the Parser's Purpose**
   - [ ] Parse user input into:
     - Commands
     - Arguments
     - Redirections
     - Pipes
     - Environment variable expansions
     - Built-in commands
   - [ ] Handle syntax errors gracefully.

---

## **2. Tokenizer (Lexical Analysis)**
   - [ ] Split the input string into tokens based on spaces and special characters (`|`, `>`, `<`, `&`, `;`, etc.).
   - [ ] Handle quotes (`"`, `'`) to group words into a single token.
     - Example: `"hello world"` should be treated as one token.
   - [ ] Handle escape characters (`\`) to allow special characters as literals.
     - Example: `echo \$HOME` → `echo $HOME`
   - [ ] Recognize special symbols:
     - [ ] Redirection (`>`, `<`, `>>`, `2>`, etc.)
     - [ ] Piping (`|`)
     - [ ] Background execution (`&`)
     - [ ] Command chaining (`;`)
   - [ ] Output: A list of tokens.
     - Example:
       ```
       Input: ls -l | grep "pattern" > output.txt
       Tokens: ["ls", "-l", "|", "grep", "pattern", ">", "output.txt"]
       ```

---

## **3. Syntax Analysis**
   - [ ] Build a syntax tree or structured representation of the command.
   - [ ] Identify the different parts of the command:
     - [ ] Command
     - [ ] Arguments
     - [ ] Input/Output redirections
     - [ ] Pipes
   - [ ] Example:
     ```
     Input: ls -l | grep "pattern" > output.txt
     Syntax Tree:
       ├── Command: ls
       │   ├── Argument: -l
       ├── Pipe: |
       ├── Command: grep
       │   ├── Argument: "pattern"
       ├── Redirection: >
       │   ├── File: output.txt
     ```

---

## **4. Handle Special Characters**
   - [ ] **Pipes (`|`)**
     - [ ] Split commands connected by pipes into separate units.
     - [ ] Ensure output of one command becomes input for the next.
   - [ ] **Redirections**
     - [ ] Identify and store input/output redirections (`>`, `<`, `>>`, `2>`, etc.).
     - [ ] Validate file paths and permissions.
   - [ ] **Background Execution (`&`)**
     - [ ] Split commands ending with `&` for background execution.
   - [ ] **Command Chaining (`;`)**
     - [ ] Separate multiple commands chained with `;`.

---

## **5. Environment Variable Expansion**
   - [ ] Detect variables prefixed with `$`.
   - [ ] Replace variables with their values.
     - Example:
       ```
       Input: echo $HOME
       Output Tokens: ["echo", "/home/user"]
       ```

---

## **6. Error Handling**
   - [ ] Detect and report syntax errors:
     - [ ] Missing quotes.
     - [ ] Misplaced redirection operators.
     - [ ] Unsupported syntax.
     - Example:
       ```
       Input: ls | > output.txt
       Error: Misplaced redirection operator.
       ```
   - [ ] Provide helpful error messages to the user.

---

## **7. Optional Features**
   - [ ] **Command Substitution**
     - [ ] Support for `$(...)` and backticks (`` `...` ``).
     - Example:
       ```
       Input: echo $(ls)
       Output Tokens: ["echo", "file1", "file2"]
       ```
   - [ ] **Globbing**
     - [ ] Expand wildcards (`*`, `?`, etc.) in file paths.
     - Example:
       ```
       Input: ls *.txt
       Output Tokens: ["ls", "file1.txt", "file2.txt"]
       ```

---

## **8. Testing and Debugging**
   - [ ] Test with simple commands.
     - Example: `ls`, `pwd`, `echo hello`
   - [ ] Test complex commands.
     - Example: `ls -l | grep "pattern" > output.txt`
   - [ ] Test error scenarios.
     - Example: `cat < > file`
   - [ ] Debug edge cases:
     - [ ] Empty input.
     - [ ] Excessive spaces.
     - [ ] Nested quotes.

---

## **9. Integration with the Shell**
   - [ ] Connect the parser's output to the executor.
   - [ ] Ensure the parser's output is in a format that the executor can use.

---

## **10. Documentation**
   - [ ] Document the parser's functionality and design.
   - [ ] Include examples of input and corresponding parsed output.
   - [ ] Write clear comments in the code for maintainability.

---

## **Tools and Libraries (Optional)**
   - [ ] Use a parser generator (e.g., `yacc` or `bison`) for more complex syntax.
   - [ ] Use a regular expression library for tokenization.