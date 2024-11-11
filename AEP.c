#include <conio.c>
#include <locale.h>
#include <ctype.h>

#define ARQUIVO "usuarios.txt"
#define MAX_N 50
#define MAX_S 20
#define MAX_U 10
#define CODIFICADOR 7
//-----------------------------Struct--------------------------------------------
typedef struct{
    char nome[MAX_N];
    char senha[MAX_S];
}Usuario;

int i,j; 
char tecla;
//-------------------------------------------------------------------------------
//-----------------Procedimentos e Funções---------------------------------------
void menu();
void cadastrarUsuario();
void mostrarUsuarios();
void criptografarSenha(char *senha);
void descriptografarSenha(char *senha);
int lerQuantidadeUsuarios(Usuario usuarios[], int maxUsuarios);
void ordenarUsuarios(Usuario usuarios[], int n);
void excluirUsuario();
void atualizarUsuario();
int verificarUsuario(char nomeV[MAX_N]);
void exibirTitulo();
void desenharBorda(int x, int y, int largura, int altura);
void minusculo(char *dest, const char *src);
void despedida();
void exibirTextoComQuebraDeLinha(int x, int y, const char *texto, int largura);
int verificarSenha(const char *senha);
int repetirSenha(const char *senhaOriginal);
//-------------------------------------------------------------------------------
//--------------------------Estilização------------------------------------------
void exibirTitulo() {
    textcolor(15);
    gotoxy(22, 6); 
    printf("====================================");
    gotoxy(22, 7);
    printf("|| SISTEMA DE GESTÃO DE USUÁRIOS  ||");
    gotoxy(22, 8);
    printf("====================================");
    textcolor(15);
}

void desenharBorda(int x, int y, int largura, int altura) {
    int i;
    gotoxy(x, y);
    printf("+");
    for (i = 0; i < largura - 2; i++){
		printf("-");
	} 
    printf("+");

    for (i = 1; i < altura - 1; i++) {
        gotoxy(x, y + i);
        printf("|");
        gotoxy(x + largura - 1, y + i);
        printf("|");
    }


    gotoxy(x, y + altura - 1);
    printf("+");
    for (i = 0; i < largura - 2; i++){
    	printf("-");	
	} 
    printf("+");
}
//-------------------------------------------------------------------------------
//--------------------Criptografia e Descriptografia-----------------------------
void criptografarSenha(char *senha){
    for(i = 0; senha[i] != '\0'; i++){
        senha[i] += CODIFICADOR;
    }
}

void descriptografarSenha(char *senha){
    for(i = 0; senha[i] != '\0'; i++){
        senha[i] -= CODIFICADOR;
    }
}

int verificarSenha(const char *senha) {
    int hasUpper = 0, hasLower = 0, hasDigit = 0, hasSpecial = 0;
    
    for (i = 0; senha[i] != '\0'; i++) {
        if (isupper(senha[i])) hasUpper = 1;
        else if (islower(senha[i])) hasLower = 1;
        else if (isdigit(senha[i])) hasDigit = 1;
        else hasSpecial = 1;
    }
    
    return hasUpper && hasLower && hasDigit && hasSpecial;
}

int repetirSenha(const char *senhaOriginal) {
    char senhaRepetida[MAX_S];
    gotoxy(22,14);
	printf("************                      ");
    gotoxy(22,15);
    printf("Repita a senha: \n");
    gotoxy(22,16);
    fgets(senhaRepetida, MAX_S, stdin);
    senhaRepetida[strcspn(senhaRepetida, "\n")] = '\0';
    
    return strcmp(senhaOriginal, senhaRepetida) == 0;
}
//--------------------------------------------------------------------------------
//------------------------------CREATE--------------------------------------------
void cadastrarUsuario(){
    FILE *file = fopen(ARQUIVO, "a");
    
    if(!file){
        printf("Erro ao abrir o arquivo\n");
        menu();
    }
    
    system("cls");
	desenharBorda(20, 5, 40, 16);
	exibirTitulo();
	
    Usuario usuario;
    
    gotoxy(22,10);
    printf("Digite o nome do usuário: \n");
    gotoxy(22,11);
    fgets(usuario.nome, MAX_N, stdin);
    usuario.nome[strcspn(usuario.nome, "\n")] = '\0';
    
    if(verificarUsuario(usuario.nome)){
    	gotoxy(23,14);
    	textcolor(4);
    	printf("Já existe um usuário com este nome");
    	Sleep(1000);
    	system("cls");
    	textcolor(15);
    	menu();
	}else if(strlen(usuario.nome) == 0){
		gotoxy(23,14);
    	textcolor(4);
    	printf("O nome não pode estar vazio");
    	Sleep(1000);
    	system("cls");
    	textcolor(15);
    	menu();
	}
    
	int senhaValida = 0;
    do {
        gotoxy(22,13);
        printf("Digite a senha do usuário: \n");
        gotoxy(22,14);  
        fgets(usuario.senha, MAX_S, stdin);
        usuario.senha[strcspn(usuario.senha, "\n")] = '\0';
        
        if (!verificarSenha(usuario.senha)) {
            gotoxy(23,17);
            textcolor(4);
            printf("A senha deve conter maiúscula");
            gotoxy(23,18);
            printf("minúscula, número e especial");
            Sleep(2000);
            gotoxy(22,14);
            printf("                                 ");
            gotoxy(23,17);
            printf("                                 ");
            gotoxy(23,18);
            printf("                                 ");
            textcolor(15);
        } else if (!repetirSenha(usuario.senha)) {
			gotoxy(25,19);
            textcolor(4);
            printf("As senhas não coincidem");
            Sleep(2000);
            gotoxy(22,14);
            printf("                                  ");
            gotoxy(22,15);
            printf("                                  ");
            gotoxy(22,16);
            printf("                                  ");
            gotoxy(22,19);
            printf("                                  ");
            textcolor(15);
        } else {
            senhaValida = 1;
        }
    } while (!senhaValida);
    
    fflush(stdin);
    
    criptografarSenha(usuario.senha);
    fprintf(file, "%s,%s\n", usuario.nome, usuario.senha);
    fclose(file);

    gotoxy(24,18);
    textcolor(2);
    printf("Usuário adicionado com sucesso!\n");
    gotoxy(0,0);
    Sleep(2000);
    textcolor(15);
	system("cls");
	Sleep(1000);
    menu();
}

int verificarUsuario(char nomeV[MAX_N]) {
    FILE *file = fopen(ARQUIVO, "r");
    
    if (!file) {
        printf("Erro ao abrir o arquivo!\n");
        return 0; 
    }

    Usuario usuario;
    
    while (fscanf(file, "%[^,],%[^\n]\n", usuario.nome, usuario.senha) != EOF) {
        if (strcmp(usuario.nome, nomeV) == 0) {
            fclose(file);
            return 1; 
        }
    }
    
    fclose(file);
    return 0;
}
//--------------------------------------------------------------------------------
//--------------------------------READ--------------------------------------------
int lerQuantidadeUsuarios(Usuario usuarios[], int maxUsuarios){
    FILE *file = fopen(ARQUIVO, "r");
    
    if(!file){
        printf("Não há usuários cadastrados!\n");
        menu();
    }
    
    int i = 0;
    
    while(i < maxUsuarios && fscanf(file, "%[^,],%[^\n]\n", usuarios[i].nome, usuarios[i].senha) != EOF){
        i++;
    }
    
    fclose(file);
    return i;
}

void minusculo(char *dest, const char *src) {
    int i = 0;
    while (src[i]) {
        dest[i] = tolower(src[i]);
        i++;
    }
    dest[i] = '\0';
}

void ordenarUsuarios(Usuario usuarios[], int n) {
    Usuario tmp;
    char nome1[MAX_N], nome2[MAX_N];  

    for(i = 0; i < n - 1; i++) {
        for(j = 0; j < n - i - 1; j++) {
            minusculo(nome1, usuarios[j].nome);
            minusculo(nome2, usuarios[j + 1].nome);

            if(strcmp(nome1, nome2) > 0) {
                tmp = usuarios[j];
                usuarios[j] = usuarios[j + 1];
                usuarios[j + 1] = tmp;
            }
        }
    }
}

void mostrarUsuarios(){
    Usuario usuarios[MAX_U];
    int numUsuarios = lerQuantidadeUsuarios(usuarios, MAX_U);
    system("cls");
    desenharBorda(20, 5, 55, 20);
    if(numUsuarios == 0){
    	gotoxy(28,9);
        printf("Nenhum usuário cadastrado.\n");
        system("cls");
        menu();
    } else {
        ordenarUsuarios(usuarios, numUsuarios);
        gotoxy(30,9);
        textcolor(15);
	    gotoxy(28, 6); 
	    printf("====================================");
	    gotoxy(28, 7);
	    printf("|| SISTEMA DE GESTÃO DE USUÁRIOS  ||");
	    gotoxy(28, 8);
	    printf("||        LISTA DE USUÁRIOS       ||");
	    gotoxy(28, 9);
	    printf("====================================");
	    textcolor(15);
        gotoxy(27,10);
        printf("Nome");
        gotoxy(55,10);
        printf("Senha");
        for(i = 0; i < numUsuarios + 1; i++){
        	gotoxy(27, 11+i);
            printf("%s", usuarios[i].nome);
            gotoxy(55, 11+i);
            printf("%s", usuarios[i].senha);
        };
        gotoxy(32,23);
        textcolor(6);
        printf("DIGITE T PARA VOLTAR AO MENU");
        textcolor(15);
    }
    
    
    do{
    	tecla = getch();
	    if(tecla == 'T' || tecla == 't'){
	    	system("cls");
	    	menu();	
		};
    }while(tecla != 'T' || tecla != 't');
}
//--------------------------------------------------------------------------------
//------------------------------DELETE--------------------------------------------
void excluirUsuario(){
    FILE *file = fopen(ARQUIVO, "r");
    if(!file){
        printf("Não há usuários cadastrados!\n");
        menu();
    }
    
    FILE *temp = fopen("temp.txt", "w");
    if(!temp){
        printf("Erro ao abrir o arquivo temporário\n");
        fclose(file);
        menu();
    }
    
    Usuario usuario;
    char nomeExcluir[MAX_N];
    int encontrado = 0;
    
    system("cls");
	desenharBorda(20, 5, 43, 11);
    exibirTitulo();
    
    gotoxy(22,10);
    printf("Digite o nome do usuário a ser excluído");
    gotoxy(35,12);
    fgets(nomeExcluir, MAX_N, stdin);
    nomeExcluir[strcspn(nomeExcluir, "\n")] = '\0';
    
    while(fscanf(file, "%[^,],%[^\n]\n", usuario.nome, usuario.senha) != EOF){
        descriptografarSenha(usuario.senha);
        if(strcmp(usuario.nome, nomeExcluir) != 0){
            criptografarSenha(usuario.senha);
            fprintf(temp, "%s,%s\n", usuario.nome, usuario.senha);
        } else {
            encontrado = 1;
        }
    }
    
    fclose(file);
    fclose(temp);
    
    if(encontrado){
        remove(ARQUIVO);
        rename("temp.txt", ARQUIVO);
        gotoxy(29,14);
        textcolor(2);
        printf("Usuário excluído!");
        Sleep(1000);
        system("cls");
        textcolor(15);
    } else {
        remove("temp.txt");
        gotoxy(29,14);
        textcolor(4);
        printf("Usuário não encontrado!");
        Sleep(1000);
        system("cls");
        textcolor(15);
    }
    menu();
}
//--------------------------------------------------------------------------------
//------------------------------UPDATE--------------------------------------------
void atualizarUsuario(){
    FILE *file = fopen(ARQUIVO, "r");
    if(!file){
        printf("Não há usuários cadastrados!\n");
        menu();
    }
    
    FILE *temp = fopen("temp.txt", "w");
    if(!temp){
        printf("Erro ao abrir o arquivo temporário\n");
        fclose(file);
        menu();
    }
    
    Usuario usuario, usuarioAlterado;
    char nomeV[MAX_N], senhaV[MAX_S];
    int opt, encontrado = 0;
    
    system("cls");
	desenharBorda(20, 5, 40, 16);
	exibirTitulo();
    
    gotoxy(22,10);
    printf("Digite o nome do usuário:");
    gotoxy(22,11);
	fgets(nomeV, MAX_N, stdin);
    nomeV[strcspn(nomeV, "\n")] = '\0';
    gotoxy(22,12);
    printf("Digite a senha do usuário: ");
    gotoxy(22,13);
    fgets(senhaV, MAX_S, stdin);
    senhaV[strcspn(senhaV, "\n")] = '\0';
    
    criptografarSenha(senhaV);
    
    while(fscanf(file, "%[^,],%[^\n]\n", usuario.nome, usuario.senha) != EOF){
        if(strcmp(usuario.nome, nomeV) == 0 && strcmp(usuario.senha, senhaV) == 0){
            encontrado = 1;
            gotoxy(22,15);
            printf("1 - Atualizar nome");
            gotoxy(22,16);
            printf("2 - Atualizar senha");
            gotoxy(22,17);
			printf("3 - Atualizar ambos");
            gotoxy(22,18);
			printf("Opção: ");
			scanf("%i", &opt);
			fflush(stdin);
            
			system("cls");
			desenharBorda(20, 5, 40, 16);
			exibirTitulo();
            
			if(opt == 1 || opt == 3){
				gotoxy(22,10);
                printf("Digite o novo nome: ");
                gotoxy(22,11);
                fgets(usuarioAlterado.nome, MAX_N, stdin);
                usuarioAlterado.nome[strcspn(usuarioAlterado.nome, "\n")] = '\0';
            } else {
                strcpy(usuarioAlterado.nome, usuario.nome);
            }

            if(opt == 2 || opt == 3){
            	gotoxy(22,12);
                printf("Digite a nova senha: ");
                gotoxy(22,13);
                fgets(usuarioAlterado.senha, MAX_S, stdin);
                usuarioAlterado.senha[strcspn(usuarioAlterado.senha, "\n")] = '\0';
                criptografarSenha(usuarioAlterado.senha);
            } else {
                strcpy(usuarioAlterado.senha, usuario.senha);
            }

            fprintf(temp, "%s,%s\n", usuarioAlterado.nome, usuarioAlterado.senha);
        } else {
            fprintf(temp, "%s,%s\n", usuario.nome, usuario.senha);
        }
    }
    
    fclose(file);
    fclose(temp);
    
    if(encontrado){
        remove(ARQUIVO);
        rename("temp.txt", ARQUIVO);
        textcolor(2);
        gotoxy(25,19);
        printf("Usuário atualizado com sucesso!\n");
    	Sleep(1000);
		textcolor(15);
	} else {
        remove("temp.txt");
        textcolor(4);
        gotoxy(28,19);
        printf("Usuário não encontrado!\n");
        Sleep(1000);
		textcolor(15);
    }
    menu();
}
//--------------------------------------------------------------------------------
//------------------------------DESPEDIDA-----------------------------------------
void despedida() {
    system("cls");
    desenharBorda(1, 5, 79, 20);

    const char *paragrafo1 = "Gostaríamos de dedicar este trabalho principalmente aos nossos professores e mentores, que generosamente compartilham seus conhecimentos e nos guiam ao longo de nossa jornada de aprendizado. Realizar este projeto foi uma experiência enriquecedora que contribuiu significativamente para nosso desenvolvimento e ampliou nosso entendimento sobre o tema.";
    
    const char *paragrafo2 = "Agradecemos a todos que, de alguma forma, apoiaram e incentivaram nosso progresso. Esperamos que o resultado deste trabalho reflita o esforço e a dedicação investidos e que ele possa representar nosso apreço por tudo o que nos foi ensinado.";
	
	gotoxy(32,6);
	textcolor(5);
	printf("Agradecimentos");
	textcolor(8);
    exibirTextoComQuebraDeLinha(3, 8, paragrafo1, 75);
    exibirTextoComQuebraDeLinha(3, 14, paragrafo2, 75); 
    gotoxy(32,20);
    textcolor(15);
    printf("Feito Por:");
	gotoxy(10,22);
	textcolor(10);
	printf("Gabriel Borges");
	gotoxy(30,22);
	textcolor(12);
	printf("Gabriel Motta");
	gotoxy(50,22);
	textcolor(9);
	printf("Nicolas Maran");
	gotoxy(10,23);
	textcolor(10);
	printf("RA: 24055138-2");
    gotoxy(30,23);
	textcolor(12);
	printf("RA: 24055138-2");
	gotoxy(50,23);
	textcolor(9);
	printf("RA: 24055138-2");
    Sleep(10000); 
    system("cls");
	exit(0);
}

void exibirTextoComQuebraDeLinha(int x, int y, const char *texto, int largura) {
    int posX = x;
    int linha = 0;
    
    for (i = 0; texto[i] != '\0'; i++) {
        if (posX - x >= largura) {
            linha++;           
            posX = x;          
        }

        gotoxy(posX, y + linha);  
        putchar(texto[i]);       
        fflush(stdout);          
        posX++;                 
        Sleep(50);             
    }
}
//--------------------------------------------------------------------------------
//--------------------------------MENU--------------------------------------------
void menu() {
    int opt = 1;
    textmode(C80);
	system("cls");

    do {
        clrscr();
        
        desenharBorda(20, 5, 40, 16);
        exibirTitulo();
        
        textcolor(15);
        
        char *opcoes[] = {
            "Cadastrar usuário",
            "Mostrar usuários",
            "Atualizar usuário",
            "Deletar usuário",
            "Sair"
        };

        for (i = 0; i < 5; i++) {
            if (opt == i + 1) { 
                textbackground(0); 
                textcolor(8);
                gotoxy(23,10 + i);
                printf("-> ");
            } else {
            	textbackground(0);
                textcolor(15);
            }

            gotoxy(26, 10 + i); 
            printf("%s", opcoes[i]); 
        }

        textcolor(15);
        gotoxy(26, 17);
        printf("Pressione ENTER para selecionar");
        gotoxy(26, 18);
        printf(" ou W ou S para navegar.");

        tecla = getch(); 

        if (tecla == 'w' || tecla == 'W') { 
            if (opt > 1) {
                opt--;
            }else{
				opt = 5;
			}
        } else if (tecla == 's' || tecla == 'S') {
            if (opt < 5) {
                opt++;
            }else{
				opt = 1;
			}
        } else if (tecla == 13) { 
            break; 
        }

    } while (1);

    switch(opt) {
        case 1: cadastrarUsuario(); break;
        case 2: mostrarUsuarios(); break;
        case 3: atualizarUsuario(); break;
        case 4: excluirUsuario(); break;
        case 5: despedida(); break;
        default: break;
    };  
}
//--------------------------------------------------------------------------------
int main(){
    setlocale(LC_ALL, "portuguese");
    menu();
}
