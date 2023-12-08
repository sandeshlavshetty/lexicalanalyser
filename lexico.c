#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Define token types
typedef enum
{
  KEYWORD,
  IDENTIFIER,
  LITERAL,
  OPERATOR,
  SPECIAL_SYMBOL
} TokenType;

// Define a structure to represent tokens
typedef struct
{
  TokenType type;
  char value[100]; // Assuming a maximum token length of 100 characters
} Token;

// Function to check if a character is an operator or special symbol
int isOperatorOrSymbol(char ch)
{
  char operators[] = "+-*/%%=><!";
  char symbols[] = "();,{}[]";

  for (int i = 0; i < strlen(operators); i++)
  {
    if (ch == operators[i] || ch == symbols[i])
    {
      return 1;
    }
  }
  return 0;
}

// Function to check if a string is a keyword
int isKeyword(char *str)
{
  char *keywords[] = {"if", "else", "while", "int", "float", "char", "return","double","long","short","signed","unsigned","void",
  "for","do","goto","switch","case","break","continue","default","enum","extern","register","sizeof","static","struct","typedef",
  "union","volatile","auto","const","restrict","_Bool","_Complex","_Imaginary"};
  int numKeywords = sizeof(keywords) / sizeof(keywords[0]);

  for (int i = 0; i < numKeywords; i++)
  {
    if (strcmp(str, keywords[i]) == 0)
    {
      return 1;
    }
  }
  return 0;
}

// Function to analyze the source code and generate tokens
void lexicalAnalyzer(char *sourceCode)
{
  int length = strlen(sourceCode);
  int i = 0;

  while (i < length)
  {
    // Skip whitespace characters
    while (isspace(sourceCode[i]))
    {
      i++;
    }
    // Check for operators and special symbols
    if (isOperatorOrSymbol(sourceCode[i]))
    {
      Token token;
      token.type = OPERATOR;
      token.value[0] = sourceCode[i];
      token.value[1] = '\0';         //** why this didnt understood
      printf("Type: Operator, Value: %s\n", token.value);
      i++;
    }
    // Check for identifiers and keywords
    else if (isalpha(sourceCode[i]) || sourceCode[i] == '_')
    {
      char buffer[100];
      int j = 0;

      // Collect characters for the identifier
      while (isalnum(sourceCode[i]) || sourceCode[i] == '_')
      {
        buffer[j++] = sourceCode[i++];
      }
      buffer[j] = '\0';    

      Token token;

      if (isKeyword(buffer))
      {
        token.type = KEYWORD;
      }
      else
      {
        token.type = IDENTIFIER;
      }

      strcpy(token.value, buffer);
      printf("Type: %s, Value: %s\n", token.type == KEYWORD ? "Keyword" : "Identifier", token.value);
    }
    // Check for literals (integers and floating-point numbers)
    else if (isdigit(sourceCode[i]))
    {
      char buffer[100];
      int j = 0;

      // Collect characters for the literal
      while (isdigit(sourceCode[i]))
      {
        buffer[j++] = sourceCode[i++];
      }

      // Check for a floating-point literal
      if (sourceCode[i] == '.')
      {
        buffer[j++] = sourceCode[i++];
        while (isdigit(sourceCode[i]))
        {
          buffer[j++] = sourceCode[i++];
        }
        buffer[j] = '\0';
        Token token;
        token.type = LITERAL;
        strcpy(token.value, buffer);
        printf("Type: Literal, Value: %s\n", token.value);
      }
      else
      {
        buffer[j] = '\0';
        Token token;
        token.type = LITERAL;
        strcpy(token.value, buffer);
        printf("Type: Literal, Value: %s\n", token.value);
      }
    }
    // Check for string literals
    else if (sourceCode[i] == '\"')
    {
      char buffer[100];
      int j = 0;

      // Collect characters for the string literal
      buffer[j++] = sourceCode[i++]; // include the opening double quote

      while (sourceCode[i] != '\"')
      {
        buffer[j++] = sourceCode[i++];
      }

      buffer[j++] = sourceCode[i++]; // include the closing double quote
      buffer[j] = '\0';

      Token token;
      token.type = LITERAL;
      strcpy(token.value, buffer);
      printf("Type: Literal, Value: %s\n", token.value);
    }
    else
    {
      // Unrecognized character
      printf("Unrecognized character: %c\n", sourceCode[i]);
      i++;
    }
  }
}

int main()
{
 FILE *file = fopen("source_code.txt", "r");
if (file == NULL)
{
    printf("Could not open file source_code.txt");
    return 1;
}

// Go to the end of the file
fseek(file, 0, SEEK_END);
// Get the file size
long fileSize = ftell(file);
// Go back to the start of the file
fseek(file, 0, SEEK_SET);

// Allocate memory for the file content
char *sourceCode = malloc(fileSize + 1);
if (sourceCode == NULL)
{
    printf("Could not allocate memory for sourceCode");
    return 1;
}

// Read the file into sourceCode
fread(sourceCode, 1, fileSize, file);

// Null-terminate the sourceCode string
sourceCode[fileSize] = '\0';

fclose(file);

lexicalAnalyzer(sourceCode);

free(sourceCode);

return 0;
}