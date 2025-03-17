#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MEM_SIZE 256
#define LINE_SIZE 50

int memory[MEM_SIZE] = {0}; 
int ACC = 0;               
int ICR = 0;               
int MAR = 0;                
int MDR = 0;                
int UC = 1;                 // Unidad de Control


int getMemoryIndex(char *dx){
    if (dx[0] == 'D' && dx[1] >= '0' && dx[1] <= '9')
    {
        return atoi(dx + 1); 
    }
    return -1; 
}

void executeInstruction(char *inst, char *op1, char *op2, char *op3){
    int addr1 = getMemoryIndex(op1);
    int addr2 = getMemoryIndex(op2);
    int addr3 = getMemoryIndex(op3);

    if (strcmp(inst, "SET") == 0 && addr1 != -1)
    {
        memory[addr1] = atoi(op2); 
    }
    else if (strcmp(inst, "LDR") == 0 && addr1 != -1)
    {
        ACC = memory[addr1]; 
    }
    else if (strcmp(inst, "ADD") == 0)
    {
        if (addr1 != -1 && addr2 == -1 && addr3 == -1)
        {
            ACC += memory[addr1];
        }
        else if (addr1 != -1 && addr2 != -1 && addr3 == -1)
        {
            ACC = memory[addr1] + memory[addr2];
        }
        else if (addr1 != -1 && addr2 != -1 && addr3 != -1)
        {
            memory[addr3] = memory[addr1] + memory[addr2];
        }
    }
    else if (strcmp(inst, "INC") == 0 && addr1 != -1)
    {
        memory[addr1]++;
    }
    else if (strcmp(inst, "DEC") == 0 && addr1 != -1)
    {
        memory[addr1]--;
    }
    else if (strcmp(inst, "STR") == 0 && addr1 != -1)
    {
        memory[addr1] = ACC; // Guarda el valor del acumulador en memoria
    }
    else if (strcmp(inst, "SHW") == 0)
    {
        if (strcmp(op1, "ACC") == 0)
        {
            printf("%d\n", ACC);
        }
        else if (strcmp(op1, "ICR") == 0)
        {
            printf("%d\n", ICR);
        }
        else if (strcmp(op1, "MAR") == 0)
        {
            printf("%d\n", MAR);
        }
        else if (strcmp(op1, "MDR") == 0)
        {
            printf("%d\n", MDR);
        }
        else if (strcmp(op1, "UC") == 0)
        {
            printf("%d\n", UC);
        }
        else if (addr1 != -1)
        {
            printf("%d\n", memory[addr1]);
        }
    }
    else if (strcmp(inst, "PAUSE") == 0)
    {

       char input[10];

       while (1)
       {
         printf("Se ha pausado la ejecucion. Escriba CONT para continuar o DUMP para inspeccionar la memoria \n");
         scanf("%s", input);
         if (strcmp(input, "CONT") == 0)
         {
            break;
         }
         else if (strcmp(input, "DUMP") == 0)
         {
            printf("ACC: %d, ICR: %d, MAR: %d, MDR: %d, UC: %d\n", ACC, ICR, MAR, MDR, UC);
            for(int i = 0; i< MEM_SIZE; i++)
            {
                if (memory[i] != 0)
                {
                    printf("D%d: %d\n", i, memory [i]);
                }
            }
         }
         else
        {
            printf("Comando desconocido. Porfavor escriba 'CONT' para continuar la ejecucion o 'DUMP' para inspecionar la memoria \n");
        }
       }
       
    }
    else if (strcmp(inst, "END") == 0)
    {
        UC = 0; // Finaliza el ciclo de ejecución
    }
}

void runProgram(const char *filename){
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        printf("Error al abrir el archivo %s\n", filename);
        return;
    }

    char line[LINE_SIZE];
    while (fgets(line, LINE_SIZE, file) && UC)
    {
        char inst[10], op1[10] = "NULL", op2[10] = "NULL", op3[10] = "NULL";
        sscanf(line, "%s %s %s %s", inst, op1, op2, op3);
        executeInstruction(inst, op1, op2, op3);
        ICR++; 
    }
    fclose(file);
}

int main(int argc, char *argv[]) {
    if(argc != 2) {
        printf("Uso correcto: %s <nombre_del_archivo>\n", argv[0]);
        return 1;
    }
    runProgram(argv[1]);
    return 0;
}

