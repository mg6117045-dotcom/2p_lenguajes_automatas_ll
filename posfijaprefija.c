#include <stdio.h>
#include <string.h>
#include <ctype.h>

//variable global para el archivo el cual contiene la expresion infija que se desea convertir a postfija
char narchivo[50] = "ejemplo.txt";


/* Recibe como parámetro c, el cual es un carácter, este carácter representa un operador matemático.
 * Identifica la precedencia de los operadores, los cuales pueden ser ( ^ * / + - ).
 * Retorna un número entero el cual representa la prioridad de los operadores, siendo el de mayor prioridad
 * la potencia, después la multiplicación o división y finalmente la suma o resta.
 */
int prioridad(char c) {
    if (c == '^') return 3;
    else if (c == '*' || c == '/') return 2;
    else if (c == '+' || c == '-') return 1;
    return 0;
}

/* Recibe como parámetro c, el cual es un carácter.
 * Verifica si el carácter es una letra o un número.
 * Retorna 1 si c es un operando y 0 en caso contrario.
 */
int es_operando(char c) {
    return isalnum((unsigned char)c) ? 1 : 0;
}

/* Recibe dos cadenas: la primera contiene la expresion infija y la segunda almacena el resultado de la expresion postfija.
 * Recorre la expresion de izquierda a derecha, identificando operandos, operadores y parentesis mediante una pila,guarda 
 * la expresion convertida en ExpPOS.*/
void c_infija_prefija(char ExpINF[], char ExpPOS[]) {

    char pila[100];
    char simbolo;
    int tope = 0;
    int j = 0;
    int i = 0;
    

// Recorre la expresion infija de izquierda a derecha
    while (ExpINF[i] != '\0') {

        simbolo = ExpINF[i];

// Ignora los espacios en blanco
        if (isspace((unsigned char)simbolo)) {
            i++;
            continue;
        }



// Si encuentra un parentesis izquierdo, lo agrega a la pila  
        if (simbolo == '(') {
            tope++;
            pila[tope] = simbolo;
        }

// Si encuentra un parentesis derecho, extrae los operadores de la pila hasta encontrar el parentesis izquierdo, para hacer pares 

        else if (simbolo == ')') {

            while (tope > 0 && pila[tope] != '(') {
                ExpPOS[j++] = pila[tope];
                ExpPOS[j++] = ' ';
                tope--;
            }


// Elimina el parentesis izquierdo sin agregarlo al resultado
            if (tope > 0) {
                tope--;
            }
        }

// Si encuentra un operando, lo agrega directamente al resultado
        else if (es_operando(simbolo)) {

// Si encuentra un numero, agrega todos sus digitos
            if (isdigit((unsigned char)simbolo)) {

                while (isdigit((unsigned char)ExpINF[i])) {
                    ExpPOS[j++] = ExpINF[i];
                    i++;
                }

// Separa el numero del siguiente operando 
                ExpPOS[j++] = ' ';
                i--;
            }

            else {
                ExpPOS[j++] = simbolo;
                ExpPOS[j++] = ' ';
            }
        }

// Si encuentra un operador, compara su prioridad con los operadores que se encuentran en la pila
        else {

            while (tope > 0 &&
                   pila[tope] != '(' &&
                   prioridad(simbolo) <= prioridad(pila[tope])) {

                ExpPOS[j++] = pila[tope];
                ExpPOS[j++] = ' ';
                tope--;
            }

// agrega el operador actual a la pila 
            tope++;
            pila[tope] = simbolo;
        }

        i++;
    }

// Extrae los operadores restantes de la pila
    while (tope > 0) {

        if (pila[tope] != '(') {
            ExpPOS[j++] = pila[tope];
            ExpPOS[j++] = ' ';
        }

        tope--;
    }

// Elimina el ultimo espacio de la expresion
    if (j > 0 && ExpPOS[j - 1] == ' ') {
        j--;
    }

    ExpPOS[j] = '\0';
}


/* Lee las expresiones infijas,lee cada expresion y llama a c_infija_prefija para convertirla.Finalmente, muestra la expresion original y su resultado.*/
int main() {

    char ExpINF[100];
    char ExpPOS[200];

// Abre el archivo ejemplo.txt
    FILE *archivo = fopen(narchivo, "r");

// Verifica si el archivo se abrio correctamente
    if (archivo == NULL) {
        printf("No se pudo abrir el archivo.\n");
        return 1;
    }

// Lee las expresiones del archivo una por una
    while (fgets(ExpINF, sizeof(ExpINF), archivo) != NULL) {

// Elimina el salto de linea al final de cada expresion para que no interfiera al convertir la expresion
        ExpINF[strcspn(ExpINF, "\n")] = '\0';

// Convierte la expresion infija a postfija
        c_infija_prefija(ExpINF, ExpPOS);

        printf("Infija: %s\n", ExpINF);
        printf("Postfija: %s\n\n", ExpPOS);
    }

// Cierra el archivo despues de terminar 
    fclose(archivo);

    return 0;
}

