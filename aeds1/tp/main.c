#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <stdio.h>
#include <time.h>

#define SCREEN_W 850
#define SCREEN_H 650
#define FPS 60
#define TAMANHO_imagem_jogador 50
#define PASSO 50
#define NUMERO_DE_INIMIGOS 3
#define QTD_BOMBA 3
#define MAX_RECORDE 2

typedef struct bomba{
	ALLEGRO_BITMAP *imagem_bomba;
	int coord_x;
	int coord_y;
	int relogio;
} bomba;
	
typedef struct player{
	ALLEGRO_BITMAP *imagem_jogador;
	int coord_x;
	int coord_y;
	int vida;
	bomba bomb[QTD_BOMBA];
} player;

int testeMapaVertical(int x){
	// função para testar se há paredes antes do movimento: 1 - não há, 0 - há parede
	int i, j;
	
	for(i=100; i<=700; i+=100)
		for(j=i; j<i+50; j++)
			if(x==j)
				return 0;	
	return 1;
}

int testeMapaHorizontal(int y){

	// função para testar se há paredes antes do movimento: 1 - não há, 0 - há parede
	int i, j;
	
	for(i=100; i<=500; i+=100)
		for(j=i; j<i+50; j++)
			if(y==j)
				return 0;
	
	return 1;
}

void moveMonstrosX(player *inimigo, bomba *bomba0, bomba *bomba1, bomba *bomba2){
	int sentido;
		//determina o sentido: 1 - positivo, 0 - negativo
		sentido = rand() % 2;
		if(sentido && inimigo->coord_x <= 700 && testeMapaHorizontal(inimigo->coord_y) && !( (inimigo->coord_x == bomba0->coord_x - PASSO && inimigo->coord_y == bomba0->coord_y) || (inimigo->coord_x == bomba1->coord_x - PASSO && inimigo->coord_y == bomba1->coord_y) || (inimigo->coord_x == bomba2->coord_x - PASSO && inimigo->coord_y == bomba2->coord_y) )){
			inimigo->imagem_jogador = al_load_bitmap("direita.i.png");
			inimigo->coord_x += PASSO;
		}
		else if (inimigo->coord_x >= 100 && testeMapaHorizontal(inimigo->coord_y) && !( (inimigo->coord_x == bomba0->coord_x + PASSO && inimigo->coord_y == bomba0->coord_y) || (inimigo->coord_x == bomba1->coord_x + PASSO && inimigo->coord_y == bomba1->coord_y) || (inimigo->coord_x == bomba2->coord_x + PASSO && inimigo->coord_y == bomba2->coord_y) )){
			inimigo->imagem_jogador = al_load_bitmap("esquerda.i.png");
			inimigo->coord_x -= PASSO;
		}
}

void moveMonstrosY(player *inimigo, bomba *bomba0, bomba *bomba1, bomba *bomba2){
	int sentido;
	sentido = rand() % 2;
		if(sentido && inimigo->coord_y < 550 && testeMapaVertical(inimigo->coord_x) && !( (inimigo->coord_x == bomba0->coord_x && inimigo->coord_y == bomba0->coord_y - PASSO) || (inimigo->coord_x == bomba1->coord_x && inimigo->coord_y == bomba1->coord_y - PASSO) || (inimigo->coord_x == bomba2->coord_x && inimigo->coord_y == bomba2->coord_y - PASSO) )){
			inimigo->imagem_jogador = al_load_bitmap("frente.i.png");
			inimigo->coord_y += PASSO;	
		}
		else if (inimigo->coord_y >= 100 && testeMapaVertical(inimigo->coord_x) && !( (inimigo->coord_x == bomba0->coord_x && inimigo->coord_y == bomba0->coord_y + PASSO) || (inimigo->coord_x == bomba1->coord_x && inimigo->coord_y == bomba1->coord_y + PASSO) || (inimigo->coord_x == bomba2->coord_x && inimigo->coord_y == bomba2->coord_y + PASSO) )){
			inimigo->imagem_jogador = al_load_bitmap("costas.i.png");
			inimigo->coord_y -= PASSO;
		}
}

void movePerseguidor(player *inimigo, player p1, int random_direita_esquerda, bomba *bomba0, bomba *bomba1, bomba *bomba2){
	
	random_direita_esquerda=rand()%2;
	//Testa se o monstro está na mesma linha que o p1
	if(inimigo->coord_x >= 100 && inimigo->coord_y == p1.coord_y && inimigo->coord_x > p1.coord_x && testeMapaHorizontal(inimigo->coord_y) && !( (inimigo->coord_x == bomba0->coord_x + PASSO && inimigo->coord_y == bomba0->coord_y) || (inimigo->coord_x == bomba1->coord_x + PASSO && inimigo->coord_y == bomba1->coord_y) || (inimigo->coord_x == bomba2->coord_x + PASSO && inimigo->coord_y == bomba2->coord_y) )){
		inimigo->imagem_jogador = al_load_bitmap("esquerda.p.png");
		inimigo->coord_x -= PASSO;
	}
	else if(inimigo->coord_x <= 700 && inimigo->coord_y == p1.coord_y && inimigo->coord_x< p1.coord_x && testeMapaHorizontal(inimigo->coord_y) && !( (inimigo->coord_x == bomba0->coord_x - PASSO && inimigo->coord_y == bomba0->coord_y) || (inimigo->coord_x == bomba1->coord_x - PASSO && inimigo->coord_y == bomba1->coord_y) || (inimigo->coord_x == bomba2->coord_x - PASSO && inimigo->coord_y == bomba2->coord_y) )){
		inimigo->imagem_jogador = al_load_bitmap("direita.p.png");
		inimigo->coord_x += PASSO;
	}
	//Testa se o monstro está na mesma coluna que o p1
	else if(inimigo->coord_y >= 100 && inimigo->coord_x == p1.coord_x && inimigo->coord_y>p1.coord_y && testeMapaVertical(inimigo->coord_x) && !( (inimigo->coord_x == bomba0->coord_x && inimigo->coord_y == bomba0->coord_y + PASSO) || (inimigo->coord_x == bomba1->coord_x && inimigo->coord_y == bomba1->coord_y + PASSO) || (inimigo->coord_x == bomba2->coord_x && inimigo->coord_y == bomba2->coord_y + PASSO) )){
		inimigo->imagem_jogador = al_load_bitmap("costas.p.png");
		inimigo->coord_y -= PASSO;
	}
	else if(inimigo->coord_y < 550 && inimigo->coord_x == p1.coord_x && inimigo->coord_y<p1.coord_y && testeMapaVertical(inimigo->coord_x) && !( (inimigo->coord_x == bomba0->coord_x && inimigo->coord_y == bomba0->coord_y - PASSO) || (inimigo->coord_x == bomba1->coord_x && inimigo->coord_y == bomba1->coord_y - PASSO) || (inimigo->coord_x == bomba2->coord_x && inimigo->coord_y == bomba2->coord_y - PASSO) )){
		inimigo->imagem_jogador = al_load_bitmap("frente.p.png");
		inimigo->coord_y += PASSO;
	}
	//Se não estiver em nenhum dos dois casos acima, faz ele aproximar por X ou Y
	else if(random_direita_esquerda && inimigo->coord_x <= 700 && inimigo->coord_x<p1.coord_x && testeMapaHorizontal(inimigo->coord_y) && !( (inimigo->coord_x == bomba0->coord_x - PASSO && inimigo->coord_y == bomba0->coord_y) || (inimigo->coord_x == bomba1->coord_x - PASSO && inimigo->coord_y == bomba1->coord_y) || (inimigo->coord_x == bomba2->coord_x - PASSO && inimigo->coord_y == bomba2->coord_y) )){
			inimigo->imagem_jogador = al_load_bitmap("direita.p.png");
			inimigo->coord_x += PASSO;
	}else if(random_direita_esquerda && inimigo->coord_x >= 100 && testeMapaHorizontal(inimigo->coord_y) && !( (inimigo->coord_x == bomba0->coord_x + PASSO && inimigo->coord_y == bomba0->coord_y) || (inimigo->coord_x == bomba1->coord_x + PASSO && inimigo->coord_y == bomba1->coord_y) || (inimigo->coord_x == bomba2->coord_x + PASSO && inimigo->coord_y == bomba2->coord_y) )){
			inimigo->imagem_jogador = al_load_bitmap("esquerda.p.png");
			inimigo->coord_x -= PASSO;
	}else if(!random_direita_esquerda && inimigo->coord_y < 550 && p1.coord_y > inimigo->coord_y && testeMapaVertical(inimigo->coord_x) && !( (inimigo->coord_x == bomba0->coord_x && inimigo->coord_y == bomba0->coord_y - PASSO) || (inimigo->coord_x == bomba1->coord_x && inimigo->coord_y == bomba1->coord_y - PASSO) || (inimigo->coord_x == bomba2->coord_x && inimigo->coord_y == bomba2->coord_y - PASSO) )){
			inimigo->imagem_jogador = al_load_bitmap("frente.p.png");
			inimigo->coord_y += PASSO;
	}else if(!random_direita_esquerda && inimigo->coord_y >= 100 && testeMapaVertical(inimigo->coord_x) && !( (inimigo->coord_x == bomba0->coord_x && inimigo->coord_y == bomba0->coord_y + PASSO) || (inimigo->coord_x == bomba1->coord_x && inimigo->coord_y == bomba1->coord_y + PASSO) || (inimigo->coord_x == bomba2->coord_x && inimigo->coord_y == bomba2->coord_y + PASSO) )){
		inimigo->imagem_jogador = al_load_bitmap("costas.p.png");
		inimigo->coord_y -= PASSO;	
	}
}

testeMorte(player *inimigo,  bomba *bomba0, bomba *bomba1, bomba *bomba2, int i){
	ALLEGRO_SAMPLE *matou = NULL;
	matou = al_load_sample("matou.wav");
	if(  (inimigo->coord_x == (bomba0->coord_x + 2*PASSO) && inimigo->coord_y == (bomba0->coord_y + PASSO) && bomba0->relogio==0) || (inimigo->coord_x == (bomba1->coord_x + 2*PASSO) && inimigo->coord_y == (bomba1->coord_y + PASSO) && bomba1->relogio==0) || (inimigo->coord_x == (bomba2->coord_x + 2*PASSO) && inimigo->coord_y == (bomba2->coord_y + PASSO) && bomba2->relogio==0) || (inimigo->coord_x == (bomba0->coord_x + PASSO) && inimigo->coord_y == bomba0->coord_y && bomba0->relogio==0) || (inimigo->coord_x == (bomba1->coord_x + PASSO) && inimigo->coord_y == bomba1->coord_y && bomba1->relogio==0) || (inimigo->coord_x == (bomba2->coord_x + PASSO) && inimigo->coord_y == bomba2->coord_y && bomba2->relogio==0) || (inimigo->coord_x == bomba0->coord_x && inimigo->coord_y == (bomba0->coord_y + PASSO) && bomba0->relogio==0) || (inimigo->coord_x == bomba1->coord_x && inimigo->coord_y == (bomba1->coord_y + PASSO) && bomba1->relogio==0) || (inimigo->coord_x == bomba2->coord_x && inimigo->coord_y == (bomba2->coord_y + PASSO) && bomba2->relogio==0) || (inimigo->coord_x == (bomba0->coord_x + PASSO) && inimigo->coord_y == (bomba0->coord_y +2*PASSO) && bomba0->relogio==0) || (inimigo->coord_x == (bomba1->coord_x + PASSO) && inimigo->coord_y == (bomba1->coord_y +2*PASSO) && bomba1->relogio==0) || (inimigo->coord_x == (bomba2->coord_x + PASSO) && inimigo->coord_y == (bomba2->coord_y +2*PASSO) && bomba2->relogio==0)) {
		inimigo->coord_x = 3000;
		inimigo->coord_y = 3000;		
		inimigo->vida = 0;
		al_play_sample(matou, 1.0, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
	}
 }
iniciaInimigo(player *inimigo){
	int teste = 1;
	inimigo->coord_x = (1 + (rand()%15) ) * PASSO;
	switch(inimigo->coord_x){
		case 100:
		case 200:
		case 300:
		case 400:
		case 500:
		case 600:
		case 700:
			do{
				inimigo->coord_y = (1 + (rand()%11) ) * PASSO;
				switch(inimigo->coord_y){
					case 100:
					case 200:
					case 300:
					case 400:
					case 500:
						break;
					default:
						teste = 0;
				}
			}while(teste);
			break;
		default:
			inimigo->coord_y = (1 + (rand()%11) ) * PASSO;
	}
	inimigo->vida = 1;
}

int main(){
	srand(time(NULL));
	
	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_BITMAP *imagem = NULL;
	ALLEGRO_EVENT_QUEUE *fila_eventos = NULL;
	ALLEGRO_TIMER *timer = NULL;
	ALLEGRO_SAMPLE *musica = NULL, *you_lose = NULL, *matou = NULL, *fogo = NULL, *passou_de_fase = NULL, *bomberman_audio = NULL;
	ALLEGRO_SAMPLE_ID musica_id;
	ALLEGRO_EVENT evento;
	
	player p1;
	player inimigo[3];
	int i, j=0, inicio_posicao_bomba = 1000, inicio_relogio = -2, cont_tempo_bomba = 0, cont_fase = 0, start = 1, comeca = 0, play_musica = 1;
	int cont = 0, random_direita_esquerda, jogar = 1, fase = 0, saiu_pelo_x = 0, recorde, teste_perseguidor = 1, move_fase_monstro = FPS/2;
	char leRecorde[MAX_RECORDE], my_text[100], texto_fase[8], texto_inicio[50];
	
	for(i=0; i< NUMERO_DE_INIMIGOS; i++){
		iniciaInimigo(&inimigo[i]);
	}
	
	//Posição inicial do imagem_jogador
	p1.coord_x = SCREEN_W / 17;
	p1.coord_y = SCREEN_H / 13;
	p1.vida = 1;
	p1.imagem_jogador = NULL;
	
	//Inicia as bombas fora do mapa
	for(i=0; i<QTD_BOMBA; i++){
		p1.bomb[i].imagem_bomba = NULL;
		p1.bomb[i].relogio = inicio_relogio;
		p1.bomb[i].coord_x = inicio_posicao_bomba;
		p1.bomb[i].coord_y = inicio_posicao_bomba;
	}
	
	//-----------ROTINAS DE INICIALIZAÇÃO-----------
	if (!al_init()){
        fprintf(stderr, "Falha ao inicializar a Allegro.\n");
        return -1;
    }
	
	if(!al_install_audio()){
		fprintf(stderr, "Falha ao inicializar o al_install_audio.\n");
        return -1;
	}
	
	if(!al_init_acodec_addon()){
		fprintf(stderr, "Falha ao inicializar o al_install_audio.\n");
        return -1;
	}
	
	if (!al_reserve_samples(6))
    {
        fprintf(stderr, "Falha ao alocar canais de áudio.\n");
        return false;
    }
	
	//Carrega os áudios
	musica = al_load_sample("musica.flac");
	you_lose = al_load_sample("youlose.wav");
	matou = al_load_sample("matou.wav");
	fogo = al_load_sample("fogo.wav");
	passou_de_fase = al_load_sample("passou_de_fase.wav");
	bomberman_audio = al_load_sample("bomberman_audio.wav");
	
    if (!al_init_image_addon()){
        fprintf(stderr, "Falha ao inicializar add-on allegro_image.\n");
        return -1;
    }
 
    display = al_create_display(SCREEN_W, SCREEN_H);
    if (!display){
        fprintf(stderr, "Falha ao criar janela.\n");
        return -1;
    }
	
	p1.imagem_jogador = al_load_bitmap("frente.b.png");
	if(!p1.imagem_jogador) {
      fprintf(stderr, "FALHA AO CRIAR O BITMAP imagem_jogador!\n");
      return -1;
   }
   
   for(i=0; i<NUMERO_DE_INIMIGOS; i++){
	   if(i!=2){
			inimigo[i].imagem_jogador = al_load_bitmap("frente.i.png");
	   }else inimigo[i].imagem_jogador = al_load_bitmap("frente.p.png");
		if(!inimigo[i].imagem_jogador) {
		fprintf(stderr, "FALHA AO CRIAR O BITMAP INIMIGO!\n");
		return -1;
		}
   }
   
	//Cria um bitmap para a bomba
	for(i=0; i<QTD_BOMBA; i++){
		p1.bomb[i].imagem_bomba = al_load_bitmap("bomba.png");
		if(!p1.bomb[i].imagem_bomba){
			fprintf(stderr, "FALHA AO CRIAR O BITMAP BOMBA!\n");
		return -1;
		}
	}
	
	timer = al_create_timer(1.0 / FPS);
	if(!timer) {
      fprintf(stderr, "FALHA AO CRIAR O TIMER!\n");
      return -1;
   }
   
    imagem = al_load_bitmap("mapa.png");
    if (!imagem){
        fprintf(stderr, "Falha ao carregar o arquivo de imagem.\n");
        return -1;
    }
 
    fila_eventos = al_create_event_queue();
    if (!fila_eventos){
        fprintf(stderr, "Falha ao criar fila de eventos.\n");
        return -1;
    }
	//--------------------------------------------------------------------------------------

	//Inicializa o modulo allegro que carrega as fontes
	al_init_font_addon();
	//Inicializa o modulo allegro que entende arquivos tff de fontes
	al_init_ttf_addon();
	//Carrega as fontes
	ALLEGRO_FONT *fonte = al_load_font("docktrin.ttf", 40, 1);
	ALLEGRO_FONT *fonte2 = al_load_font("ka1.ttf", 70, 1);

	al_install_keyboard();
	
	//Registra na fila de eventos que eu quero identificar quando a tela foi alterada
	al_register_event_source(fila_eventos, al_get_display_event_source(display));
	//Registra na fila de eventos que eu quero identificar quando o tempo alterou de t para t+1
	al_register_event_source(fila_eventos, al_get_timer_event_source(timer));
	//Registra que a fila de eventos deve detectar quando uma tecla for pressionada no teclado
	al_register_event_source(fila_eventos, al_get_keyboard_event_source());   
	
	//Inicia o temporizador
	al_start_timer(timer);
	
	al_set_window_title(display, "Bomberman");
	
	
	while(jogar){
	if(evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
		saiu_pelo_x = 1;
		break;
	}
	
	al_wait_for_event(fila_eventos, &evento);

		if(start){
			//Som inicial
			al_play_sample(bomberman_audio, 1.0, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
			//Tela inicial
			al_clear_to_color(al_map_rgb(0,0,0));
			sprintf(texto_inicio, "BOMBERMAN");
			al_draw_text(fonte2, al_map_rgb(255, 255, 255), SCREEN_W/2, SCREEN_H/6, ALLEGRO_ALIGN_CENTRE, texto_inicio);	
			sprintf(my_text, "Aperte enter para começar");
			al_draw_text(fonte, al_map_rgb(255, 255, 255), SCREEN_W/2, SCREEN_H/2 + 60, ALLEGRO_ALIGN_CENTRE, my_text);	
			sprintf(texto_inicio, "Movimentos: W, A, S, D / setas");
			al_draw_text(fonte, al_map_rgb(255, 255, 255), SCREEN_W/2, SCREEN_H - 100, ALLEGRO_ALIGN_CENTRE, texto_inicio);	
			sprintf(texto_inicio, "Bomba: Espaço");
			al_draw_text(fonte, al_map_rgb(255, 255, 255), SCREEN_W/2, SCREEN_H - 50, ALLEGRO_ALIGN_CENTRE, texto_inicio);	
			al_flip_display();
			start = 0;
		}
		
		//Se o tipo do evento for uma tecla pressionada
		if(evento.type == ALLEGRO_EVENT_KEY_DOWN) {
			//Verifica qual tecla foi apertada
			if(play_musica)
				switch(evento.keyboard.keycode) {
					case ALLEGRO_KEY_ENTER:
						comeca = 1;
						play_musica = 0;
						//Começa a tocara música em loop
						al_play_sample(musica, 1.0, 0.0,1.0,ALLEGRO_PLAYMODE_LOOP,&musica_id);
						break;
				}
			
			if(comeca){
				switch(evento.keyboard.keycode) {
					//Se a tecla for o W
					case ALLEGRO_KEY_W:
					case ALLEGRO_KEY_UP:
						if(p1.vida){
							p1.imagem_jogador = al_load_bitmap("costas.b.png");
							if(p1.coord_y >= 100 && testeMapaVertical(p1.coord_x) && !( (p1.coord_x == p1.bomb[0].coord_x && p1.coord_y == p1.bomb[0].coord_y + PASSO) || (p1.coord_x == p1.bomb[1].coord_x && p1.coord_y == p1.bomb[1].coord_y + PASSO) || (p1.coord_x == p1.bomb[2].coord_x && p1.coord_y == p1.bomb[2].coord_y + PASSO) ) )
								p1.coord_y -= PASSO;
						}
					break;
					//Se a tecla for o S
					case ALLEGRO_KEY_S:
					case ALLEGRO_KEY_DOWN:
						if(p1.vida){
							p1.imagem_jogador = al_load_bitmap("frente.b.png");
							if(p1.coord_y < SCREEN_H - 100 && testeMapaVertical(p1.coord_x) && !( (p1.coord_x == p1.bomb[0].coord_x && p1.coord_y == p1.bomb[0].coord_y - PASSO) || (p1.coord_x == p1.bomb[1].coord_x && p1.coord_y == p1.bomb[1].coord_y - PASSO) || (p1.coord_x == p1.bomb[2].coord_x && p1.coord_y == p1.bomb[2].coord_y - PASSO) ) )			
								p1.coord_y += PASSO;
						}
					break;
					//Se a tecla for o A
					case ALLEGRO_KEY_A:
					case ALLEGRO_KEY_LEFT:
						if(p1.vida){
							p1.imagem_jogador = al_load_bitmap("esquerda.b.png");
							if(p1.coord_x >= 100 && testeMapaHorizontal(p1.coord_y) && !( (p1.coord_x == p1.bomb[0].coord_x + PASSO && p1.coord_y == p1.bomb[0].coord_y) || (p1.coord_x == p1.bomb[1].coord_x + PASSO && p1.coord_y == p1.bomb[1].coord_y) || (p1.coord_x == p1.bomb[2].coord_x + PASSO && p1.coord_y == p1.bomb[2].coord_y) ) ) 
								p1.coord_x -= PASSO;
						}
					break;
					//Se a tecla for o D
					case ALLEGRO_KEY_D:
					case ALLEGRO_KEY_RIGHT:
						if(p1.vida){
							p1.imagem_jogador = al_load_bitmap("direita.b.png");
							if(p1.coord_x < SCREEN_W - 100 && testeMapaHorizontal(p1.coord_y)	 && !( (p1.coord_x == p1.bomb[0].coord_x - PASSO && p1.coord_y == p1.bomb[0].coord_y) || (p1.coord_x == p1.bomb[1].coord_x - PASSO && p1.coord_y == p1.bomb[1].coord_y) || (p1.coord_x == p1.bomb[2].coord_x - PASSO && p1.coord_y == p1.bomb[2].coord_y) ))
								p1.coord_x += PASSO;
						}
					break;
				
					case ALLEGRO_KEY_SPACE:
						if(p1.vida){
							if(p1.bomb[j].coord_x != 1000)
								break;
							p1.bomb[j].relogio = 3;
							p1.bomb[j].coord_x = p1.coord_x;
							p1.bomb[j].coord_y = p1.coord_y;
							if(j==2)
								j=0;
							else j++;
							break;
						}
				}//FINAL DO SEGUNDO SWITCH
			}//FINAL DO IF COMECA
		}//FINAL DO IF ALLEGRO_KEY_DOWN	

		if(comeca){
			//Se o tipo de evento for um evento do temporizador, ou seja, se o tempo passou de t para t+1
			if(evento.type == ALLEGRO_EVENT_TIMER) {
				al_draw_bitmap(imagem, 0, 0, 0);
				sprintf(texto_fase, "FASE: %d", fase+1);
				al_draw_text(fonte, al_map_rgb(255,255,255), 5, 5, ALLEGRO_ALIGN_LEFT, texto_fase);	
				cont_tempo_bomba++;
				cont++;
	
				if(cont_tempo_bomba >= FPS/2){
					for(i=0; i<QTD_BOMBA; i++){
						//Se o relogio for != do valor inicial do relogio, decresce 1 segundo de todas as bombas
						if(p1.bomb[i].relogio != inicio_relogio)
							p1.bomb[i].relogio--;
					}

					for(i=0; i<QTD_BOMBA; i++){
						//Se a bomba estourar, define qual imagem de fogo deve colocar, de acordo com a posição da bomba
						if(p1.bomb[i].relogio == 0){
							if(p1.bomb[i].coord_x == 50 && p1.bomb[i].coord_y == 50)
								p1.bomb[i].imagem_bomba = al_load_bitmap("fogo11.png");
							else if(p1.bomb[i].coord_x == 50 && p1.bomb[i].coord_y == 550)
								p1.bomb[i].imagem_bomba = al_load_bitmap("fogo9.png");
							else if(p1.bomb[i].coord_x == 750 && p1.bomb[i].coord_y == 50)
								p1.bomb[i].imagem_bomba = al_load_bitmap("fogo10.png");
							else if(p1.bomb[i].coord_x == 750 && p1.bomb[i].coord_y == 550)
								p1.bomb[i].imagem_bomba = al_load_bitmap("fogo8.png");
							else if(p1.bomb[i].coord_x == 50 && (p1.bomb[i].coord_y == 150 || p1.bomb[i].coord_y == 250 || p1.bomb[i].coord_y == 350 || p1.bomb[i].coord_y == 450))
								p1.bomb[i].imagem_bomba = al_load_bitmap("fogo5.png");
							else if(p1.bomb[i].coord_x == 50 && (p1.bomb[i].coord_y == 100 || p1.bomb[i].coord_y == 200 || p1.bomb[i].coord_y == 300 || p1.bomb[i].coord_y == 400))
								p1.bomb[i].imagem_bomba = al_load_bitmap("fogo2.png");
							else if(p1.bomb[i].coord_y == 550 && (p1.bomb[i].coord_x == 150 || p1.bomb[i].coord_x == 250 || p1.bomb[i].coord_x == 350 || p1.bomb[i].coord_x == 450 || p1.bomb[i].coord_x == 550 || p1.bomb[i].coord_x == 650))
								p1.bomb[i].imagem_bomba = al_load_bitmap("fogo4.png");
							else if(p1.bomb[i].coord_x == 750 && (p1.bomb[i].coord_y == 150 || p1.bomb[i].coord_y == 250 || p1.bomb[i].coord_y == 350 || p1.bomb[i].coord_y == 450))
								p1.bomb[i].imagem_bomba = al_load_bitmap("fogo7.png");
							else if(p1.bomb[i].coord_y == 50 && (p1.bomb[i].coord_x == 150 || p1.bomb[i].coord_x == 250 || p1.bomb[i].coord_x == 350 || p1.bomb[i].coord_x == 450 || p1.bomb[i].coord_x == 550 || p1.bomb[i].coord_x == 650))
								p1.bomb[i].imagem_bomba = al_load_bitmap("fogo6.png");
							else if(!testeMapaVertical(p1.bomb[i].coord_x))
								p1.bomb[i].imagem_bomba = al_load_bitmap("fogo3.png");
							else if(!testeMapaHorizontal(p1.bomb[i].coord_y))
								p1.bomb[i].imagem_bomba = al_load_bitmap("fogo2.png");
							else p1.bomb[i].imagem_bomba = al_load_bitmap("fogo1.png");
						
							al_play_sample(fogo, 1.0, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
							p1.bomb[i].coord_x -= 50; 
							p1.bomb[i].coord_y -= 50;
						}
						//Se o tempo da bomba for -1, teleporta para a posição 1000x1000
						if(p1.bomb[i].relogio == -1){
							p1.bomb[i].coord_x = 1000;
							p1.bomb[i].coord_y = 1000;
							p1.bomb[i].imagem_bomba = al_load_bitmap("bomba.png");
						}					
					}
					cont_tempo_bomba = 0;
				}//FINAL DO IF CONT_BOMBA
				
				//Testa se algum monstro morreu
				for(i=0; i<NUMERO_DE_INIMIGOS; i++)
					testeMorte(&inimigo[i],  &p1.bomb[0], &p1.bomb[1], &p1.bomb[2], i);

				//Move os monstros aleatoriamente, exceto 1, que persegue o p1
				if(cont>=move_fase_monstro){
					for(i=0; i<NUMERO_DE_INIMIGOS; i++){
						if(i!=2){
							if(inimigo[i].vida){
								random_direita_esquerda=rand()%2;
								if(random_direita_esquerda)
									moveMonstrosX(&inimigo[i], &p1.bomb[0], &p1.bomb[1], &p1.bomb[2]);
								else 
									moveMonstrosY(&inimigo[i], &p1.bomb[0], &p1.bomb[1], &p1.bomb[2]);
							}
							cont = 0;
						} else{
							movePerseguidor(&inimigo[i], p1, random_direita_esquerda, &p1.bomb[0], &p1.bomb[1], &p1.bomb[2]);
							cont = 0;
						}
					}//FINAL DO FOR NUMERO_DE_INIMIGOS
				}//FINAL DO IF MOVE MONSTROS
				//Testa se o p1 morreu
				for(i=0; i<NUMERO_DE_INIMIGOS; i++){
					if( (inimigo[i].coord_x == p1.coord_x && inimigo[i].coord_y == p1.coord_y ) || ( (p1.coord_x == (p1.bomb[i].coord_x + 2*PASSO) && p1.coord_y == (p1.bomb[i].coord_y + PASSO) && p1.bomb[i].relogio==0)  || (p1.coord_x == (p1.bomb[i].coord_x + PASSO) && p1.coord_y == p1.bomb[i].coord_y && p1.bomb[i].relogio==0) || (p1.coord_x == p1.bomb[i].coord_x && p1.coord_y == (p1.bomb[i].coord_y + PASSO) && p1.bomb[i].relogio==0) || (p1.coord_x == (p1.bomb[i].coord_x + PASSO) && p1.coord_y == (p1.bomb[i].coord_y +2*PASSO) && p1.bomb[i].relogio==0) || (p1.coord_x == (p1.bomb[i].coord_x + PASSO) && p1.coord_y == (p1.bomb[i].coord_y + PASSO) && p1.bomb[i].relogio==0))  ){
						p1.vida = 0;
						p1.imagem_jogador = al_load_bitmap("sangue.png");
						al_stop_sample(&musica_id);
						al_play_sample(you_lose, 1.0, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
						jogar = 0;
					}
				}
				//Desenha o imagem_jogador 1
				al_draw_bitmap(p1.imagem_jogador, p1.coord_x, p1.coord_y, 0);
				
				//Desenha os monstros
				for(i=0; i<NUMERO_DE_INIMIGOS; i++){
					al_draw_bitmap(inimigo[i].imagem_jogador, inimigo[i].coord_x, inimigo[i].coord_y, 0);
				}	
				//Desenha o p1 nas novas posicoes x e y
				al_draw_bitmap(p1.imagem_jogador, p1.coord_x, p1.coord_y, 0);
				//Se o imagem_jogador estiver morto, ele trava a tela por dois segundos (imagem do sangue)
				if(!jogar){
					al_flip_display();
					al_rest(2);
				}	
				//Desenha as bombas
				for(i=0; i<QTD_BOMBA; i++)
					al_draw_bitmap(p1.bomb[i].imagem_bomba, p1.bomb[i].coord_x, p1.bomb[i].coord_y, 0);
			}//final do ALLEGRO_EVENT_TIMER
			
			//Testa se todos os monstros estão mortos	
			if(inimigo[0].vida == 0 && inimigo[1].vida == 0 && inimigo[2].vida == 0){
				fase++;
				al_play_sample(passou_de_fase, 1.0, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
				//Aumenta a velocidade dos monstros em 10%
				move_fase_monstro -= 5;
				for(i=0; i<NUMERO_DE_INIMIGOS; i++){
					iniciaInimigo(&inimigo[i]);
				}
			}
			al_flip_display();
		}//FINAL DO IF COMECA
	}//FINAL DO WHILE
	
	if(!saiu_pelo_x){
		//Colore a tela de preto
		al_clear_to_color(al_map_rgb(0,0,0));
		//Abre o arquivo "recorde.txt" para leitura
		FILE *arq = fopen("recorde.txt", "r");

		//Le o recorde do arquivo e armazena na variável recorde
		fgets(leRecorde, MAX_RECORDE, arq);
		recorde = atoi(strtok(leRecorde, ""));
		
		//Testa se a quantidade de fases passadas é superior ao recorde salvo no arquivo .txt
		if(recorde<fase){
			recorde = fase;
			if(recorde==1){
				sprintf(my_text, "Novo recorde!!");
				al_draw_text(fonte, al_map_rgb(158, 158, 158), SCREEN_W/2, SCREEN_H/3 + 40, ALLEGRO_ALIGN_CENTRE, my_text);
				sprintf(my_text, "Voce passou %d fase", recorde);
				al_draw_text(fonte, al_map_rgb(158, 158, 158), SCREEN_W/2, SCREEN_H/3 + 120, ALLEGRO_ALIGN_CENTRE, my_text);;
				
			}else{ 
				sprintf(my_text, "Novo recorde!!");
				al_draw_text(fonte, al_map_rgb(158, 158, 158), SCREEN_W/2, SCREEN_H/3 + 40, ALLEGRO_ALIGN_CENTRE, my_text);
				sprintf(my_text, "Voce passou %d fases", recorde);
				al_draw_text(fonte, al_map_rgb(158, 158, 158), SCREEN_W/2, SCREEN_H/3 + 120, ALLEGRO_ALIGN_CENTRE, my_text);
				al_flip_display();
			}
			fclose(arq);
			arq = fopen("recorde.txt", "w");
			fprintf(arq, "%d",recorde);
			fclose(arq);
		}else if(fase==1){
			sprintf(my_text, "Voce passou %d fase", fase);
			al_draw_text(fonte, al_map_rgb(158, 158, 158), SCREEN_W/2, SCREEN_H/3 + 40, ALLEGRO_ALIGN_CENTRE, my_text);	
			sprintf(my_text, "recorde: %d", recorde);
			al_draw_text(fonte, al_map_rgb(158, 158, 158), SCREEN_W/2, SCREEN_H/2 + 40, ALLEGRO_ALIGN_CENTRE, my_text);
			fclose(arq);
		}else{
			sprintf(my_text, "Voce passou: %d fases", fase);
			al_draw_text(fonte, al_map_rgb(158, 158, 158), SCREEN_W/2, SCREEN_H/3 + 40, ALLEGRO_ALIGN_CENTRE, my_text);	
			sprintf(my_text, "recorde: %d", recorde);
			al_draw_text(fonte, al_map_rgb(158, 158, 158), SCREEN_W/2, SCREEN_H/2 + 40, ALLEGRO_ALIGN_CENTRE, my_text);
			fclose(arq);
		}
		al_flip_display();
		al_rest(4);
	}//FINAL DO IF SAIU_PELO_X
	
	//Finalização
	al_destroy_sample(matou);
	al_destroy_sample(you_lose);
	al_destroy_sample(musica);
	al_destroy_sample(fogo);
	al_destroy_sample(passou_de_fase);
	al_destroy_sample(bomberman_audio);
	al_destroy_font(fonte);
	al_destroy_font(fonte2);
	al_destroy_display(display);
	al_destroy_event_queue(fila_eventos);
	al_destroy_bitmap(imagem);
	al_destroy_bitmap(p1.imagem_jogador);
	for(i=0; i<NUMERO_DE_INIMIGOS; i++){
		al_destroy_bitmap(inimigo[i].imagem_jogador);
	}
	for(i=0; i<QTD_BOMBA; i++){
		al_destroy_bitmap(p1.bomb[i].imagem_bomba);
	}
	al_destroy_timer(timer);
	
	return 0;
}
