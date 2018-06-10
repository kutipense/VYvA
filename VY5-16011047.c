#include <stdbool.h>
#include <limits.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>

/*
	EVERY STATEMENT THAT INCLUDES PRINT KEYWORD STANDS TO OUTPUT.
	THOSE STATEMENTS CAN BE DELETED SAFELY.
*/

// BEGINNING OF STACK OPS

#define STACKLIMIT (100)

typedef struct Stack{
        int size;
        int stack[STACKLIMIT];
} Stack;

bool isEmpty(Stack *s){ return s->size == -1; }

int pop(Stack *s){ return s->stack[s->size--]; }

void push(Stack* s, int val) { s->stack[++s->size] = val; }

int peak(Stack *s) { return s->stack[s->size]; }

// END OF STACK OPS

//preprocessor declarations for various operations.
//mapping macro for variables to array indicies
#define TEXT2VARIABLE(T) ((T)-'A'-((T)>=97 ? 6 : 0))
//mapping macro for array indicies to variables
#define VARIABLE2TEXT(V) (V+'A'+((V)>=26 ? 6 : 0))
//operator priorty comparation macro
#define OP2PRIORTY(O) ((O)-'(')

//function to evaluate given arithmetic operation
int evaluate(int a, int b, char c){
    switch (c){
        case '*': return a*b;
        case '/': return a/b;
        case '+': return a+b;
        case '-': return a-b;
    }
}

//function to print given stack
void printStack(Stack *s){
	int i;
	printf("Stack:");
	if(!isEmpty(s))
		for(i=0; i<=s->size;i++)
			printf("%c", s->stack[i]);
	else
		printf("Empty");
	printf("\n");
}

//function to print given stack to given buffer
void printStackBuffer(Stack *s, char* buff){
	int i;
	snprintf(buff+strlen(buff),STACKLIMIT*2-strlen(buff),"Stack:");
	for(i=0; i<=s->size;i++)
        snprintf(buff+strlen(buff),STACKLIMIT*4-strlen(buff),"%d ",s->stack[i]);
	snprintf(buff+strlen(buff),STACKLIMIT*2-strlen(buff),"\n");
}


int shunting_yard(char *input, int *variables){
	// copy of given text 
    char *statement = strdup(input);
    // priority array for operators
    static char priority[8] = {0,0,2,1,0,1,0,2};
    // values stack for just in time calculations
    Stack values = {.size=-1};
    // operator stack
    Stack ops = {.size=-1};
    // buffer to store output text
    char postfix[STACKLIMIT*2] = "";
    // buffer to store output text
    char result[STACKLIMIT*4] = "";
    // operator variable
    char *op;

    op = strtok(statement," ");
    while(op!=NULL && *op!=';'){
        if (strlen(op) == 1){
            if(ispunct(*op)){
            	// is current text is opeator
                if(*op=='('){
                	// open parenthesis case
                    push(&ops,'(');
                }else if (*op==')'){
                	// close parenthesis case
                    while(!isEmpty(&ops) && peak(&ops)!='('){
                        snprintf(postfix+strlen(postfix),STACKLIMIT*2-strlen(postfix),"%c",peak(&ops));
                        printStackBuffer(&values,result);
                        
                        //evaluate the expression until encounter to a parenthesis
				        push(&values, evaluate(pop(&values),pop(&values),pop(&ops)));
                        
						printf("Postfix:%-20s",postfix);
				        printStack(&ops);
                    }
                    pop(&ops);
                }else{
                	// pop until there is no higher priority operator than you
                    while(!isEmpty(&ops) && priority[OP2PRIORTY(peak(&ops))]>=priority[OP2PRIORTY(*op)]){
                        snprintf(postfix+strlen(postfix),STACKLIMIT*2-strlen(postfix),"%c",peak(&ops));
                        printStackBuffer(&values,result);
                        
                        //evaluate the expression
				        push(&values, evaluate(pop(&values),pop(&values),pop(&ops)));
                        
				        printf("Postfix:%-20s",postfix);
				        printStack(&ops);
                    }
                    push(&ops,*op);
                }
            }else{
            	// if it's a digit
                push(&values,isalpha(*op) ? (variables[TEXT2VARIABLE(*op)]) : *op - '0');

                snprintf(postfix+strlen(postfix),STACKLIMIT*2 - strlen(postfix),"%c",*op);
            }
        }else{
            printStackBuffer(&values,result);
            
        	// if it's a number
			push(&values,atoi(op));
            
            snprintf(postfix+strlen(postfix),STACKLIMIT*2 - strlen(postfix),"%d",peak(&values));
        }
        op = strtok(NULL," ");
        
        printf("Postfix:%-20s",postfix);
        printStack(&ops);
    }

	// empty stack if there is left any operator
    while(!isEmpty(&ops)){
        snprintf(postfix+strlen(postfix),STACKLIMIT*2-strlen(postfix),"%c",peak(&ops));
		printStackBuffer(&values,result);
		
		//evaluate the expression
        push(&values, evaluate(pop(&values),pop(&values),pop(&ops)));
    }

    printf("Postfix:%-20sStack:Empty\n",postfix);
    printf("\nOutput:%s\n",postfix);
    printf("\n%sStack:%d\n",result,peak(&values));
    free(statement);
    return pop(&values);
}


int main(int argc, char const *argv[]){
	// variable value array
    int variables[52] = {0};
    // buffer to read lines
    char buffer[STACKLIMIT*2];
    // buffer util variable
    char *bufftmp;
    // input file
    FILE *f = fopen("input.txt","r");
    
    while(fgets(buffer, STACKLIMIT*2,f)){
    	bufftmp = strtok(buffer,"\n");
        printf("expression: %s\n",bufftmp);
        variables[TEXT2VARIABLE(*buffer)] = shunting_yard(bufftmp+4,variables);
        printf("%c: %d\n\n",*bufftmp, variables[TEXT2VARIABLE(*buffer)]);
    }

    // array index
    int i=0;
    for(i = 0; i<52; i++){
    	if(variables[i])	
	        printf("%c: %d ",VARIABLE2TEXT(i), variables[i]);
	}
    return 0;
}
