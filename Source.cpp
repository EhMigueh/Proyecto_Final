//Bibliotecas.
#include<time.h>
#include<stdio.h>
#include<stdlib.h>
#include<allegro5/file.h>
#include<allegro5/allegro.h>
#include<allegro5/allegro_ttf.h>
#include<allegro5/allegro_font.h>
#include<allegro5/allegro_image.h>
#include<allegro5/allegro_primitives.h>
#include<allegro5/allegro_native_dialog.h>

//Valores definidos.
#define ANCHO_RES 1600
#define ALTO_RES 800
#define ANCHO_OBRERO 32
#define ALTO_OBRERO 32
#define X_ARREGLO 41
#define Y_ARREGLO 20
#define TAM_RECURSO 200
#define CANT_ENEMIGOS 96
#define MAX_TORRES 50
#define MAX_MUNICION 1000

//Declaraciones de ALLEGRO globales.
ALLEGRO_DISPLAY* ventana = NULL;
ALLEGRO_EVENT_QUEUE* eventos = NULL;
ALLEGRO_TIMER* temporizador = NULL;
ALLEGRO_BITMAP* municion1 = NULL;
ALLEGRO_BITMAP* obrero_der = NULL;
ALLEGRO_BITMAP* obrero_izq = NULL;
ALLEGRO_BITMAP* obrero_ar = NULL;
ALLEGRO_BITMAP* obrero_ab = NULL;
ALLEGRO_BITMAP* muro = NULL;
ALLEGRO_BITMAP* camino = NULL;
ALLEGRO_BITMAP* cesped = NULL;
ALLEGRO_BITMAP* arbol = NULL;
ALLEGRO_BITMAP* roca = NULL;
ALLEGRO_BITMAP* nucleo = NULL;
ALLEGRO_BITMAP* nucleo_b = NULL;
ALLEGRO_BITMAP* t_arq_1 = NULL;
ALLEGRO_BITMAP* t_arq_2 = NULL;
ALLEGRO_BITMAP* t_arq_3 = NULL;
ALLEGRO_BITMAP* t_mag_1 = NULL;
ALLEGRO_BITMAP* t_mag_2 = NULL;
ALLEGRO_BITMAP* t_mag_3 = NULL;
ALLEGRO_BITMAP* goblin = NULL;
ALLEGRO_FONT* fuente_pos = NULL;
ALLEGRO_FONT* fuente_piedra = NULL;
ALLEGRO_FONT* fuente_madera = NULL;
ALLEGRO_FONT* fuente_hp = NULL;
ALLEGRO_FONT* fuente_derrota = NULL;
ALLEGRO_FILE* mapa = al_fopen("mapa.txt", "r");

//Declaraciones tipo booleanas globales.
bool ejecutandose = true;
bool recolectando = false;

//Declaraciones tipo enteras globales.
int tecla_ar = false;
int tecla_ab = false;
int tecla_izq = false;
int tecla_der = false;
int cont_piedra = 300;
int cont_madera = 300;
int hp_torre = 5000;

//Declaraciones tipo tiempo globales.
time_t tiempo_ult_rec = 0;
time_t tiempo_apar_enem = 0;
//time_t tiempo_ultimo_disparo = 0;

//Estructuras.
struct _personaje
{
	int x, y;
	float vel;
	bool dir;
};
typedef struct _personaje personaje;
personaje obrero = { ANCHO_RES / 2 - ANCHO_OBRERO / 2, ALTO_RES / 2 - ALTO_OBRERO / 2, 2, false};
struct _enemigo
{
	int x, y, vida;
	float vel;
	bool vivo;
};
typedef struct _enemigo enemigo;
struct _recurso
{
	int x, y, cant;
};
typedef struct _recurso recurso;
struct _municion
{
	int x, y;
	bool colision = false;
	char sentido[1];
};
typedef struct _municion municion;
struct _edificio
{
	int x, y, nivel;
	int tipo;
	char lugar[1];
};
typedef struct _edificio edificio;

//Funciones.
void lector_mapa(char archivo[Y_ARREGLO][X_ARREGLO]);
void inicializar_recursos(char archivo[Y_ARREGLO][X_ARREGLO], recurso bosque[TAM_RECURSO], recurso pedreria[TAM_RECURSO], int *contador_arboles, int *contador_rocas);
void colision_bordes();
void colision_objetos(char archivo[Y_ARREGLO][X_ARREGLO]);
void dibujar_objetos(char archivo[Y_ARREGLO][X_ARREGLO],ALLEGRO_BITMAP* municiongraf1, ALLEGRO_BITMAP* muro, ALLEGRO_BITMAP* camino, ALLEGRO_BITMAP* cesped, ALLEGRO_BITMAP* arbol, ALLEGRO_BITMAP* roca, ALLEGRO_BITMAP* nucleo, ALLEGRO_BITMAP* nucleo_b, ALLEGRO_BITMAP* t_arq_1, ALLEGRO_BITMAP* t_mag_1, ALLEGRO_BITMAP* goblin);
void dibujar_obrero();
void dibujar_textos(ALLEGRO_FONT* fuente_posicion, ALLEGRO_FONT* fuente_piedra, ALLEGRO_FONT* fuente_madera, ALLEGRO_FONT* fuente_hp);
void construir_torre_arqueros(char archivo[Y_ARREGLO][X_ARREGLO], edificio torres[MAX_TORRES], int *contador_torres);
void construir_torre_magos(char archivo[Y_ARREGLO][X_ARREGLO], edificio torres[MAX_TORRES], int* contador_torres);
int buscar_recurso(int x_mapa, int y_mapa, recurso tipo_recurso[TAM_RECURSO]);
void recolectar_piedra(char archivo[Y_ARREGLO][X_ARREGLO], recurso pedreria[TAM_RECURSO]);
void recolectar_madera(char archivo[Y_ARREGLO][X_ARREGLO], recurso bosque[TAM_RECURSO]);
void proceso_teclado_input(ALLEGRO_EVENT juego, char archivo[Y_ARREGLO][X_ARREGLO], edificio torres[MAX_TORRES], int *contador_torres);
void proceso_teclado_output(ALLEGRO_EVENT juego);
int inicializar_enemigos(char archivo[Y_ARREGLO][X_ARREGLO], enemigo goblin1[CANT_ENEMIGOS], int cantidad_enemigos_actuales);
int inicializar_edificios(char archivo[Y_ARREGLO][X_ARREGLO], edificio edificios1[MAX_TORRES], int cantidad_torres_actuales);
int inicializar_municiones(char archivo[Y_ARREGLO][X_ARREGLO], municion edificios1[MAX_MUNICION], int* cantidad_municiones_actuales, char sentido, int x, int y);
void dibujar_proyectiles(char archivo[Y_ARREGLO][X_ARREGLO], municion municiones[MAX_MUNICION]);
void dibujar_enemigos(char archivo[Y_ARREGLO][X_ARREGLO], enemigo goblin1[CANT_ENEMIGOS]);
void dibujar_perdida();

//Función principal.
int main(void) {
	//Declaraciónes tipo enteras locales.
	int cantidad_municiones_actuales = 0;
	int cantidad_torres_actuales = 0;
	int cantidad_enemigos_actuales = 0;
	int contador_arboles = 0;
	int contador_rocas = 0;
	int contador_torres = 0;
	int velocidad = 0;
	int velocidad_bala = 0;
	int i, j, k;

	//Declaraciones tipo caracter locales.
	char archivo[Y_ARREGLO][X_ARREGLO];

	//Declaraciones de estructuras locales.
	recurso bosque[TAM_RECURSO];
	recurso pedreria[TAM_RECURSO];
	enemigo goblin1[CANT_ENEMIGOS];
	edificio torres[MAX_TORRES];
	municion municiones[MAX_MUNICION];

	//Inicialización de las variales de tipo tiempo.
	tiempo_ult_rec = time(NULL);
	tiempo_apar_enem= time(NULL);
	//tiempo_ultimo_disparo = time(NULL);

	//Errores e inicializaciones de ALLEGRO.
	if (!al_init()) {
		printf("Error al inicializar ALLEGRO.");
		return -1;
	}

	//Inicializaciones de ALLEGRO.
	ventana = al_create_display(ANCHO_RES, ALTO_RES);
	eventos = al_create_event_queue();
	al_install_keyboard();
	temporizador = al_create_timer(1.0 / 60);
	al_init_image_addon();
	municion1 = al_load_bitmap("bala.png");
	obrero_der = al_load_bitmap("obrero_der.png");
	obrero_izq = al_load_bitmap("obrero_izq.png");
	obrero_ar = al_load_bitmap("obrero_ar.png");
	obrero_ab = al_load_bitmap("obrero_ab.png");
	muro = al_load_bitmap("muro.png");
	camino = al_load_bitmap("camino.png");
	cesped = al_load_bitmap("cesped.png");
	arbol = al_load_bitmap("arbol.png");
	roca = al_load_bitmap("roca.png");
	nucleo = al_load_bitmap("nucleo.png");
	nucleo_b = al_load_bitmap("nucleo_b.png");
	t_arq_1 = al_load_bitmap("t_arq_1.png");
	t_arq_2 = al_load_bitmap("t_arq_2.png");
	t_arq_3 = al_load_bitmap("t_arq_3.png");
	t_mag_1 = al_load_bitmap("t_mag_1.png");
	t_mag_2 = al_load_bitmap("t_mag_2.png");
	t_mag_3 = al_load_bitmap("t_mag_3.png");
	goblin = al_load_bitmap("goblin.png");
	al_init_font_addon();
	al_init_ttf_addon();
	fuente_pos = al_load_font("arial.ttf", 20, 0);
	fuente_piedra = al_load_font("arial.ttf", 20, 0);
	fuente_madera = al_load_font("arial.ttf", 20, 0);
	fuente_hp = al_load_font("arial.ttf", 20, 0);
	fuente_derrota = al_load_font("arial.ttf", 40, 0);

	//Posibles errores al inicializar ALLEGRO.
	if (!ventana || !al_install_keyboard() || !eventos) {
		printf("Error al inicializar la ventana, la cola de eventos o instalar teclado.");
		al_destroy_display(ventana);
		return -1;
	}

	if (!temporizador) {
		printf("Error al inicializar temporizador");
		al_destroy_event_queue(eventos);
		al_destroy_display(ventana);
		return -1;
	}

	if (!obrero_der || !obrero_izq || !obrero_ar || !obrero_ab || !muro || !camino || !cesped || !arbol || !roca || !nucleo || !nucleo_b || !t_arq_1 || !t_arq_2 || !t_arq_3 || !t_mag_1 || !t_mag_2 || !t_mag_3 || !goblin || !municion1) {
		printf("Error al inicializar un dibujo dentro del mapa");
		al_destroy_timer(temporizador);
		al_destroy_event_queue(eventos);
		al_destroy_display(ventana);
		return -1;
	}

	if (!fuente_pos || !fuente_piedra || !fuente_madera || !fuente_derrota) {
		printf("Error al inicializar la fuente de la posición, piedra o madera");
		al_destroy_timer(temporizador);
		al_destroy_event_queue(eventos);
		al_destroy_display(ventana);
		return -1;
	}

	//Registro de eventos.
	al_register_event_source(eventos, al_get_display_event_source(ventana));
	al_register_event_source(eventos, al_get_timer_event_source(temporizador));
	al_register_event_source(eventos, al_get_keyboard_event_source());
	al_set_window_title(ventana, "Protect the CORE");
	al_flip_display();
	al_start_timer(temporizador);

	//Llamada función que lee el mapa.
	lector_mapa(archivo);

	//Llamada función que inicializa los recursos.
	inicializar_recursos(archivo, bosque, pedreria, &contador_arboles, &contador_rocas);


	//Ejecución del juego en loop.
	while (ejecutandose)
	{
		//Creacion evento juego.
		ALLEGRO_EVENT juego;
		al_wait_for_event(eventos, &juego);

		//Cuerpo juego.
		if (juego.type == ALLEGRO_EVENT_TIMER)
		{
			//Llamada función de colisión con bordes.
			colision_bordes();

			//Limpieza de los dibujos.
			al_clear_to_color(al_map_rgb(0, 0, 0));

			//Llamada función para dibujar objetos.
			dibujar_objetos(archivo,municion1, muro, camino, cesped, arbol, roca, nucleo, nucleo_b, t_arq_1, t_mag_1, goblin);

			//EN PROGRESO, ENEMIGOS.
			time_t tiempo_actual_enemigo = time(NULL);
			if (tiempo_actual_enemigo - tiempo_apar_enem >= 5) {
				cantidad_enemigos_actuales = inicializar_enemigos(archivo, goblin1, cantidad_enemigos_actuales);
				tiempo_apar_enem = tiempo_actual_enemigo;
			}
			dibujar_enemigos(archivo, goblin1);
			dibujar_proyectiles(archivo, municiones);
			if (ALLEGRO_EVENT_TIMER == juego.type) {
				velocidad = (velocidad + 1) % 60;
				velocidad_bala = (velocidad_bala + 1) % 120;
				if (velocidad == 0) {
					for (j = 0; j < cantidad_municiones_actuales; j++) {
						if (municiones != NULL) {
							if (archivo[municiones[j].y][municiones[j].x] != 'm') {
								switch (municiones[j].sentido[0]) {
									case 'd': municiones[j].x++; break;
									case 'i': municiones[j].x--; break;
									default: break;
								}
							}
							if (archivo[municiones[j].y][municiones[j].x] == 'm' || archivo[municiones[j].y][municiones[j].x] == 'k') {
								municiones[j].colision = true;
							}
						}
					}
					for (i = 0; i < cantidad_enemigos_actuales; i++) {
						if (archivo[goblin1[i].y][goblin1[i].x] != 'n') {
							goblin1[i].y --;
						}
						else if (archivo[goblin1[i].y][goblin1[i].x ] == 'n') {
							if (goblin1[i].vivo) {
								hp_torre -= 50;
							}
							goblin1[i].vivo = false;
							//Cambiar el sprite por un nucleo destruido, o por una explosión.
						}
						if (archivo[goblin1[i].y][goblin1[i].x] == 'b') {
							archivo[goblin1[i].y][goblin1[i].x] = 't';
							goblin1[i].vivo = false;
						}
						for (j = 0; j < cantidad_municiones_actuales; j++) {
							if (goblin1[i].vivo == true && municiones[j].colision == false && goblin1[i].x == municiones[j].x && goblin1[i].y == municiones[j].y) {
								municiones[j].colision = true;
								goblin1[i].vivo = false;
								goblin1[i].vida = 0;
							}
						}
					}
				}
				if (velocidad_bala == 0) {
					for (k = 0; k < contador_torres; k++) {
						//disparo del proyectil
						if (torres != NULL) {
							if (torres[k].lugar[0] == 'd') {
								if (archivo[torres[k].y][torres[k].x - 1] == 'm') {
									archivo[torres[k].y][torres[k].x - 1] = 'k';
								}
								inicializar_municiones(archivo, municiones, &cantidad_municiones_actuales,'i', torres[k].x, torres[k].y);
							}
							else {
								if (archivo[torres[k].y][torres[k].x + 1] == 'm') {
									archivo[torres[k].y][torres[k].x + 1] = 'k';
								}
								inicializar_municiones(archivo, municiones, &cantidad_municiones_actuales,'d', torres[k].x, torres[k].y);
							}
						}
					}
				}
			}
			if (hp_torre <= 0) {
				hp_torre = 0;
				dibujar_perdida();
			}

			//Llamada función para dibujar obrero.
			dibujar_obrero();

			//Llamada función para dibujar textos.
			dibujar_textos(fuente_pos, fuente_piedra, fuente_madera, fuente_hp);

			//Llamada a funciones de recolección.
			recolectar_madera(archivo, bosque);
			recolectar_piedra(archivo, pedreria);

			//Actualización ventana.
			al_flip_display();

			//Llamada a función encargada de colisión con objetos.
			colision_objetos(archivo);
		}

		//Cerrar juego.
		else if (juego.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
		{
			ejecutandose = false;
		}

		//Llamada  a función interacción  de entrada de teclado.
		else if (juego.type == ALLEGRO_EVENT_KEY_DOWN)
		{
			proceso_teclado_input(juego, archivo, torres, &contador_torres);
		}

		//LLamada a función interaccion de salida de teclado.
		else if (juego.type == ALLEGRO_EVENT_KEY_UP)
		{
			proceso_teclado_output(juego);
		}
	}

	//Liberación de memoria.
	al_destroy_bitmap(obrero_der);
	al_destroy_bitmap(obrero_izq);
	al_destroy_bitmap(obrero_ar);
	al_destroy_bitmap(obrero_ab);
	al_destroy_bitmap(muro);
	al_destroy_bitmap(camino);
	al_destroy_bitmap(cesped);
	al_destroy_bitmap(arbol);
	al_destroy_bitmap(roca);
	al_destroy_bitmap(nucleo);
	al_destroy_bitmap(nucleo_b);
	al_destroy_bitmap(t_arq_1);
	al_destroy_bitmap(t_arq_2);
	al_destroy_bitmap(t_arq_3);
	al_destroy_bitmap(t_mag_1);
	al_destroy_bitmap(t_mag_2);
	al_destroy_bitmap(t_mag_3);
	al_destroy_bitmap(goblin);
	al_destroy_bitmap(municion1);
	al_destroy_timer(temporizador);
	al_destroy_event_queue(eventos);
	al_destroy_display(ventana);
	al_destroy_font(fuente_pos);
	al_destroy_font(fuente_piedra);
	al_destroy_font(fuente_madera);
	al_destroy_font(fuente_derrota);

	return (EXIT_SUCCESS);
}

//Función para leer el mapa.
void lector_mapa(char archivo[Y_ARREGLO][X_ARREGLO]) {
	int x_mapa, y_mapa;
	for (y_mapa = 0; y_mapa < Y_ARREGLO; y_mapa++) {
		for (x_mapa = 0; x_mapa < X_ARREGLO; x_mapa++) {
			archivo[y_mapa][x_mapa] = al_fgetc(mapa);
		}
	}
	al_fclose(mapa);
}

//Función para inicializar recursos.
void inicializar_recursos(char archivo[Y_ARREGLO][X_ARREGLO], recurso bosque[TAM_RECURSO], recurso pedreria[TAM_RECURSO], int *contador_arboles, int *contador_rocas) {
	int y_mapa, x_mapa;
	for (y_mapa = 0; y_mapa < Y_ARREGLO; y_mapa++) {
		for (x_mapa = 0; x_mapa < X_ARREGLO; x_mapa++) {
			if (archivo[y_mapa][x_mapa] == 'a') {
				bosque[*contador_arboles].y = y_mapa;
				bosque[*contador_arboles].x = x_mapa;
				bosque[*contador_arboles].cant = 30;
				(*contador_arboles)++;
			}
			if (archivo[y_mapa][x_mapa] == 'r') {
				pedreria[*contador_rocas].y = y_mapa;
				pedreria[*contador_rocas].x = x_mapa;
				pedreria[*contador_rocas].cant = 20;
				(*contador_rocas)++;
			}
		}
	}
}

//Función encargada de la colision con los bordes.
void colision_bordes() {
	if (tecla_ar && obrero.y > 0) {
		obrero.y -= obrero.vel;
	}
	if (tecla_ab && obrero.y + ALTO_OBRERO < ALTO_RES) {
		obrero.y += obrero.vel;
	}
	if (tecla_izq && obrero.x > 0) {
		obrero.x -= obrero.vel;
		obrero.dir = false;
	}
	if (tecla_der && obrero.x + ANCHO_OBRERO< ANCHO_RES) {
		obrero.x += obrero.vel;
		obrero.dir = true;
	}
}

//Colisión con objetos.
void colision_objetos(char archivo[Y_ARREGLO][X_ARREGLO]) {
	int x_act, y_act, x_sig, y_sig;
	if (tecla_ar && obrero.y > 0) {
		y_sig = (obrero.y - obrero.vel) / 40;
		x_act = obrero.x / 40;
		if (archivo[y_sig][x_act] != 'm' && archivo[y_sig][x_act] != 'a' && archivo[y_sig][x_act] != 'r' && archivo[y_sig][(obrero.x + ANCHO_OBRERO - 1) / 40] != 'm' && archivo[y_sig][(obrero.x + ANCHO_OBRERO - 1) / 40] != 'a' && archivo[y_sig][(obrero.x + ANCHO_OBRERO - 1) / 40] != 'r' ) {
			obrero.y -= obrero.vel;
		}
		else {
			obrero.y = (y_sig + 1) * 40;
		}
	}
	if (tecla_ab && obrero.y + ALTO_OBRERO< ALTO_RES) {
		y_sig = (obrero.y + obrero.vel + ALTO_OBRERO) / 40;
		x_act = obrero.x / 40;
		if (archivo[y_sig][x_act] != 'm' && archivo[y_sig][x_act] != 'a' && archivo[y_sig][x_act] != 'r' && archivo[y_sig][(obrero.x + ANCHO_OBRERO - 1) / 40] != 'm' && archivo[y_sig][(obrero.x + ANCHO_OBRERO - 1) / 40] != 'a' && archivo[y_sig][(obrero.x + ANCHO_OBRERO - 1) / 40] != 'r') {
			obrero.y += obrero.vel;
		}
		else {
			obrero.y = y_sig * 40 - ALTO_OBRERO;
		}
	}
	if (tecla_izq && obrero.x > 0) {
		y_act = obrero.y / 40;
		x_sig = (obrero.x - obrero.vel) / 40;
		if (archivo[y_act][x_sig] != 'm' && archivo[y_act][x_sig] != 'a' && archivo[y_act][x_sig] != 'r' && archivo[(obrero.y + ALTO_OBRERO - 1) / 40][x_sig] != 'm' && archivo[(obrero.y + ALTO_OBRERO - 1) / 40][x_sig] != 'a' && archivo[(obrero.y + ALTO_OBRERO - 1) / 40][x_sig] != 'r') {
			obrero.x -= obrero.vel;
		}
		else {
			obrero.x = (x_sig + 1) * 40;
		}
	}
	if (tecla_der && obrero.x + ANCHO_OBRERO < ANCHO_RES) {
		y_act = obrero.y / 40;
		x_sig = (obrero.x + obrero.vel + ANCHO_OBRERO) / 40;
		if (archivo[y_act][x_sig] != 'm' && archivo[y_act][x_sig] != 'a' && archivo[y_act][x_sig] != 'r' && archivo[(obrero.y + ALTO_OBRERO - 1) / 40][x_sig] != 'm' && archivo[(obrero.y + ALTO_OBRERO - 1) / 40][x_sig] != 'a' && archivo[(obrero.y + ALTO_OBRERO - 1) / 40][x_sig] != 'r') {
			obrero.x += obrero.vel;
		}
		else {
			obrero.x = x_sig * 40 - ANCHO_OBRERO;
		}
	}
}

//Función para dibujar objetos.
void dibujar_objetos(char archivo[Y_ARREGLO][X_ARREGLO], ALLEGRO_BITMAP* municion1, ALLEGRO_BITMAP* muro, ALLEGRO_BITMAP* camino, ALLEGRO_BITMAP* cesped, ALLEGRO_BITMAP* arbol, ALLEGRO_BITMAP* roca, ALLEGRO_BITMAP* nucleo, ALLEGRO_BITMAP* nucleo_b, ALLEGRO_BITMAP* t_arq_1, ALLEGRO_BITMAP* t_mag_1, ALLEGRO_BITMAP* goblin) {
	int x_mapa, y_mapa;
	for (y_mapa = 0; y_mapa < Y_ARREGLO; y_mapa++) {
		for (x_mapa = 0; x_mapa < X_ARREGLO; x_mapa++) {
			if (archivo[y_mapa][x_mapa] == 'm')
			{
				al_draw_bitmap(muro, x_mapa * 40, y_mapa * 40, 0);
			}
			else if (archivo[y_mapa][x_mapa] == 't')
			{
				al_draw_bitmap(camino, x_mapa * 40, y_mapa * 40, 0);
			}
			else if (archivo[y_mapa][x_mapa] == 'c')
			{
				al_draw_bitmap(cesped, x_mapa * 40, y_mapa * 40, 0);
			}
			else if (archivo[y_mapa][x_mapa] == 'a')
			{
				al_draw_bitmap(cesped, x_mapa * 40, y_mapa * 40, 0);
				al_draw_bitmap(arbol, x_mapa * 40, y_mapa * 40, 0);
			}
			else if (archivo[y_mapa][x_mapa] == 'r')
			{
				al_draw_bitmap(cesped, x_mapa * 40, y_mapa * 40, 0);
				al_draw_bitmap(roca, x_mapa * 40, y_mapa * 40, 0);
			}
			else if (archivo[y_mapa][x_mapa] == 'n')
			{
				al_draw_bitmap(camino, x_mapa * 40, y_mapa * 40, 0);
				al_draw_bitmap(nucleo, x_mapa * 40, y_mapa * 40, 0);
			}
			else if (archivo[y_mapa][x_mapa] == 'b')
			{
				al_draw_bitmap(camino, x_mapa * 40, y_mapa * 40, 0);
				al_draw_bitmap(nucleo_b, x_mapa * 40, y_mapa * 40, 0);
			}
			else if (archivo[y_mapa][x_mapa] == 'o')
			{
				al_draw_bitmap(cesped, x_mapa * 40, y_mapa * 40, 0);
			}
			else if (archivo[y_mapa][x_mapa] == 'z' || archivo[y_mapa][x_mapa] == 'v')
			{
				al_draw_bitmap(cesped, x_mapa * 40, y_mapa * 40, 0);
			}
			else if (archivo[y_mapa][x_mapa] == 'h')
			{
				al_draw_bitmap(cesped, x_mapa * 40, y_mapa * 40, 0);
				al_draw_bitmap(t_arq_1, x_mapa * 40, y_mapa * 40, 0);
			}
			else if (archivo[y_mapa][x_mapa] == 'j')
			{
				al_draw_bitmap(cesped, x_mapa * 40, y_mapa * 40, 0);
				al_draw_bitmap(t_mag_1, x_mapa * 40, y_mapa * 40, 0);
			}
			else if (archivo[y_mapa][x_mapa] == 'x')
			{
				al_draw_bitmap(camino, x_mapa * 40, y_mapa * 40, 0);
			}
			else if (archivo[y_mapa][x_mapa] == 'k') {
				al_draw_bitmap(muro, x_mapa * 40, y_mapa * 40, 0);
			}
			else if (archivo[y_mapa][x_mapa] == 'l') {
				al_draw_bitmap(camino, x_mapa * 40, y_mapa * 40, 0);
			}
		}
	}
}

//Función para dibujar obrero.
void dibujar_obrero() {
	if (obrero.dir) {
		if (tecla_ar) {
			al_draw_bitmap(obrero_ar, obrero.x, obrero.y, 0);
		}
		else if (tecla_ab) {
			al_draw_bitmap(obrero_ab, obrero.x, obrero.y, 0);
		}
		else {
			al_draw_bitmap(obrero_der, obrero.x, obrero.y, 0);
		}
	}
	else {
		if (tecla_ar) {
			al_draw_bitmap(obrero_ar, obrero.x, obrero.y, 0);
		}
		else if (tecla_ab) {
			al_draw_bitmap(obrero_ab, obrero.x, obrero.y, 0);
		}
		else {
			al_draw_bitmap(obrero_izq, obrero.x, obrero.y, 0);
		}
	}
}

//Función para dibujar textos.
void dibujar_textos(ALLEGRO_FONT* fuente_pos, ALLEGRO_FONT* fuente_piedra, ALLEGRO_FONT* fuente_madera, ALLEGRO_FONT* fuente_hp) {
	char posicion_texto[50], conpiedra_texto[50], conmadera_texto[50], hp_texto[50];
	snprintf(posicion_texto, sizeof(posicion_texto), "Posicion: (%d, %d)", obrero.x, obrero.y);
	al_draw_text(fuente_pos, al_map_rgb(255, 0, 0), 10, 10, 0, posicion_texto);
	snprintf(conpiedra_texto, sizeof(conpiedra_texto), "Piedra: %d", cont_piedra);
	al_draw_text(fuente_piedra, al_map_rgb(255, 0, 0), 10, 30, 0, conpiedra_texto);
	snprintf(conmadera_texto, sizeof(conmadera_texto), "Madera: %d", cont_madera);
	al_draw_text(fuente_madera, al_map_rgb(255, 0, 0), 10, 50, 0, conmadera_texto);
	snprintf(hp_texto, sizeof(hp_texto), "HP: %d", hp_torre);
	al_draw_text(fuente_hp, al_map_rgb(255, 0, 0), 10, 70, 0, hp_texto);
}

//Función para construir torre de arqueros.
void construir_torre_arqueros(char archivo[Y_ARREGLO][X_ARREGLO], edificio torres[MAX_TORRES], int *contador_torres) {
	int x_mapa = obrero.x / 40, y_mapa = obrero.y / 40;
	if ((archivo[y_mapa][x_mapa] == 'z' || archivo[y_mapa][x_mapa] == 'v') && cont_madera >= 30 && cont_piedra >= 5 && *contador_torres < MAX_TORRES) {
		cont_madera -= 30;
		cont_piedra -= 5;
		torres[*contador_torres].x = x_mapa;
		torres[*contador_torres].y = y_mapa;
		torres[*contador_torres].tipo = 0;
		if (archivo[y_mapa][x_mapa] == 'z') {
			torres[*contador_torres].lugar[0] = 'i';
		}
		else {
			torres[*contador_torres].lugar[0] = 'd';
		}
		archivo[y_mapa][x_mapa] = 'h';
		(*contador_torres)++;
	}
}

//Función para construir torre de magos.
void construir_torre_magos(char archivo[Y_ARREGLO][X_ARREGLO], edificio torres[MAX_TORRES], int *contador_torres) {
	int x_mapa = obrero.x / 40, y_mapa = obrero.y / 40;
	if ((archivo[y_mapa][x_mapa] == 'z'|| archivo[y_mapa][x_mapa] == 'v') && cont_madera >= 5 && cont_piedra >= 30 && *contador_torres < MAX_TORRES) {
		cont_madera -= 5;
		cont_piedra -= 30;
		torres[*contador_torres].x = x_mapa;
		torres[*contador_torres].y = y_mapa;
		torres[*contador_torres].tipo = 1;
		if (archivo[y_mapa][x_mapa] == 'z') {
			torres[*contador_torres].lugar[0] = 'i';
		}
		else {
			torres[*contador_torres].lugar[0] = 'd';
		}
		archivo[y_mapa][x_mapa] = 'j';
		(*contador_torres)++;
	}
}

//Función para buscar recursos.
int buscar_recurso(int x_mapa, int y_mapa, recurso tipo_recurso[TAM_RECURSO]) {
	int posicion = 0;
	for (posicion = 0; posicion <= TAM_RECURSO; posicion++) {
		if (tipo_recurso[posicion].x == x_mapa && tipo_recurso[posicion].y == y_mapa) {
			return posicion;
		}
	}
	return -1;
}

//Recolector y contador piedra.
void recolectar_piedra(char archivo[Y_ARREGLO][X_ARREGLO], recurso pedreria[TAM_RECURSO]) {
	int x_mapa = obrero.x / 40, y_mapa = obrero.y / 40, indice;
	time_t tiempo_actual;
	if (recolectando) {
		if (archivo[y_mapa][x_mapa] == 'r') {
			indice = buscar_recurso(x_mapa, y_mapa, pedreria);
			tiempo_actual = time(NULL);
			if (tiempo_actual - tiempo_ult_rec >= 2) {
				cont_piedra = cont_piedra + 5;
				pedreria[indice].cant -= 5;
				if (pedreria[indice].cant <= 0) {
					archivo[y_mapa][x_mapa] = 'o';
				}
				tiempo_ult_rec = tiempo_actual;
			}
		}
		else if (archivo[y_mapa][(obrero.x + ANCHO_OBRERO - 1) / 40] == 'r') {
			indice = buscar_recurso((obrero.x + ANCHO_OBRERO - 1) / 40, y_mapa, pedreria);
			tiempo_actual = time(NULL);
			if (tiempo_actual - tiempo_ult_rec >= 2) {
				cont_piedra = cont_piedra + 5;
				pedreria[indice].cant -= 5;
				if (pedreria[indice].cant <= 0) {
					archivo[y_mapa][(obrero.x + ANCHO_OBRERO - 1) / 40] = 'o';
				}
				tiempo_ult_rec = tiempo_actual;
			}
		}
		else if (archivo[(obrero.y + ALTO_OBRERO - 1) / 40][x_mapa] == 'r') {
			indice = buscar_recurso(x_mapa, (obrero.y + ALTO_OBRERO- 1) / 40, pedreria);
			tiempo_actual = time(NULL);
			if (tiempo_actual - tiempo_ult_rec >= 2) {
				cont_piedra = cont_piedra + 5;
				pedreria[indice].cant -= 5;
				if (pedreria[indice].cant <= 0) {
					archivo[(obrero.y + ALTO_OBRERO - 1) / 40][x_mapa] = 'o';
				}
				tiempo_ult_rec = tiempo_actual;
			}
		}
		else if (archivo[(obrero.y + ALTO_OBRERO- 1) / 40][(obrero.x + ANCHO_OBRERO - 1) / 40] == 'r') {
			indice = buscar_recurso((obrero.y + ALTO_OBRERO - 1) / 40, (obrero.x + ANCHO_OBRERO - 1) / 40, pedreria);
			tiempo_actual = time(NULL);
			if (tiempo_actual - tiempo_ult_rec >= 2) {
				cont_piedra = cont_piedra + 5;
				pedreria[indice].cant -= 5;
				if (pedreria[indice].cant <= 0) {
					archivo[(obrero.y + ALTO_OBRERO - 1) / 40][(obrero.x + ANCHO_OBRERO - 1) / 40] = 'o';
				}
				tiempo_ult_rec = tiempo_actual;
			}
		}
	}
}

//Recolector y contador madera.
void recolectar_madera(char archivo[Y_ARREGLO][X_ARREGLO], recurso bosque[TAM_RECURSO]) {
	int x_mapa = obrero.x / 40, y_mapa = obrero.y / 40, indice;
	time_t tiempo_actual;
	if (recolectando) {
		if (archivo[y_mapa][x_mapa] == 'a') {
			indice = buscar_recurso(x_mapa, y_mapa, bosque);
			tiempo_actual = time(NULL);
			if (tiempo_actual - tiempo_ult_rec >= 2) {
				cont_madera = cont_madera + 5;
				bosque[indice].cant -= 5;
				if (bosque[indice].cant <= 0) {
					archivo[y_mapa][x_mapa] = 'o';
				}
				tiempo_ult_rec = tiempo_actual;
			}
		}
		else if (archivo[y_mapa][(obrero.x + ANCHO_OBRERO - 1) / 40] == 'a') {
			indice = buscar_recurso((obrero.x + ANCHO_OBRERO - 1) / 40, y_mapa, bosque);
			tiempo_actual = time(NULL);
			if (tiempo_actual - tiempo_ult_rec >= 2) {
				cont_madera = cont_madera + 5;
				bosque[indice].cant-= 5;
				if (bosque[indice].cant <= 0) {
					archivo[y_mapa][(obrero.x + ANCHO_OBRERO - 1) / 40] = 'o';
				}
				tiempo_ult_rec = tiempo_actual;
			}
		}
		else if (archivo[(obrero.y + ALTO_OBRERO - 1) / 40][x_mapa] == 'a') {
			indice = buscar_recurso(x_mapa, (obrero.y + ALTO_OBRERO - 1) / 40, bosque);
			tiempo_actual = time(NULL);
			if (tiempo_actual - tiempo_ult_rec >= 2) {
				cont_madera = cont_madera + 5;
				bosque[indice].cant -= 5;
				if (bosque[indice].cant <= 0) {
					archivo[(obrero.y + ALTO_OBRERO- 1) / 40][x_mapa] = 'o';
				}
				tiempo_ult_rec = tiempo_actual;
			}
		}
		else if (archivo[(obrero.y + ALTO_OBRERO - 1) / 40][(obrero.x + ANCHO_OBRERO - 1) / 40] == 'a') {
			indice = buscar_recurso((obrero.x + ANCHO_OBRERO - 1) / 40, (obrero.y + ALTO_OBRERO - 1) / 40, bosque);
			tiempo_actual = time(NULL);
			if (tiempo_actual - tiempo_ult_rec >= 2) {
				cont_madera = cont_madera + 5;
				bosque[indice].cant -= 5;
				if (bosque[indice].cant <= 0) {
					archivo[(obrero.y + ALTO_OBRERO - 1) / 40][(obrero.x + ANCHO_OBRERO - 1) / 40] = 'o';
				}
				tiempo_ult_rec = tiempo_actual;
			}
		}
	}
}

//Función encargada del teclado cuando se presiona una tecla.
void proceso_teclado_input(ALLEGRO_EVENT juego, char archivo[Y_ARREGLO][X_ARREGLO], edificio torres[MAX_TORRES], int *contador_torres) {
	switch (juego.keyboard.keycode) {
		case ALLEGRO_KEY_UP: tecla_ar = true; break;
		case ALLEGRO_KEY_DOWN: tecla_ab = true; break;
		case ALLEGRO_KEY_LEFT: tecla_izq = true; break;
		case ALLEGRO_KEY_RIGHT: tecla_der = true; break;
		case ALLEGRO_KEY_Q: recolectando = true; break;
		case ALLEGRO_KEY_W: construir_torre_arqueros(archivo, torres, contador_torres); break;
		case ALLEGRO_KEY_E: construir_torre_magos(archivo, torres, contador_torres); break;
		case ALLEGRO_KEY_ESCAPE: ejecutandose = false; break;
	}
}

//Función encargada de teclado cuando se deja de pulsar una tecla.
void proceso_teclado_output(ALLEGRO_EVENT juego) {
	switch (juego.keyboard.keycode) {
		case ALLEGRO_KEY_UP: tecla_ar = false; break;
		case ALLEGRO_KEY_DOWN: tecla_ab = false; break;
		case ALLEGRO_KEY_LEFT: tecla_izq = false; break;
		case ALLEGRO_KEY_RIGHT: tecla_der = false; break;
		case ALLEGRO_KEY_Q: recolectando = false; break;
	}
}

//Función para inicializar enemigos.
int inicializar_enemigos(char archivo[Y_ARREGLO][X_ARREGLO], enemigo goblin1[CANT_ENEMIGOS], int cantidad_enemigos_actuales) {
	int y_mapa, x_mapa;
	for (y_mapa = 0; y_mapa < Y_ARREGLO; y_mapa++) {
		for (x_mapa = 0; x_mapa < X_ARREGLO; x_mapa++) {
			if (archivo[y_mapa][x_mapa] == 'x') {
				if (cantidad_enemigos_actuales == CANT_ENEMIGOS) {
					return cantidad_enemigos_actuales;
				}
				goblin1[cantidad_enemigos_actuales].y = y_mapa ;
				goblin1[cantidad_enemigos_actuales].x = x_mapa ;
				goblin1[cantidad_enemigos_actuales].vida = 150;
				goblin1[cantidad_enemigos_actuales].vel = 1;
				goblin1[cantidad_enemigos_actuales].vivo = true;
				cantidad_enemigos_actuales++;
			}
		}
	}
	return cantidad_enemigos_actuales;
}

//Función para inicializar las torres.
int inicializar_edificios(char archivo[Y_ARREGLO][X_ARREGLO], edificio edificios1[MAX_TORRES], int cantidad_torres_actuales) {
	int y_mapa, x_mapa;
	for (y_mapa = 0; y_mapa < Y_ARREGLO; y_mapa++) {
		for (x_mapa = 0; x_mapa < X_ARREGLO; x_mapa++) {
			if (archivo[y_mapa][x_mapa] == 'h' || archivo[y_mapa][x_mapa] == 'j') {
				if (cantidad_torres_actuales == MAX_MUNICION) {
					return cantidad_torres_actuales;
				}
				edificios1[cantidad_torres_actuales].y = y_mapa * 40;
				edificios1[cantidad_torres_actuales].x = x_mapa * 40;
				edificios1[cantidad_torres_actuales].nivel = 1;
				switch (archivo[y_mapa][x_mapa])
				{
					case 'h': edificios1[cantidad_torres_actuales].tipo= 0; break;
					case 'j': edificios1[cantidad_torres_actuales].tipo = 1; break;
					default: break;
				}
				cantidad_torres_actuales++;
			}
		}
	}
	return cantidad_torres_actuales;
}

//Función para inicializar las municiones.
int inicializar_municiones(char archivo[Y_ARREGLO][X_ARREGLO], municion municiones[MAX_MUNICION], int* cantidad_municiones_actuales,char sentido, int x, int y)
{
	int y_mapa, x_mapa;
	for (y_mapa = 0; y_mapa < Y_ARREGLO; y_mapa++) {
		for (x_mapa = 0; x_mapa < X_ARREGLO; x_mapa++) {
			if (archivo[y_mapa][x_mapa] == 'k') {
				if (*cantidad_municiones_actuales == MAX_MUNICION) {
					return *cantidad_municiones_actuales;
				}
				switch (sentido) {
					case 'i': municiones[*cantidad_municiones_actuales].y = y; municiones[*cantidad_municiones_actuales].x = x-1; break;
					case 'd': municiones[*cantidad_municiones_actuales].y = y; municiones[*cantidad_municiones_actuales].x = x+1; break;
					default: break;
				}
				municiones[*cantidad_municiones_actuales].sentido[0] = sentido;
				(*cantidad_municiones_actuales)++;
			}
		}
	}
	return *cantidad_municiones_actuales;
}

//Dibujar enemigos.
void dibujar_enemigos(char archivo[Y_ARREGLO][X_ARREGLO], enemigo goblin1[CANT_ENEMIGOS]) {
	int posicion;
	for (posicion = 0; posicion <= CANT_ENEMIGOS; posicion++) {
		if (goblin1[posicion].vivo == true) {
			al_draw_bitmap(goblin, goblin1[posicion].x * 40, goblin1[posicion].y * 40, 0);
		}	
	}
}

//Dibujar balas.
void dibujar_proyectiles(char archivo[Y_ARREGLO][X_ARREGLO], municion municiones[MAX_MUNICION]) {
	int posicion;
	for (posicion = 0; posicion <= MAX_MUNICION; posicion++) {
		if (municiones[posicion].colision == false) {
			al_draw_bitmap(municion1, municiones[posicion].x * 40, municiones[posicion].y * 40, 0);
		}
	}
}

//Dibyujar texto de derrota
void dibujar_perdida() {
	char perdida_estado_texto[50];
	snprintf(perdida_estado_texto, sizeof(perdida_estado_texto), "Has perdido!");
	al_draw_text(fuente_derrota, al_map_rgb(255, 0, 0), 500, 350, 0, perdida_estado_texto);
}