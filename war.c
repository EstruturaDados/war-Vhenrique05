// ============================================================================
//         PROJETO WAR ESTRUTURADO - DESAFIO DE CÓDIGO
// ============================================================================


// Inclusão das bibliotecas padrão necessárias para entrada/saída, alocação de memória, manipulação de strings e tempo.
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// --- Constantes Globais ---
// Definem valores fixos para o número de territórios, missões e tamanho máximo de strings, facilitando a manutenção.
#define MAX_TERRITORIOS 5
#define MAX_STRING 25

// --- Estrutura de Dados ---
// Define a estrutura para um território, contendo seu nome, a cor do exército que o domina e o número de tropas.
typedef struct {
	char nome[MAX_STRING];
	char cor[MAX_STRING];
	unsigned int tropas;
} Territorio;

Territorio* territorios;

// --- Protótipos das Funções ---
// Declarações antecipadas de todas as funções que serão usadas no programa, organizadas por categoria.

// Funções de setup e gerenciamento de memória:
int main();
bool alocarMapa();
void inicializarTerritorios();
void liberarMemoria();

// Funções de interface com o usuário:
void exibirMenuPrincipal();
void exibirMapa();
void faseDeAtaque();

// Funções de lógica principal do jogo:
void simularAtaque(Territorio* ataque, Territorio* defesa);
bool verificarVitoria();

// Função utilitária:
void imprimirTropa(unsigned int i);

// ============================================================================


// tenta alocar na memória o espaço para os territórios
// e retorna se houve sucesso
bool alocarMapa() {
	territorios = (Territorio*)calloc(MAX_TERRITORIOS, sizeof(Territorio));
	return territorios != NULL;
}


// inicia o vetor onde será armazenado os dados do mapa
void inicializarTerritorios() {
	// se a referência for nula, retorna
	if (territorios == NULL) {
		return;
	}

	// realiza um loop para armazenar os territórios
	for (int i = 0; i < MAX_TERRITORIOS; i++) {
		// variável onde será armazenado os dados atuais
		Territorio temp;

		// imprime na tela a inscrição atual (i + 1) e o total
		// utilizo o i + 1, devido às listas começarem em 0 na linguagem C
		printf("-------------[%d/%d]-------------\n", i + 1, MAX_TERRITORIOS);

		// eu utilizo o " %24[^\n]" para ler até 24 caracteres (mais o caráctere \0, para terminação de string)
		// e permitindo textos com espaço, terminando a linha no Enter (\n)
		printf("\nInsira o nome do território:\n>> ");
		scanf(" %24[^\n]", &temp.nome);

		// repito o mesmo aqui
		printf("\nInsira a cor da(s) tropa(s):\n>> ");
		scanf(" %24[^\n]", &temp.cor);

		printf("\nInsira a quantidade de tropas:\n>> ");
		scanf("%d", &temp.tropas);

		// armazena os dados atuais na lista global
		territorios[i] = temp;
		printf("\n------------------------------\n");
	}
}

// libera o espaço alocado na memória
void liberarMemoria() {
	free(territorios);
}

// função para imprimir na tela a informação de uma tropa,
// pelo seu índice na lista
void imprimirTropa(const unsigned int i) {
	printf("[%d] Nome: %s, Cor: %s, Tropas: %d\n", (i + 1), territorios[i].nome, territorios[i].cor,
	       territorios[i].tropas);
}

// permite o usuário selecionar os territórios para ataque e defesa
// e chama a função simularAtaque() para executar a lógica da batalha.
void faseDeAtaque() {
	// se o mapa não tiver sido iniciado ele cancela
	if (territorios == NULL) {
		return;
	}

	unsigned int indice_atacante = 0, indice_defesa = 0;

	unsigned int escolha;
	do {
		printf("-------[Fase de Ataque]-------\n");
		printf(" > Selecione um território para realizar o ataque:\n");
		printf("[0] Cancelar ataque\n");

		for (int i = 0; i < MAX_TERRITORIOS; i++)
			imprimirTropa(i);

		printf("------------------------------\n");
		printf(">> ");
		scanf("%d", &escolha);

		printf("\n------------------------------\n");
		if (escolha == 0) {
			printf("[*] Cancelando ataque...\n");
			return;
		}

		if (escolha > MAX_TERRITORIOS) {
			printf("[!] Opção inválida\n");
			continue;
		}

		indice_atacante = escolha - 1;

		// verifica se o territorio selecionado
		// não indica um espaço nulo na memória
		if (&(territorios[indice_atacante]) == NULL) {
			printf("[X] Erro Inesperado\n");
			printf("[!] O territorio %d é nulo\n", indice_atacante);
			continue;
		}

		if (territorios[indice_atacante].tropas <= 0) {
			printf("[!] O território selecionado não possúi tropas\n");
			continue;
		}

		printf("[*] Selecionando território (ATAQUE): \t");
		imprimirTropa(indice_atacante);
		break;

	} while (true);

	do {
		printf("-------[Fase de Ataque]-------\n");
		printf(" > Selecione um território para realizar o ataque:\n");
		printf("[0] Cancelar ataque\n");

		for (int i = 0; i < MAX_TERRITORIOS; i++) {
			if (i != indice_atacante) {
				imprimirTropa(i);
			} else {
				printf("[%d] (ATAQUE) Nome: %s - Cor: %s, Tropas: %d\n", (i + 1), territorios[i].nome, territorios[i].cor,
				       territorios[i].tropas);
			}
		}
		printf("------------------------------\n");
		printf(">> ");
		scanf("%d", &escolha);

		printf("\n------------------------------\n");
		if (escolha == 0) {
			printf("[*] Cancelando ataque...\n");
			return;
		}

		indice_defesa = escolha - 1;

		if (indice_defesa == indice_atacante) {
			printf("[!] Opção inválida\n");
			printf("[>] Esse território já foi selecionado\n");
			printf("    para realizar o ataque\n");

			continue;
		}

		// compara a cor dos dois territórios
		// se forem iguais, o "strcmp" retorna 0
		if (strcmp(territorios[indice_defesa].cor, territorios[indice_atacante].cor) == 0) {
			printf("[!] Opção inválida\n");
			printf("[>] Esse território é da mesma cor de tropa\n");
			printf("    que o território que irá realizar o ataque\n");

			continue;
		}

		if (escolha > MAX_TERRITORIOS) {
			printf("[!] Opção inválida\n");
			continue;
		}

		// verifica se o territorio selecionado
		// não indica um espaço nulo na memória
		if (&(territorios[indice_defesa]) == NULL) {
			printf("[X] Erro Inesperado\n");
			printf("[!] O territorio %d é nulo\n", indice_defesa);
			continue;
		}

		if (territorios[indice_defesa].tropas <= 0) {
			printf("[!] O território selecionado não possúi tropas\n");
			continue;
		}

		printf("[*] Selecionando território (ATAQUE): \t");
		imprimirTropa(indice_atacante);
		printf("[*] Selecionando território (DEFESA): \t");
		imprimirTropa(indice_defesa);
		break;

	} while (true);

	simularAtaque(&territorios[indice_atacante], &territorios[indice_defesa]);
}

// verifica se alcançou a vitória
bool verificarVitoria() {
	// se o mapa não tiver sido iniciado ele retorna falso
	if (territorios == NULL) {
		return false;
	}

	// verifica se todos os territorios estão sob o domínio
	// da mesma cor de tropa
	int tropas_sob_controle = 1;
	for (int i = 1; i < MAX_TERRITORIOS; i++) {
		// compara a cor do território
		// se forem iguais, o "strcmp" retorna 0
		if (strcmp(territorios[i].cor, territorios[0].cor) == 0) {
			tropas_sob_controle++;
		}
	}

	return (tropas_sob_controle == MAX_TERRITORIOS);
}


// função usada para realizar a lógica de batalha entre dois territórios.
void simularAtaque(Territorio* ataque, Territorio* defesa) {
	// escolhe um número aleatório de 0 a 10 para ambos territorios
	const unsigned int r_ataque = rand() % 10 + 1;
	const unsigned int r_defesa = rand() % 10 + 1;

	// crio uma variável para imprimir dinâmicamente o resultado
	char resultado[MAX_STRING] = "ERRO";


	// seleciona a mensagem para o resultado, define o sucesso do ataque como:
	//  - se o valor do ataque for maior ou igual que o defesa (VITÓRIA ou EMPATE)
	//  - ou se o território estiver sem tropas (VITÓRIA por falta de oponente)
	if (
		(r_ataque >= r_defesa) || (defesa->tropas <= 0)
	) {
		// define o texto de resultado entre EMPATE ou VITÓRIA
		// a função "strcpy" serve para copiar o valor de uma string para outra
		strcpy(resultado, (r_ataque == r_defesa ? "EMPATE" : "VITÓRIA DO ATACANTE"));

		// se o território de defesa perde uma tropa
		// exceto no caso dele não possuir nenhuma
		if (defesa->tropas > 0)
			defesa->tropas--;
	} else {
		strcpy(resultado, "ATAQUE FALHOU");
	}

	// exibe o resultado
	printf("[*] O ataque resultou: %s\n", resultado);

	// se o territorio (defesa) perder todas as tropas
	if (defesa->tropas <= 0) {
		// o "memcpy" realiza a cópia do valor da memória
		// ele copia o valor: cor da tropa (ataque) para (defesa)
		memcpy(defesa->cor, ataque->cor, sizeof(defesa->cor));

		// move uma tropa para (defesa)
		// se o território atacante possuir alguma tropa
		if (ataque->tropas > 0) {
			ataque->tropas--;
			defesa->tropas = 1;
		}

	}

}


// função para mostrar o menu principal e selecionar a ação do usuário
void exibirMenuPrincipal() {
	unsigned int escolha;
	do {
		printf("-------[Menu Principal]-------\n");
		printf(" > Selecione uma ação:\n");
		printf(" - [1]: Inicia a fase de ataque\n");
		printf(" - [2]: Verificar vitória\n");
		printf(" - [3]: Exibir o mapa\n");
		printf(" - [0]: Encerra o jogo\n");
		printf("------------------------------\n");
		printf(">> ");
		scanf("%d", &escolha);

		printf("\n------------------------------\n");
		switch (escolha) {
		case 0:
			printf("[*] Saindo do jogo...\n");
			return; // esse return é valido para encerrar a função exibirMenuPrincipal()

		case 1:
			printf("[*] Iniciando fase de ataque...\n");
			faseDeAtaque();
			break;

		case 2:
			printf("[*] Verificando vitória...\n");

		// se a vitória foi alcançada, encerra o jogo
			if (verificarVitoria()) {
				printf("[*] Vitória alcançada!\n");
				printf("[*] Saindo do jogo...\n");
				return;
			}

			printf("[*] A vitória ainda não foi alcançada...\n");
			break;

		case 3:
			printf("[*] Exibindo o mapa...\n");
			exibirMapa();
			break;

		default:
			printf("[!] Opção inválida\n");
			break;
		}
	} while (escolha != 0);
}


// função de exibição do mapa, listando todas as tropas
void exibirMapa() {
	// verifica se o mapa foi iniciado
	if (territorios == NULL) {
		printf("[!] Mapa não iniciado\n");
		return;
	}

	// faz um laço for, imprimindo as informações de todas as tropas
	for (int i = 0; i < MAX_TERRITORIOS; i++)
		imprimirTropa(i);
}


// função que principal, usada para organizar todas as outras
int main() {
	// configura o locale e a seed do gerador de números aleatórios
	setlocale(LC_ALL, "portuguese");
	srand(time(nullptr));

	// tenta alocar o mapa
	if (!alocarMapa()) {
		printf("[!] Falha ao alocar o mapa na memória\n");
		return -1;
	}

	printf("[*] Alocação do mapa na memória realizado com sucesso\n");

	// inicia os dados de territórios no mapa
	inicializarTerritorios();

	// exibe o mapa inicial
	printf("[*] Exibindo o mapa inicial\n");
	exibirMapa();

	// seleciona a opção
	exibirMenuPrincipal();

	// é executado após finalizar o jogo,
	// limpa a memória
	liberarMemoria();

	return 0;
}
