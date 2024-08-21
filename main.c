#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Estrutura para representar uma carta
typedef struct Carta {
    char cor[20];   // Cor da carta
    int numero;     // Número da carta
    struct Carta *prox; // Ponteiro para a próxima carta na lista
} Carta;

// Estrutura para representar uma mão de jogador
typedef struct {
    Carta *inicio;  // Ponteiro para a primeira carta na mão
    Carta *fim;     // Ponteiro para a última carta na mão
} Mao;

// Estrutura para representar o baralho (fila)
typedef struct {
    Carta *inicio;  // Ponteiro para a primeira carta no baralho
    Carta *fim;     // Ponteiro para a última carta no baralho
} Baralho;

// Estrutura para representar a mesa (pilha)
typedef struct {
    Carta *topo;    // Ponteiro para a carta no topo da mesa
} Mesa;

// Protótipos das funções
void inicializarBaralho(Baralho *baralho);
void inicializarMao(Mao *mao);
void inicializarMesa(Mesa *mesa);
void distribuirCartas(Baralho *baralho, Mao *maoJogador, Mao *maoComputador);
void exibirMao(Mao *mao);
void exibirMesa(Mesa *mesa);
int cartaValida(Carta *carta, Mesa *mesa);
Carta *comprarCarta(Baralho *baralho);
void jogadaComputador(Mao *maoComputador, Mesa *mesa);
void limparMao(Mao *mao);
void embaralharBaralho(Baralho *baralho);

// Função para inicializar o baralho com todas as cartas
// Função para inicializar o baralho com todas as cartas, incluindo especiais
void inicializarBaralho(Baralho *baralho) {
    // Inicializa o ponteiro para o início e fim do baralho como NULL
    baralho->inicio = NULL;
    baralho->fim = NULL;

    // Cores disponíveis no jogo UNO
    char cores[4][20] = {"Vermelho", "Amarelo", "Verde", "Azul"};

    // Adiciona as cartas numeradas de 0 a 9 de cada cor no baralho
    int i, j;
    for(i = 0; i < 4; i++) {
        for(j = 0; j <= 9; j++) {
            Carta *novaCarta = (Carta *)malloc(sizeof(Carta));
            strcpy(novaCarta->cor, cores[i]);
            novaCarta->numero = j;
            novaCarta->prox = NULL;

            // Se o baralho estiver vazio, define a nova carta como início e fim
            if (baralho->inicio == NULL) {
                baralho->inicio = novaCarta;
                baralho->fim = novaCarta;
            } else {
                // Adiciona a nova carta ao fim do baralho
                baralho->fim->prox = novaCarta;
                baralho->fim = novaCarta;
            }
        }
    }

    // Adiciona as cartas especiais (Skip, Reverse, +2, Wild Card) de cada cor no baralho
    int p, y;
    for(p = 0; p < 4; p++) {
        for(y = 0; y < 4; y++) {
            Carta *novaCarta = (Carta *)malloc(sizeof(Carta));
            strcpy(novaCarta->cor, cores[p]);
            switch (y) {
                case 0:
                    novaCarta->numero = -1; // Skip
                    break;
                case 1:
                    novaCarta->numero = -2; // Reverse
                    break;
                case 2:
                    novaCarta->numero = -3; // +2
                    break;
                case 3:
                    novaCarta->numero = -4; // Wild Card
                    break;
            }
            novaCarta->prox = NULL;

            // Se o baralho estiver vazio, define a nova carta como início e fim
            if (baralho->inicio == NULL) {
                baralho->inicio = novaCarta;
                baralho->fim = novaCarta;
            } else {
                // Adiciona a nova carta ao fim do baralho
                baralho->fim->prox = novaCarta;
                baralho->fim = novaCarta;
            }
        }
    }

    // Embaralha o baralho após adicionar todas as cartas
    embaralharBaralho(baralho);
}

// Função para embaralhar o baralho
void embaralharBaralho(Baralho *baralho) {
    if (baralho->inicio == NULL) {
        return;
    }

    // Contar o número de cartas no baralho
    int numCartas = 0;
    Carta *cartaAtual = baralho->inicio;
    while (cartaAtual != NULL) {
        numCartas++;
        cartaAtual = cartaAtual->prox;
    }

    // Colocar as cartas em um array
    Carta **cartasArray = (Carta **)malloc(numCartas * sizeof(Carta *));
    cartaAtual = baralho->inicio;
    int i;

    for (i = 0; i < numCartas; i++) {
        cartasArray[i] = cartaAtual;
        cartaAtual = cartaAtual->prox;
    }

    // Embaralhar o array
    for (i = 0; i < numCartas; i++) {
        int j = rand() % numCartas;
        Carta *temp = cartasArray[i];
        cartasArray[i] = cartasArray[j];
        cartasArray[j] = temp;
    }

    // Reconstruir o baralho com a ordem embaralhada
    baralho->inicio = cartasArray[0];
    cartaAtual = baralho->inicio;
    for (i = 1; i < numCartas; i++) {
        cartaAtual->prox = cartasArray[i];
        cartaAtual = cartaAtual->prox;
    }
    cartaAtual->prox = NULL;
    baralho->fim = cartaAtual;

    // Liberar o array temporário
    free(cartasArray);
}

// Função para inicializar a mão de um jogador
void inicializarMao(Mao *mao) {
    // Inicializa a mão do jogador com ponteiros NULL
    mao->inicio = NULL;
    mao->fim = NULL;
}

//Função para inicializar a mesa (pilha)
void inicializarMesa(Mesa *mesa) {
    // Inicializa o topo da mesa como NULL, indicando uma mesa vazia
    mesa->topo = NULL;
}

// Função para distribuir cartas do baralho para a mão de um jogador
void distribuirCartas(Baralho *baralho, Mao *maoJogador, Mao *maoComputador) {
    Carta *cartaAtual;

    // Distribui 7 cartas para o jogador
    int i;
    for(i = 0; i < 7; i++) {
        cartaAtual = comprarCarta(baralho); // Compra uma carta do baralho
        // Adiciona a carta à mão do jogador
        if (maoJogador->inicio == NULL) {
            maoJogador->inicio = cartaAtual;
            maoJogador->fim = cartaAtual;
        } else {
            maoJogador->fim->prox = cartaAtual;
            maoJogador->fim = cartaAtual;
        }
    }

    // Distribui 7 cartas para o computador

    for (i = 0; i < 7; i++) {
        cartaAtual = comprarCarta(baralho); // Compra uma carta do baralho
        // Adiciona a carta à mão do computador
        if (maoComputador->inicio == NULL) {
            maoComputador->inicio = cartaAtual;
            maoComputador->fim = cartaAtual;
        } else {
            maoComputador->fim->prox = cartaAtual;
            maoComputador->fim = cartaAtual;
        }
    }
}

// Função para exibir as cartas na mão de um jogador
void exibirMao(Mao *mao) {
    Carta *cartaAtual = mao->inicio;

    // Percorre a lista de cartas na mão do jogador e exibe cada uma delas
    while (cartaAtual != NULL) {
        if (cartaAtual->numero >= 0) {
            printf("[%s %d] ", cartaAtual->cor, cartaAtual->numero);
        } else {
            switch (cartaAtual->numero) {
                case -1:
                    printf("[%s Bloqueio] ", cartaAtual->cor);
                    break;
                case -2:
                    printf("[%s Reverse] ", cartaAtual->cor);
                    break;
                case -3:
                    printf("[%s +2] ", cartaAtual->cor);
                    break;
                case -4:
                    printf("[Troca de cor] ");
                    break;
                default:
                    printf("[%s Especial] ", cartaAtual->cor);
                    break;
            }
        }
        cartaAtual = cartaAtual->prox;
    }
    printf("\n");
}

// Função para exibir a carta atualmente na mesa
void exibirMesa(Mesa *mesa) {
    if (mesa->topo != NULL) {
        Carta *cartaAtual = mesa->topo;
        printf("\n\nCarta atual na mesa: ");
        if (cartaAtual->numero >= 0) {
            printf("[%s %d]\n\n\n", cartaAtual->cor, cartaAtual->numero);
        } else {
            switch (cartaAtual->numero) {
                case -1:
                    printf("[%s Bloqueio]\n\n\n", cartaAtual->cor);
                    break;
                case -2:
                    printf("[%s Reverse]\n\n\n", cartaAtual->cor);
                    break;
                case -3:
                    printf("[%s +2]\n\n\n", cartaAtual->cor);
                    break;
                case -4:
                    printf("[Troca de cor]\n\n\n");
                    break;
                default:
                    printf("[%s Especial]\n\n\n", cartaAtual->cor);
                    break;
            }
        }
    } else {
        printf("A mesa está vazia.\n");
    }
}

// Função para verificar se uma carta é válida de acordo com a carta atual na mesa
int cartaValida(Carta *carta, Mesa *mesa) {
    if (mesa->topo == NULL) {
        // Se a mesa estiver vazia, qualquer carta é válida
        return 1;
    } else {
        // Verifica se a cor ou o número da carta jogada é igual à carta na mesa
        if (strcmp(carta->cor, mesa->topo->cor) == 0 || carta->numero == mesa->topo->numero) {
            return 1; // Carta válida
        } else {
            return 0; // Carta inválida
        }
    }
}

// Função para comprar uma carta do baralho
Carta *comprarCarta(Baralho *baralho) {
    // Verifica se o baralho está vazio
    if (baralho->inicio == NULL) {
        printf("O baralho está vazio. Não é possível comprar mais cartas.\n");
        return NULL;
    } else {
        // Remove a primeira carta do baralho e a retorna
        Carta *cartaComprada = baralho->inicio;
        baralho->inicio = baralho->inicio->prox;
        cartaComprada->prox = NULL;
        return cartaComprada;
    }
}

// Função para simular a jogada do computador
void jogadaComputador(Mao *maoComputador, Mesa *mesa) {
    Carta *cartaAtual = maoComputador->inicio;
    Carta *cartaSelecionada = NULL;

    // Percorre a mão do computador para encontrar uma carta válida
    while (cartaAtual != NULL) {
        if (cartaValida(cartaAtual, mesa)) {
            cartaSelecionada = cartaAtual; // Seleciona a primeira carta válida encontrada
            break;
        }
        cartaAtual = cartaAtual->prox;
    }

    if (cartaSelecionada != NULL) {
        // Remove a carta selecionada da mão do computador e a coloca na mesa
        if (cartaSelecionada == maoComputador->inicio) {
            maoComputador->inicio = maoComputador->inicio->prox;
            if (maoComputador->inicio == NULL) {
                maoComputador->fim = NULL;
            }
        } else {
            Carta *ant = maoComputador->inicio;
            while (ant->prox != cartaSelecionada) {
                ant = ant->prox;
            }
            ant->prox = cartaSelecionada->prox;
            if (cartaSelecionada == maoComputador->fim) {
                maoComputador->fim = ant;
            }
        }

        // Coloca a carta selecionada no topo da mesa
        cartaSelecionada->prox = mesa->topo;
        mesa->topo = cartaSelecionada;

    printf("\n\n------------------------\n\n\n\n");
        printf("O computador jogou a carta [%s %d].\n", cartaSelecionada->cor, cartaSelecionada->numero);
        printf("\n\n\n\n------------------------\n\n");
    } else {
        // Se o computador não tiver cartas válidas para jogar, ele compra uma carta
        Carta *cartaComprada = comprarCarta(maoComputador);
        if (cartaComprada != NULL) {
            // Adiciona a carta comprada à mão do computador
            if (maoComputador->inicio == NULL) {
                maoComputador->inicio = cartaComprada;
                maoComputador->fim = cartaComprada;
            } else {
                maoComputador->fim->prox = cartaComprada;
                maoComputador->fim = cartaComprada;
            }
            printf("\n\n------------------------\n\n");
            printf("O computador comprou uma carta.\n");
            printf("\n\n------------------------\n\n");
        }
    }
}

// Função para limpar a mão de um jogador
void limparMao(Mao *mao) {
    Carta *cartaAtual = mao->inicio;
    Carta *proximaCarta;

    // Percorre a lista de cartas na mão e libera a memória alocada para cada uma delas
    while (cartaAtual != NULL) {
        proximaCarta = cartaAtual->prox;
        free(cartaAtual);
        cartaAtual = proximaCarta;
    }

    // Define o início e o fim da mão como NULL
    mao->inicio = NULL;
    mao->fim = NULL;
}

void receberCarta(Mao *maoJogador, Baralho *baralho) {
    Carta *cartaComprada = comprarCarta(baralho);
    if (cartaComprada != NULL) {
        // Adiciona a carta comprada à mão do jogador
        if (maoJogador->inicio == NULL) {
            maoJogador->inicio = cartaComprada;
            maoJogador->fim = cartaComprada;
        } else {
            maoJogador->fim->prox = cartaComprada;
            maoJogador->fim = cartaComprada;
        }
        printf("\n\n------------------------\n\n");
        printf("Voce comprou uma carta.\n");
        printf("\n\n------------------------\n\n");
    }
}

int main() {
    // Inicializa o gerador de números aleatórios
    srand(time(NULL));

    // Inicializa o baralho, a mão do jogador e a mão do computador
    Baralho baralho;
    Mao maoJogador, maoComputador;
    inicializarBaralho(&baralho);
    inicializarMao(&maoJogador);
    inicializarMao(&maoComputador);

    // Distribui cartas para o jogador e o computador
    distribuirCartas(&baralho, &maoJogador, &maoComputador);

    // Inicializa a mesa
    Mesa mesa;
    inicializarMesa(&mesa);

    // Loop principal do jogo
    while (1) {
      // Exibe a carta atualmente na mesa
      exibirMesa(&mesa);

      // Exibe a mão do jogador
      printf("Sua mao: ");
      exibirMao(&maoJogador);

      // Verifica se o jogador tem uma carta válida para jogar
      int temCartaValida = 0;
      Carta *cartaAtual = maoJogador.inicio;
      int numCartas = 0;
      while (cartaAtual != NULL) {
        numCartas++;

        if (cartaValida(cartaAtual, &mesa)) {
              temCartaValida = 1;
              break;
          }

      cartaAtual = cartaAtual->prox;
      }

      if (!temCartaValida) {
          // Se o jogador não tiver uma carta válida para jogar, ele recebe uma carta
          receberCarta(&maoJogador, &baralho);
          continue;
      }

      // Solicita ao jogador que escolha uma carta para jogar
      int escolha;
      printf("\n\n------------------------\n\n");
      printf("Escolha uma carta para jogar (digite o numero da carta): ");
      scanf("%d", &escolha);

      // Verifica se a carta escolhida é válida
      Carta *cartaEscolhida = maoJogador.inicio;
      int i;

      for (i = 1; i < escolha; i++) {
          cartaEscolhida = cartaEscolhida->prox;
      }
      if (!cartaValida(cartaEscolhida, &mesa)) {
        printf("\n\n------------------------\n\n\n\n");
          printf("Carta invalida. Voce deve escolher uma carta que combine com a carta na mesa.\n");
          printf("\n\n\n\n------------------------\n\n");
          continue;
      }

      // Remove a carta escolhida da mão do jogador e a coloca na mesa
      if (cartaEscolhida == maoJogador.inicio) {
          maoJogador.inicio = maoJogador.inicio->prox;
          if (maoJogador.inicio == NULL) {
              maoJogador.fim = NULL;
          }
      } else {
          Carta *ant = maoJogador.inicio;
          while (ant->prox != cartaEscolhida) {
              ant = ant->prox;
          }
          ant->prox = cartaEscolhida->prox;
          if (cartaEscolhida == maoJogador.fim) {
              maoJogador.fim = ant;
          }
      }
      cartaEscolhida->prox = mesa.topo;
      mesa.topo = cartaEscolhida;

      // Verifica se o jogador venceu o jogo
      if (maoJogador.inicio == NULL) {
        printf("\n\n------------------------\n\n");
          printf("|\tParabens! Voce venceu o jogo UNO!\t|\n");
          break;
      }

      // Simula a jogada do computador
      jogadaComputador(&maoComputador, &mesa);

      // Verifica se o computador venceu o jogo
      if (maoComputador.inicio == NULL) {
        printf("\n\n------------------------\n\n");
          printf("\n\n|\tO computador venceu o jogo UNO!\t|\n");
          break;
      }
  }

    // Libera a memória alocada para as cartas do baralho
    Carta *cartaAtual = baralho.inicio;
    Carta *proximaCarta;
    while (cartaAtual != NULL) {
        proximaCarta = cartaAtual->prox;
        free(cartaAtual);
        cartaAtual = proximaCarta;
    }

    return 0;
}
