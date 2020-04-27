/*
Colis�o de Bolinhas

RODRIGO YUSKE YAMAUCHI - UTFPR
*/
#include <stdio.h>
#include <SDL.h>
#include <SDL_image.h>
#include <cmath>
#include <time.h>
#include <stdlib.h>

#define TELA_LARGURA 600
#define TELA_ALTURA 600

using namespace std;

//Class Bola
class Bola{
    public:
        //Coordenadas X e Y da bola
        float coordX, coordY;

        //Proje��es da velocidade da bola
        float velX, velY;
};

/* ~~~~~~~~Variaveis Globais~~~~~~~~~~ */
//Dimensoes da bola
static const int Bola_Raio = 25;

SDL_Window* janela = NULL;
SDL_Renderer* rend = NULL;
SDL_Surface* imgBola = NULL;
SDL_Texture* texturaBola = NULL;
SDL_Rect blocoColisoes;

/* ~~~~~~~~Definindo booleans~~~~~~~~~ */
//Iniciar o SDL
bool iniciar();
bool carregarTextura();

/* ~~~~~~~~Definindo fun��es~~~~~~~~~~ */
//Fun��o para gerar bolas aleat�rias
void gerarBolas(Bola* bolas,int n);

//Fun��o de colis�o contra borda da tela
void colisaoTela(Bola* bolas, int n);

//Fun��o de colis�o entre bolas
void colisaoBolas(Bola* bolas, int n);

//Fun��o para renderizar as bolas
void rendBolas(Bola* bolas, int n);

//Fun��o para movimentar bolas e verificar colis�es
void movimentaBolas(Bola* bolas, int n);

/* ~~~~~~~~Fun��o Main~~~~~~~~~~~~~~~~ */
//Criando Fun��o main
int main (int argc, char* args[]){
    //Qntd. de bolas
    int quantidade;

    //Pedir ao usu�rio a quantidade de bolas
    printf("Digite a quantidade de bolas: ");
    scanf("%d", &quantidade);

	//Carregar Iniciar
	if(!iniciar()){
		printf("Falha para iniciar.\n");
	}
	else{
		//Carregar Textura
		if(!carregarTextura()){
			printf("Falha para carregar a textura.\n");
		}
		else{
			//A semente usando o tempo para achar n�meros aleat�rios
    		srand((unsigned)time(NULL));

    		Bola* bolas = (Bola*) malloc (sizeof (Bola) * quantidade);
            //Inserir quantidade como parametro em gerarBolas
            gerarBolas(bolas, quantidade);

            //Iniar loop de eventos
            bool sair = false;
            SDL_Event eventos;
            while( !sair ){
                while( SDL_PollEvent( &eventos ) != 0 ){
                    //Fechar a Janela
                    if( eventos.type == SDL_QUIT ){
                        sair = true;
                    }
                }
				//Limpa a tela
				SDL_RenderClear(rend);
				SDL_SetRenderDrawColor(rend, 0xFF, 0xFF, 0xFF, 0xFF);

				//Renderiza bolas
				rendBolas(bolas, quantidade);

                //Atualizar bolas por colis�es
                movimentaBolas(bolas, quantidade);
                colisaoTela(bolas, quantidade);
                colisaoBolas(bolas, quantidade);

				//Atualizar tela
				SDL_RenderPresent(rend);

				float momentop_squared = 0;
				int massa = 1;

                for(int i = 0; i < quantidade; i++)
                    momentop_squared += massa * (pow(bolas[i].velX,2) + pow(bolas[i].velY, 2));

                printf("momento p^2 = %.4f\r", momentop_squared);

				//SDL_delay -> nao necessario com VSync
				//SDL_Delay(1000/144);
            }

		}
	}
	//Termina o programa e libera as mem�rias
    SDL_DestroyTexture(texturaBola);
    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(janela);
    IMG_Quit();
	SDL_Quit();
	return 0;
}

bool iniciar(){
	//Indicador de validez
	bool validez = true;

	//Iniciar SDL
	if(SDL_Init(SDL_INIT_VIDEO) < 0){
		printf("SDL n�o inicializou -> SDL Error: %s\n", SDL_GetError());
		validez = false;
	}
	else{
		//Ativar filtro linear
		if(!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")){
			printf("Filtro Linear n�o foi ativado.");
		}

		//Criar Janela
		janela = SDL_CreateWindow("COLIS�O DE BOLAS", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, TELA_LARGURA, TELA_ALTURA, SDL_WINDOW_SHOWN);
		if(janela == NULL){
			printf("Janela n�o foi criada -> SDL Error: %s\n", SDL_GetError());
			validez = false;
		}
		else{
			//Criar renderizador
			rend = SDL_CreateRenderer(janela, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
			if(rend == NULL){
				printf("Renderizador n�o foi criado -> SDL Error: %s\n", SDL_GetError());
				validez = false;
			}
			else{
				//Definir cor para o renderizador
				SDL_SetRenderDrawColor(rend, 0xFF, 0xFF, 0xFF, 0xFF);

				//Permitir imagens em formato png/jpg
				int imgFlags = IMG_INIT_PNG;
				if(!(IMG_Init(imgFlags) & imgFlags)){
					printf("SDL image n�o iniciou -> SDL_image Error: %s\n", IMG_GetError());
					validez = false;
				}
			}
		}
	}

	return validez;
}

bool carregarTextura(){
    bool validez = true;
    imgBola = IMG_Load("sprites/bola.png");
    if (imgBola == NULL){
        printf("Falha para carregar imagem da bola!\n");
        validez = false;
    }
    else{
        SDL_SetColorKey( imgBola, SDL_TRUE, SDL_MapRGB( imgBola->format, 0xFF, 0xFF, 0xFF ) );
        texturaBola = SDL_CreateTextureFromSurface(rend, imgBola);
        if (texturaBola == NULL){
            printf("Falha para carregar imagem da bola!\n");
            validez = false;
        }
    }
    //Limpar a superf�cie velha
    SDL_FreeSurface(imgBola);
    return validez;
}

void gerarBolas(Bola* bolas,int n){
    for (int i = 0; i < n; i++){
        //Gerar as coordenadas aleat�rias
        bolas[i].coordX = rand() % (TELA_LARGURA - 50);
        bolas[i].coordY = rand() % (TELA_ALTURA - 50);

        //Gerar as velocidades aleat�rias
        bolas[i].velX = rand() % (/*(velocidade maxima) + velocidade m�nima*/ 2) + 1;
        bolas[i].velY = rand() % (/*(velocidade maxima) + velocidade m�nima*/ 2) + 1;
    }
    //Evitar bolas surgirem num mesmo lugar
    for(int i = 0; i < n; i++)
        for(int i2 = i + 1; i2 < n; i2++){
            float distEntreBolas_squared = sqrt (pow ((bolas[i].coordX - bolas[i2].coordX), 2) + pow ((bolas[i].coordY - bolas[i2].coordY), 2));
            if (distEntreBolas_squared <= Bola_Raio * 2){
                //Gerar as coordenadas aleat�rias
                bolas[i].coordX = rand() % (TELA_LARGURA - 50);
                bolas[i].coordY = rand() % (TELA_ALTURA - 50);
                i = 0;
                i2 = 1;
            }
        }
}


void colisaoTela(Bola* bolas, int n){
	for(int i = 0; i < n; i++){
		if((bolas[i].velX <= 0 && bolas[i].coordX <= 0 ) || (bolas[i].velX >= 0 && bolas[i].coordX + 2 * Bola_Raio > TELA_LARGURA))
            bolas[i].velX *= -1;
        if((bolas[i].velY <= 0 && bolas[i].coordY <= 0) || (bolas[i].velY >= 0 && bolas[i].coordY + 2 * Bola_Raio > TELA_ALTURA))
            bolas[i].velY *= -1;
	}
}

void colisaoBolas(Bola* bolas, int n){
    for(int i = 0; i < n; i++){
        for(int i2 = 1 + i ; i2 < n; i2++){
            float direcX = bolas[i2].coordX - bolas[i].coordX;
            float direcY = bolas[i2].coordY - bolas[i].coordY;
            float distEntreBolas = sqrtf(direcX*direcX + direcY*direcY);
            if (distEntreBolas <= Bola_Raio * 2 + 0 /*pixels a mais*/){
                //Verificar contato entre as bolas
                printf("CONTATO ENTRE BOLAS!!!\n");

                //https://wikimedia.org/api/rest_v1/media/math/render/svg/14d5feb68844edae9e31c9cb4a2197ee922e409c

                float dVx = bolas[i2].velX - bolas[i].velX;
                float dVy = bolas[i2].velY - bolas[i].velY;

                //Retornar velocidade se bolas n�o estiverem se aproximando para evitar que grudem e buguem
                if ((dVx * direcX + dVy * direcY) >= 0)
                    return;

                //Tangente do �ngulo de colis�o
                float a;
                if (fabs(direcX) > pow(10,-15))
                    a = direcY / direcX;
                else{
                    a = pow(10,15) * direcY / fabs(direcY);
                    if (direcX > 0)
                        a *= -1;
                }

                //Varia��o da velocidade no eixo X das bolas
                float varVelX = (dVx + a * dVy) / (1 + a * a);

                //Atualizar velocidades
                bolas[i].velX += varVelX;
                bolas[i2].velX -= varVelX;
                bolas[i].velY += a*varVelX;
                bolas[i2].velY -= a*varVelX;
            }
        }
    }
}


void rendBolas(Bola* bolas, int n){
    for(int i = 0; i < n; i++){
        blocoColisoes.x = bolas[i].coordX;
        blocoColisoes.y = bolas[i].coordY;
        blocoColisoes.h = Bola_Raio * 2;
        blocoColisoes.w = Bola_Raio * 2;

        //Renderizar na tela
        SDL_RenderCopy(rend, texturaBola, NULL, &blocoColisoes);
    }
}

void movimentaBolas(Bola* bolas, int n){
    for(int i = 0; i < n; i++){
        bolas[i].coordX += bolas[i].velX;
        bolas[i].coordY += bolas[i].velY;
    }
}
